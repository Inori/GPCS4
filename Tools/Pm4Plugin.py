
# used to debug in pycharm
DEBUG = 0


def connect_debug_server():
    import sys
    sys.path.append("F:/Software/JetBrains/PyCharm 2022.1.2/debug-eggs/pydevd-pycharm.egg")
    import pydevd_pycharm
    pydevd_pycharm.settrace('localhost', port=12345, stdoutToServer=True, stderrToServer=True)


if DEBUG:
    connect_debug_server()


import ida_funcs
import ida_typeinf
import ida_hexrays
import ida_lines
import idc
import struct
from collections import OrderedDict



def u64(data, start=0):
    return struct.unpack("<Q", data[start:start + 8])[0]


def u32(data, start=0):
    return struct.unpack("<I", data[start:start + 4])[0]


def u16(data, start=0):
    return struct.unpack("<H", data[start:start + 2])[0]


def u8(data, start=0):
    return struct.unpack("<B", data[start:start + 1])[0]


def read_entry(addr):
    pm4type = u32(idc.get_bytes(addr, 4))
    opcode = u32(idc.get_bytes(addr + 4, 4))
    handler = u64(idc.get_bytes(addr + 8, 8))
    return pm4type, opcode, handler


def parse_function(ea):
    func = ida_funcs.func_t(ea)
    res = ida_funcs.find_func_bounds(func, ida_funcs.FIND_FUNC_DEFINE)

    if res == ida_funcs.FIND_FUNC_UNDEF:
        idc.ask_yn(1, 'can not find func bounds.')
        exit(0)

    addr = func.start_ea
    hit_count = 0
    gnm_name = ''
    packet_count = 0
    while addr < func.end_ea:
        if idc.print_insn_mnem(addr) == 'call':
            sub_name = idc.print_operand(addr, 0)
            if 'SetAllocatedNodes@SchedulerProxy' in sub_name:
                arg_addrs = ida_typeinf.get_arg_addrs(addr)
                gnm_name_arg_addr = arg_addrs[1]
                if idc.print_insn_mnem(gnm_name_arg_addr) != 'lea':
                    idc.ask_yn(1, 'gnm name not passed by lea: {:X}'.format(gnm_name_arg_addr))
                    exit(0)
                name_addr = idc.get_operand_value(gnm_name_arg_addr, 1)
                gnm_name = idc.get_strlit_contents(name_addr).decode('ascii')
                hit_count += 1
            if 'set_packet_count' in sub_name:
                # we need to manually set set_packet_count function's type (press key Y)
                # or get_arg_addrs will return None
                arg_addrs = ida_typeinf.get_arg_addrs(addr)
                packet_count_arg_addr = arg_addrs[1]
                packet_count_inst = idc.print_insn_mnem(packet_count_arg_addr)
                if packet_count_inst == 'mov':
                    op_type = idc.get_operand_type(packet_count_arg_addr, 1)
                    if op_type == idc.o_imm:
                        packet_count = idc.get_operand_value(packet_count_arg_addr, 1)
                    else:
                        packet_count = 0
                elif packet_count_inst == 'lea':
                    packet_count = 0
                else:
                    idc.ask_yn(1, 'packet count passed by {} at {:X}'.format(packet_count_inst, packet_count_arg_addr))
                    exit(0)

                hit_count += 1
        if hit_count == 2:
            break
        addr = idc.next_head(addr)
    return gnm_name, packet_count

def parse_table(ea):
    table = u64(idc.get_bytes(ea, 8))
    addr = table

    func_table = []
    while True:
        pm4type, opcode, handler = read_entry(addr)
        if handler == 0:
            break
        # get gnm name and packet count
        gnm_name, packet_count = parse_function(handler)
        # decompile the handler
        func = ida_hexrays.decompile(handler)
        lines = func.get_pseudocode()
        code_lines = []
        for sline in lines:
            line = ida_lines.tag_remove(sline.line)
            code_lines.append(line)

        print('{}\t{} {} {:X} {:08X}'.format(gnm_name, packet_count, pm4type, opcode, handler))
        if not gnm_name:
            gnm_name = 'sub_{:X}'.format(handler)
        func_table.append((gnm_name, packet_count, pm4type, opcode, handler, code_lines))
        addr += 16
    return func_table


def filter_table(func_table):
    # filter out some repeat handler
    # and rename overload functions
    new_table = []

    handler_set = set()
    func_dic = {}
    for (gnm_name, packet_count, pm4type, opcode, handler, code_lines) in func_table:
        # filter repeat handler
        if handler in handler_set:
            continue
        handler_set.add(handler)

        name = ''
        if gnm_name not in func_dic:
            func_dic[gnm_name] = 1
            name = gnm_name
        else:
            func_dic[gnm_name] += 1
            num = func_dic[gnm_name]
            name = '{}{}'.format(gnm_name, num)

        new_table.append((name, packet_count, pm4type, opcode, handler, code_lines))
    return new_table


