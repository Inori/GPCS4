import re

def CamelCaseSplit(identifier):
    matches = re.finditer('.+?(?:(?<=[a-z])(?=[A-Z])|(?<=[A-Z])(?=[A-Z][a-z])|$)', identifier)
    return [m.group(0) for m in matches]


if __name__ == '__main__':

    with open('draw_func.txt') as src, open('enum.txt', 'w') as dst:
        for line in src.readlines():
            line = line.rstrip('\n')
            parts = CamelCaseSplit(line)
            upper_parts = [word.upper() for word in parts]
            op_name = 'OP_' + '_'.join(upper_parts)
            print(op_name)
            dst.write(op_name + '\n')