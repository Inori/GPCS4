import re


ArchDic = {
    'ARCH_SOUTHERN_ISLANDS': 1,
    'ARCH_SEA_ISLANDS': 2,
    'ARCH_VOLCANIC_ISLANDS': 4,
    'ARCH_HD7X00': 1,
    'ARCH_RX2X0': 2,
    'ARCH_RX3X0': 4,
    'ARCH_RXVEGA': 8,
    'ARCH_VEGA20': 16,
    'ARCH_NAVI': 32,
    'ARCH_NAVI_DL': 64,
    'ARCH_GCN_1_0_1': 0x3,
    'ARCH_GCN_1_1_2': 0x6,
    'ARCH_GCN_1_1_5': 0x62,
    'ARCH_GCN_1_1_2_4': 0x1e,
    'ARCH_GCN_1_0_1_2_4': 0x1f,
    'ARCH_GCN_1_2_4': 0x1c,
    'ARCH_GCN_1_4': 0x18,
    'ARCH_GCN_1_5': 0x60,
    'ARCH_GCN_1_5_1': 0x40,
    'ARCH_GCN_1_1_2_4_5': 0x7e,
    'ARCH_GCN_1_2_4_5': 0x7c,
    'ARCH_GCN_1_4_5': 0x78,
    'ARCH_GCN_1_0_1_5': 0x63,
    'ARCH_GCN_ALL': 0xfff,
}

TypeDic = {
'TypeNone' : 'Undefined',
'TypeB8' : 'B8',
'TypeB16' : 'B16',
'TypeB32' : 'B32',
'TypeB64' : 'B64',
'TypeB96' : 'B96',
'TypeB128' : 'B128',
'TypeF16' : 'F16',
'TypeF32' : 'F32',
'TypeF64' : 'F64',
'TypeU8' : 'U8',
'TypeU16' : 'U16',
'TypeU24' : 'U24',
'TypeU32' : 'U32',
'TypeU64' : 'U64',
'TypeI4' : 'I4',
'TypeI8' : 'I8',
'TypeI16' : 'I16',
'TypeI24' : 'I24',
'TypeI32' : 'I32',
'TypeI64' : 'I64',
'TypeF16F32' : 'F16',
'TypeF32F16' : 'F32',
'TypeF32F64' : 'F32',
'TypeF32I32' : 'F32',
'TypeF32I4' : 'F32',
'TypeF32U32' : 'F32',
'TypeF64F32' : 'F64',
'TypeF64I32' : 'F64',
'TypeF64U32' : 'F64',
'TypeI16F32' : 'I16',
'TypeI16I32' : 'I16',
'TypeI32B32' : 'I32',
'TypeI32B64' : 'I32',
'TypeI32F32' : 'I32',
'TypeI32F64' : 'I32',
'TypeI32I16' : 'I32',
'TypeI32I24' : 'I32',
'TypeI32I64' : 'I32',
'TypeI32I8' : 'I32',
'TypeU16F32' : 'U16',
'TypeU16U32' : 'U16',
'TypeU32B32' : 'U32',
'TypeU32F32' : 'U32',
'TypeU32F64' : 'U32',
'TypeU32U24' : 'U32',
'TypeU8F32' : 'U8',
}

DefaultSrcCountTable = {
'GCNENC_SOPC' : 2,
'GCNENC_SOPP' : 0,
'GCNENC_SOP1' : 1,
'GCNENC_SOP2' : 2,
'GCNENC_SOPK' : 0,
'GCNENC_SMRD' : 1,
'GCNENC_VOPC' : 2,
'GCNENC_VOP1' : 1,
'GCNENC_VOP2' : 2,
'GCNENC_VOP3' : 3,
'GCNENC_VINTRP' : 1,
'GCNENC_DS' : 3,
'GCNENC_MUBUF' : 4,
'GCNENC_MTBUF' : 4,
'GCNENC_MIMG' : 4,
'GCNENC_EXP' : 4,
}

ARCH_SOUTHERN_ISLANDS = 1
ARCH_SEA_ISLANDS = 2

InstDefDic = {}

def WriteList(encoding, op_list):
    dst_name = encoding + '.txt'
    with open(dst_name, 'w') as dst:
        for opcode, value, mode in op_list:
            line = '{}\t{}\t{}\n'.format(opcode, value, mode)
            dst.write(line)

def IsOpInList(op_name, op_list):

    for opcode, value, mode in op_list:
        if opcode == op_name:
            return True
    return  False

def FindUniqueVOP3(op_dic, vop3_list):
    unique_vop3 = []
    for opcode, value, mode in vop3_list:
        if IsOpInList(opcode, op_dic['GCNENC_VOP1']):
            continue
        if IsOpInList(opcode, op_dic['GCNENC_VOP2']):
            continue
        if IsOpInList(opcode, op_dic['GCNENC_VOPC']):
            continue

        unique_vop3.append((opcode, value, mode))
    return unique_vop3