def get_enum_name(gnm_name):
    name = gnm_name[0].upper() + gnm_name[1:]
    return 'kPacketCount{}'.format(name)


def write_packet_count_enum(table):
    fout = open('PacketEnum.h', 'w')
    fout.write('enum PacketCount : uint32_t\n')
    fout.write('{\n')
    for (gnm_name, packet_count, pm4type, opcode, handler, code_lines) in table:
        enum = get_enum_name(gnm_name)
        line = '\t{} = {},\n'.format(enum, packet_count)
        fout.write(line)
    fout.write('};\n')
    fout.close()


def get_max_code(table):
    max_code = 0
    max_sub_code = 0
    code_dic = {}
    for (gnm_name, packet_count, pm4type, opcode, handler, code_lines) in table:
        if opcode > max_code:
            max_code = opcode

        if opcode not in code_dic:
            code_dic[opcode] = 1
        else:
            code_dic[opcode] += 1

        count = code_dic[opcode]
        if count > max_sub_code:
            max_sub_code = count

    return max_code, max_sub_code


def get_proxy_func_name(gnm_name):
    name = 'GnmCommandProxy::{}'.format(gnm_name)
    return name


def get_code_name(opcode):
    op_dic = {
        0x00000010: 'IT_NOP',
        0x00000011: 'IT_SET_BASE',
        0x00000012: 'IT_CLEAR_STATE',
        0x00000013: 'IT_INDEX_BUFFER_SIZE',
        0x00000015: 'IT_DISPATCH_DIRECT',
        0x00000016: 'IT_DISPATCH_INDIRECT',
        0x00000017: 'IT_INDIRECT_BUFFER_END',
        0x00000019: 'IT_INDIRECT_BUFFER_CNST_END',
        0x0000001d: 'IT_ATOMIC_GDS',
        0x0000001e: 'IT_ATOMIC_MEM',
        0x0000001f: 'IT_OCCLUSION_QUERY',
        0x00000020: 'IT_SET_PREDICATION',
        0x00000021: 'IT_REG_RMW',
        0x00000022: 'IT_COND_EXEC',
        0x00000023: 'IT_PRED_EXEC',
        0x00000024: 'IT_DRAW_INDIRECT',
        0x00000025: 'IT_DRAW_INDEX_INDIRECT',
        0x00000026: 'IT_INDEX_BASE',
        0x00000027: 'IT_DRAW_INDEX_2',
        0x00000028: 'IT_CONTEXT_CONTROL',
        0x0000002a: 'IT_INDEX_TYPE',
        0x0000002c: 'IT_DRAW_INDIRECT_MULTI',
        0x0000002d: 'IT_DRAW_INDEX_AUTO',
        0x0000002f: 'IT_NUM_INSTANCES',
        0x00000030: 'IT_DRAW_INDEX_MULTI_AUTO',
        0x00000032: 'IT_INDIRECT_BUFFER_PRIV',
        0x00000033: 'IT_INDIRECT_BUFFER_CNST',
        # 0x00000033: 'IT_COND_INDIRECT_BUFFER_CNST',
        0x00000034: 'IT_STRMOUT_BUFFER_UPDATE',
        0x00000035: 'IT_DRAW_INDEX_OFFSET_2',
        0x00000036: 'IT_DRAW_PREAMBLE',
        0x00000037: 'IT_WRITE_DATA',
        0x00000038: 'IT_DRAW_INDEX_INDIRECT_MULTI',
        0x00000039: 'IT_MEM_SEMAPHORE',
        0x0000003a: 'IT_DRAW_INDEX_MULTI_INST',
        0x0000003b: 'IT_COPY_DW',
        0x0000003c: 'IT_WAIT_REG_MEM',
        0x0000003f: 'IT_INDIRECT_BUFFER',
        # 0x0000003f: 'IT_COND_INDIRECT_BUFFER',
        0x00000040: 'IT_COPY_DATA',
        0x00000041: 'IT_CP_DMA',
        0x00000042: 'IT_PFP_SYNC_ME',
        0x00000043: 'IT_SURFACE_SYNC',
        0x00000044: 'IT_ME_INITIALIZE',
        0x00000045: 'IT_COND_WRITE',
        0x00000046: 'IT_EVENT_WRITE',
        0x00000047: 'IT_EVENT_WRITE_EOP',
        0x00000048: 'IT_EVENT_WRITE_EOS',
        0x00000049: 'IT_RELEASE_MEM',
        0x0000004a: 'IT_PREAMBLE_CNTL',
        0x0000004c: 'IT_DRAW_RESERVED0',
        0x0000004d: 'IT_DRAW_RESERVED1',
        0x0000004e: 'IT_DRAW_RESERVED2',
        0x0000004f: 'IT_DRAW_RESERVED3',
        0x00000050: 'IT_DMA_DATA',
        0x00000051: 'IT_CONTEXT_REG_RMW',
        0x00000052: 'IT_GFX_CNTX_UPDATE',
        0x00000053: 'IT_BLK_CNTX_UPDATE',
        0x00000055: 'IT_INCR_UPDT_STATE',
        0x00000058: 'IT_ACQUIRE_MEM',
        0x00000059: 'IT_REWIND',
        0x0000005a: 'IT_INTERRUPT',
        0x0000005b: 'IT_GEN_PDEPTE',
        0x0000005c: 'IT_INDIRECT_BUFFER_PASID',
        0x0000005d: 'IT_PRIME_UTCL2',
        0x0000005e: 'IT_LOAD_UCONFIG_REG',
        0x0000005f: 'IT_LOAD_SH_REG',
        0x00000060: 'IT_LOAD_CONFIG_REG',
        0x00000061: 'IT_LOAD_CONTEXT_REG',
        0x00000062: 'IT_LOAD_COMPUTE_STATE',
        0x00000063: 'IT_LOAD_SH_REG_INDEX',
        0x00000068: 'IT_SET_CONFIG_REG',
        0x00000069: 'IT_SET_CONTEXT_REG',
        0x0000006a: 'IT_SET_CONTEXT_REG_INDEX',
        0x00000071: 'IT_SET_VGPR_REG_DI_MULTI',
        0x00000072: 'IT_SET_SH_REG_DI',
        0x00000073: 'IT_SET_CONTEXT_REG_INDIRECT',
        0x00000074: 'IT_SET_SH_REG_DI_MULTI',
        0x00000075: 'IT_GFX_PIPE_LOCK',
        0x00000076: 'IT_SET_SH_REG',
        0x00000077: 'IT_SET_SH_REG_OFFSET',
        0x00000078: 'IT_SET_QUEUE_REG',
        0x00000079: 'IT_SET_UCONFIG_REG',
        0x0000007a: 'IT_SET_UCONFIG_REG_INDEX',
        0x0000007c: 'IT_FORWARD_HEADER',
        0x0000007d: 'IT_SCRATCH_RAM_WRITE',
        0x0000007e: 'IT_SCRATCH_RAM_READ',
        0x00000080: 'IT_LOAD_CONST_RAM',
        0x00000081: 'IT_WRITE_CONST_RAM',
        0x00000083: 'IT_DUMP_CONST_RAM',
        0x00000084: 'IT_INCREMENT_CE_COUNTER',
        0x00000085: 'IT_INCREMENT_DE_COUNTER',
        0x00000086: 'IT_WAIT_ON_CE_COUNTER',
        0x00000088: 'IT_WAIT_ON_DE_COUNTER_DIFF',
        0x0000008b: 'IT_SWITCH_BUFFER',
        0x00000090: 'IT_FRAME_CONTROL',
        0x00000091: 'IT_INDEX_ATTRIBUTES_INDIRECT',
        0x00000093: 'IT_WAIT_REG_MEM64',
        0x00000094: 'IT_COND_PREEMPT',
        0x00000095: 'IT_HDP_FLUSH',
        0x00000098: 'IT_INVALIDATE_TLBS',
        0x0000009a: 'IT_DMA_DATA_FILL_MULTI',
        0x0000009b: 'IT_SET_SH_REG_INDEX',
        0x0000009c: 'IT_DRAW_INDIRECT_COUNT_MULTI',
        0x0000009d: 'IT_DRAW_INDEX_INDIRECT_COUNT_MULTI',
        0x0000009e: 'IT_DUMP_CONST_RAM_OFFSET',
        0x0000009f: 'IT_LOAD_CONTEXT_REG_INDEX',
        0x000000a0: 'IT_SET_RESOURCES',
        0x000000a1: 'IT_MAP_PROCESS',
        0x000000a2: 'IT_MAP_QUEUES',
        0x000000a3: 'IT_UNMAP_QUEUES',
        0x000000a4: 'IT_QUERY_STATUS',
        0x000000a5: 'IT_RUN_LIST',
        0x000000a6: 'IT_MAP_PROCESS_VM',
        0x0000008c: 'IT_DISPATCH_DRAW_PREAMBLE__GFX09',
        # 0x0000008c: 'IT_DISPATCH_DRAW_PREAMBLE_ACE__GFX09',
        # 0x0000008d: 'IT_DISPATCH_DRAW__GFX09',
        # 0x0000008d: 'IT_DISPATCH_DRAW_ACE__GFX09',
        0x0000008e: 'IT_GET_LOD_STATS__GFX09',
        0x0000008f: 'IT_DRAW_MULTI_PREAMBLE__GFX09',
        # 0x00000099: 'IT_AQL_PACKET__GFX09',
        # 0x0000008c: 'IT_DISPATCH_DRAW_PREAMBLE__GFX101',
        # 0x0000008c: 'IT_DISPATCH_DRAW_PREAMBLE_ACE__GFX101',
        # 0x0000008d: 'IT_DISPATCH_DRAW__GFX101',
        # 0x0000008d: 'IT_DISPATCH_DRAW_ACE__GFX101',
        # 0x0000008f: 'IT_DRAW_MULTI_PREAMBLE__GFX101',
        # 0x00000099: 'IT_AQL_PACKET__GFX101',
    }

    name = ''
    if opcode not in op_dic:
        name = 'IT_UNKNOWN_{:X}'.format(opcode)
    else:
        name = op_dic[opcode]
    return name


