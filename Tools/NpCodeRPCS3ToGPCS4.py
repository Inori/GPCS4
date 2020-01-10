import sys
import re
import os

def FixStandardType(param_list):
    new_param_list = []
    for idx, param in enumerate(param_list):
        new_param = param
        new_param = new_param.replace('u8', 'uint8_t')
        new_param = new_param.replace('u16', 'uint16_t')
        new_param = new_param.replace('u32', 'uint32_t')
        new_param = new_param.replace('u64', 'uint64_t')
        new_param = new_param.replace('s8', 'int8_t')
        new_param = new_param.replace('s16', 'int16_t')
        new_param = new_param.replace('s32', 'int32_t')
        new_param = new_param.replace('s64', 'int64_t')
        new_param_list.append(new_param)
    return new_param_list

def FixVmType(param_list):
    pat_vm = re.compile('vm::\w?ptr.*<(.*)>')
    new_param_list = []
    for idx, param in enumerate(param_list):
        m = pat_vm.search(param)
        if m:
            vm_type = m.group(0)
            org_type = m.group(1)
            if 'cptr' in vm_type:
                new_type = 'const ' + org_type + '*'
            elif 'ptr' in vm_type:
                new_type = org_type + '*'
            else:
                input('unsupported vm type {}'.format(org_type))
            new_param = param.replace(vm_type, new_type)
        else:
            new_param = param

        new_param_list.append(new_param)
    return new_param_list

def ConvertFunction(old_lines):

    pat_func_sign = re.compile(r'^error_code sceNp.*?\((.*?)\)', re.MULTILINE|re.DOTALL)
    # match function signature first
    merged_text = ''.join(old_lines)
    m = pat_func_sign.match(merged_text)
    if m:
        old_func_sign = m.group(0)
        old_param_list = m.group(1).split(',')
        new_param_list = FixStandardType(old_param_list)
        new_param_list = FixVmType(new_param_list)
        new_func_sign = old_func_sign.replace(','.join(old_param_list), ','.join(new_param_list))
    else:
        input('error, can not match function signature:\n{}'.format(merged_text))

    pat_np_todo = re.compile('sceNp.*\.(todo|warning)')
    new_lines = []

    new_func_sign = new_func_sign.replace('error_code', 'int')
    new_lines.append(new_func_sign + '\n')

    in_body = False
    lines_iter = enumerate(old_lines)
    for idx, line in lines_iter:
        if not in_body:
            if line[0] == '{':
                in_body = True
                new_lines.append(line)
        else:
            if line[0] == '}':
                in_body = False
                new_lines.append(line)
                continue

            new_line = line
            s = pat_np_todo.search(line)
            if s:
                new_line = pat_np_todo.sub('LOG_SCE_TRACE', line)
                new_line = re.sub('".*?\(', '"(', new_line)
                new_lines.append(new_line)
                if ');' in line:
                    new_lines.append('\n\tint ret = SCE_ERROR_UNKNOWN;\n')
                    new_lines.append('\tdo\n\t{\n')
                    continue
                for i in range(idx+1, len(old_lines)-1):
                    line = old_lines[i]
                    if ');' in line:
                        next(lines_iter)
                        new_lines.append(line)
                        break
                    else:
                        next(lines_iter)
                        new_lines.append(line)

                new_lines.append('\n\tint ret = SCE_ERROR_UNKNOWN;\n')
                new_lines.append('\tdo\n\t{\n')
                continue

            if 'return CELL_OK;' in line:
                new_lines.append('\t\tret = SCE_OK;\n')
                new_lines.append('\t} while(false);\n')
                new_lines.append('\treturn ret;\n')
                continue

            if not 'return' in line:
                new_lines.append('\t' + new_line)
            else:
                new_line = line.replace('return', 'ret =')
                new_lines.append('\t' + new_line)
                new_lines.append('\t\t\tbreak;\n')

    return new_lines


def main():
    if len(sys.argv) != 2:
        print('{} <source.cpp>'.format(sys.argv[0]))
        return
    src_file = open(sys.argv[1], encoding='utf8')

    pat_func_start = re.compile('error_code sceNp.*\(')
    dst_lines = []
    in_function = False
    function_lines = []
    for line in src_file.readlines():
        if not in_function:
            if pat_func_start.match(line):
                in_function = True
                function_lines.clear()
                function_lines.append(line)
            else:
                dst_lines.append(line)
        else:
            function_lines.append(line)
            if line[0] == '}' and in_function:
                in_function = False
                new_func_lines = ConvertFunction(function_lines)
                dst_lines += new_func_lines

    src_file.close()
    dst_name = os.path.basename(sys.argv[1]) + '_gpcs4.cpp'
    dst_file = open(dst_name, 'w', encoding='utf8')
    dst_file.write(''.join(dst_lines))
    dst_file.close()

if __name__ == '__main__':
    main()