def GetOpKey(encoding, op_name):
    key_name = op_name

    if encoding == 'GCNENC_VOP3':
        key_name = 'V3_' + op_name[2:]
    return key_name


def GetMaxOpValue(op_list):
    count = len(op_list)
    last = op_list[count - 1]
    return last[1]

def GetMaxOpName(op_list):
    count = len(op_list)
    last = op_list[count - 1]
    return last[0]

def GetArraySize(op_list):
    return GetMaxOpValue(op_list) + 1

def ParseOpType(op_name):
    src_type = 'Undefined'
    dst_type = src_type

    dual_type_pat = re.compile('_(\S\d+)_(\S\d+)$')
    single_type_pat = re.compile('_(\S\d+)$')

    m = dual_type_pat.search(op_name)
    if m:
        dst_type = m.group(1)
        src_type = m.group(2)

        # VINTRP special
        if 'P' in dst_type:
            dst_type = src_type
    else:
        m = single_type_pat.search(op_name)
        if m:
            src_type = m.group(1)
            if 'X' in src_type:
                src_type = 'Undefined'
            dst_type = src_type
    return dst_type, src_type

def GetScalarType(op_type):
    result = 'Undefined'

    if op_type in ['B8', 'B16', 'B32', "U8", "U16", "U24", "U32"]:
        result = 'Uint32'
    elif op_type in ['B64', 'U64']:
        result = 'Uint64'
    elif op_type in ['F16']:
        result = 'Float16'
    elif op_type in ['F32']:
        result = 'Float32'
    elif op_type in ['F64']:
        result = 'Float64'
    elif op_type in ['I4', 'I8', 'I16', "I24", 'I32']:
        result = 'Sint32'
    elif op_type in ['I64']:
        result = 'Sint64'
    elif op_type in ['B96', 'B128']:
        result = 'Undefined'
    elif op_type in ['Undefined']:
        result = 'Undefined'
    else:
        input('error type {}'.format(op_type))

    return result

def GetInstCategory(ins_class):
    category = ''
    if ins_class in [
        'ScalarArith',
		'ScalarAbs',
		'ScalarMov',
		'ScalarCmp',
		'ScalarSelect',
		'ScalarBitLogic',
		'ScalarBitManip',
		'ScalarBitField',
		'ScalarConv',
		'ScalarExecMask',
		'ScalarQuadMask',]:
        category = 'ScalarALU'
    elif ins_class in [
        'VectorRegMov',
        'VectorLane',
        'VectorBitLogic',
        'VectorBitField32',
        'VectorThreadMask',
        'VectorBitField64',
        'VectorFpArith32',
        'VectorFpRound32',
        'VectorFpField32',
        'VectorFpTran32',
        'VectorFpCmp32',
        'VectorFpArith64',
        'VectorFpRound64',
        'VectorFpField64',
        'VectorFpTran64',
        'VectorFpCmp64',
        'VectorIntArith32',
        'VectorIntArith64',
        'VectorIntCmp32',
        'VectorIntCmp64',
        'VectorConv',
        'VectorFpGraph32',
        'VectorIntGraph',
        'VectorMisc',
    ]:
        category = 'VectorALU'
    elif ins_class in [
        'ScalarProgFlow',
        'ScalarSync',
        'ScalarWait',
        'ScalarCache',
        'ScalarPrior',
        'ScalarRegAccess',
        'ScalarMsg',
    ]:
        category = 'FlowControl'
    elif ins_class in [
        'ScalarMemRd',
        'ScalarMemUt',
    ]:
        category = 'ScalarMemory'
    elif ins_class in [
        'VectorMemBufNoFmt',
        'VectorMemBufFmt',
        'VectorMemImgNoSmp',
        'VectorMemImgSmp',
        'VectorMemImgUt',
        'VectorMemL1Cache',
    ]:
        category = 'VectorMemory'
    elif ins_class in [
        'DsIdxRd',
        'DsIdxWr',
        'DsIdxWrXchg',
        'DsIdxCondXchg',
        'DsIdxWrap',
        'DsAtomicArith32',
        'DsAtomicArith64',
        'DsAtomicMinMax32',
        'DsAtomicMinMax64',
        'DsAtomicCmpSt32',
        'DsAtomicCmpSt64',
        'DsAtomicLogic32',
        'DsAtomicLogic64',
        'DsAppendCon',
        'DsDataShareUt',
        'DsDataShareMisc',
        'GdsSync',
        'GdsOrdCnt',
    ]:
        category = 'DataShare'
    elif ins_class in [
        'VectorInterpFpCache',
    ]:
        category = 'VectorInterpolation'
    elif ins_class in [
        'Exp',
    ]:
        category = 'Export'
    elif ins_class in [
        'DbgProf'
    ]:
        category = 'DebugProfile'
    elif ins_class in [
            'Undefined'
        ]:
        category = 'Undefined'
    else:
        input('error instruction class')

    return category