def write_function_array(table):

    op_dic = {}
    for (gnm_name, packet_count, pm4type, opcode, handler, code_lines) in table:
        if opcode not in op_dic:
            op_dic[opcode] = []

        proxy_name = get_proxy_func_name(gnm_name)
        op_dic[opcode].append(proxy_name)

    ordered_table = OrderedDict(sorted(op_dic.items(), key=lambda obj: obj[0]))

    fout = open('PacketArray.cpp', 'w')
    max_code, max_sub_code = get_max_code(table)
    head = 'const std::array<const std::array<ProxyFunction, {}>, {}> m_proxyTable = {{ {{\n'.format(max_sub_code, max_code)
    fout.write(head)
    last_opcode = -1
    for opcode, proxy_list in ordered_table.items():

        for i in range(last_opcode + 1, opcode):
            fout.write('\t// {}\n'.format(get_code_name(i)))
            fout.write('\t{ 0 },\n')
        last_opcode = opcode

        fout.write('\t// {}\n'.format(get_code_name(opcode)))
        fout.write('\t{\n')
        for proxy_func in proxy_list:
            fout.write('\t\t&{},\n'.format(proxy_func))
        fout.write('\t\t(ProxyFunction)0,\n')

        fout.write('\t},\n')
        fout.write('\n')

    fout.write('} };\n')
    fout.close()


