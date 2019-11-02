
def main():
    with open('constants.h', encoding='utf-8') as src, open('new_const.h', 'w', encoding='utf-8') as dst:
        dst_lines = []
        for line in src.readlines():
            if 'typedef enum ' in line:
                new_line = line.replace('typedef enum ', 'enum ')
                dst_lines.append(new_line)
            elif '///<' in line:
                pos = line.find('///<')
                new_line = line[:pos] + '\n'
                dst_lines.append(new_line)
            elif '}' in line and ';' in line:
                new_line = '};\n'
                dst_lines.append(new_line)
            else:
                dst_lines.append(line)

        for line in dst_lines:
            dst.write(line)


if __name__ == '__main__':
    main()