def WriteOpFormat(op_dic):
    dst = open('OpFormat.cpp', 'w')

    for encoding, op_list in op_dic.items():

        array_size = GetArraySize(op_list)

        dst.write('const std::array<GcnInstFormat, {}> g_instructionFormat{} = {{{{\n'.format(array_size, encoding.replace('GCNENC_', '')))

        struct_array = ['\t{ },'] * array_size
        for opcode, value, mode in op_list:
            op_key = GetOpKey(encoding, opcode)

            if not op_key in InstDefDic:
                #print('inst not found: {}'.format(op_key))
                continue

            inst_info = InstDefDic[op_key]
            cls = inst_info[0]
            src_count = DefaultSrcCountTable[encoding]
            dst_type, src_type = ParseOpType(opcode)
            src_type = GetScalarType(src_type)
            dst_type = GetScalarType(dst_type)
            category = GetInstCategory(cls)

            if 'GCN_SRC_NONE' in mode:
                src_count = 0
            if 'GCN_SRC2_NONE' in mode:
                src_count -= 1
            if 'GCN_VOP3_VOP2_DS01' in mode:
                src_count = 2
            if 'GCN_VOP3_VOP1_DS0' in mode:
                src_count = 1
            if 'GCN_VOP_ARG_NONE' in mode:
                src_count = 0
            # VOPC in VOP3, specify 2 src operands
            if encoding == 'GCNENC_VOP3' and (value >= 0 and value <= 247):
                src_count = 2

            code_line = '\t// {} = {}\n\t{{ GcnInstClass::{}, GcnInstCategory::{}, {}, {},\n\t\tGcnScalarType::{}, GcnScalarType::{} }},'.\
                format(value, opcode, cls, category, src_count, 1, src_type, dst_type)
            struct_array[value] = code_line

        dst.write('\n'.join(struct_array))
        dst.write('\n}};\n\n\n')

    dst.close()

def WriteOpEnum(op_dic):
    dst = open('OpEnum.cpp', 'w')
    for encoding, op_list in op_dic.items():

        encoding_short = encoding.replace('GCNENC_', '')
        dst.write('enum class GcnOpcode{} : uint32_t\n'.format(encoding_short))
        dst.write('{\n')

        for opcode, value, mode in op_list:
            dst.write('\t{} = {},\n'.format(opcode, value))

        max_op = GetMaxOpName(op_list)
        dst.write('\n\tOP_RANGE_{} = {} + 1,\n'.format(encoding_short, max_op))
        dst.write('};\n\n')

    dst.close()

def ProcessOpcodes(op_list):
    print('opcode count:{}'.format(len(op_list)))

    op_dic = {}
    for opcode, encoding, mode, value in op_list:

        if not encoding in op_dic:
            op_dic[encoding] = []

        op_dic[encoding].append((opcode, value, mode))

    def takeOpValue(element):
        return element[1]

    for value in op_dic.values():
        value.sort(key=takeOpValue)

    WriteOpFormat(op_dic)
    WriteOpEnum(op_dic)

    for encoding, lst in op_dic.items():
        WriteList(encoding, lst)

    vop3_list = op_dic['GCNENC_VOP3']
    unique_vop3 = FindUniqueVOP3(op_dic, vop3_list)
    WriteList('VOP3_UNIQUE', unique_vop3)



def ParseInstDefs(src_name):
    src = open(src_name)
    pat = re.compile('\{(.*),.*\{(.*),(.*)\}.*\}')
    for line in src.readlines():

        if not line:
            continue
        line = line.rstrip('\n')

        m = pat.search(line)
        if not m:
            continue

        op = m.group(1).strip().split('::')[1]
        cls = m.group(2).strip().split('::')[1]
        type = m.group(3).strip().split('::')[1]
        type = TypeDic[type]

        if op in InstDefDic:
            input('already in dic:' + op)

        InstDefDic[op] = (cls, type)

    src.close()


# use
# ^    (?!\{).*\},
# in sublime to merge lines

def main():
    src = open('GCNInstructions.cpp')

    op_list = []
    pat = re.compile('\{.*\}')
    for line in src.readlines():
        line = line.rstrip('\n')
        if not line:
            continue
        m = pat.search(line)
        if not m:
            continue

        line = line.replace('{', '').replace('}', '')
        parts = line.split(',')
        parts = [x.strip() for x in parts if x]

        opcode = parts[0].replace('"', '').upper()
        encoding = parts[1]

        if encoding == 'GCNENC_VOP3A' or encoding == 'GCNENC_VOP3B':
            encoding = 'GCNENC_VOP3'

        mode = parts[2]
        value_str = parts[3]
        if 'x' in value_str:
            op_value = int(parts[3], 16)
        else:
            op_value = int(parts[3])

        arch = parts[4]
        arch_mask = ArchDic[arch]
        if not (arch_mask & ARCH_SEA_ISLANDS):
            continue

        op_list.append((opcode.upper(), encoding, mode, op_value))

    src.close()

    ParseInstDefs('GCNInstructionDefs.cpp')
    ProcessOpcodes(op_list)


if __name__ == '__main__':
    main()