def get_comment_func_body(code_lines):
    # get function body
    body = code_lines[2:-1]
    comment = ['// ' + line + '\n' for line in body]
    return comment


def recover_gnm_name(name):
    if name[-1].isdigit():
        name = name[:-1]
    return name


def define_proxy_function(name, code):
    lines = []
    lines.append('uint32_t {}(PPM4_TYPE_3_HEADER pm4Hdr)\n'.format(get_proxy_func_name(name)))
    lines.append('{\n')

    comments = get_comment_func_body(code)
    for comment in comments:
        lines.append('\t' + comment)

    lines.append('\n')

    lines.append('\t' + 'uint32_t count = 0;\n')
    lines.append('\t' + 'if (false)\n')
    lines.append('\t' + '{\n')
    lines.append('\t\t' + '// m_cb->{}();\n'.format(recover_gnm_name(name)))
    lines.append('\t\t' + 'count = {};\n'.format(get_enum_name(name)))
    lines.append('\t' + '}\n')

    lines.append('\n')

    lines.append('\t' + 'return count;\n')

    lines.append('}\n')
    return lines


def write_proxy_function_list(table):
    fout = open('PacketProxy.cpp', 'w')
    for (gnm_name, packet_count, pm4type, opcode, handler, code_lines) in table:
        proxy_func = define_proxy_function(gnm_name, code_lines)
        fout.write(''.join(proxy_func))
        fout.write('\n')
    fout.close()


def process_table(func_table):
    table = filter_table(func_table)
    write_packet_count_enum(table)
    write_proxy_function_list(table)
    write_function_array(table)


def run():

    # PS4RazorGPUDLL.dll
    # Version: 8.0.0.2
    # MD5: EBB9C82A6F451F0BA240BE17B0DC78A8
    # Offset: 2C6BC0 (base 0) init_handler_table

    print('pm4 handler parse begin.')
    # handler_table = [0x7FFC806E1120,0x7FFC806E1128,0x7FFC806E1130,0x7FFC806E1138,0x7FFC806E1140,0x7FFC806E1148]
    handler_table = [0x7FFC806E1120, 0x7FFC806E1128, 0x7FFC806E1130]

    func_table = []
    for table in handler_table:
        print('parse table:{:08X}'.format(table))
        print('================')
        func_table += parse_table(table)

    process_table(func_table)
    print('pm4 handler parse end.')


run()
