#!/usr/bin/python3

import sys


def get_func_range(func_name:str, asm_lines:list[str]):
    start = 0
    end = 0
    for i, line in enumerate(asm_lines):
        if line.startswith(func_name + ':'):
            start = i
        if start != 0 and line.find('End function') != -1:
            end = i
            break

    return start, end


def get_ret_sled(valid_ret_labels:list[str]):
    sled = []
    sled.append('\tadd $8, %rsp\n')
    if len(valid_ret_labels) == 1:
        sled.append('\tjmp {0}\n'.format(valid_ret_labels[0]))
    else:
        for label in valid_ret_labels:
            sled.append('\tcmpq $({0}), -8(%rsp)\n'.format(label))
            sled.append('\tje {0}\n'.format(label))

        sled.append('\tcall _start\n')

    return sled


def replace_rets(func_name:str, asm_lines:list[str], valid_ret_labels:list[str]):
    fstart, fend = get_func_range(func_name, asm_lines)
    for i in range(fstart, fend):
        line = asm_lines[i]
        if line.strip().startswith('retq'):
            asm_lines[i:i+1] = get_ret_sled(valid_ret_labels)

    return asm_lines


def main():
    in_asm_filename = sys.argv[1]
    out_asm_filename = sys.argv[2]
    opt_analysis_filename = sys.argv[3]

    with open(in_asm_filename, 'r') as fp:
        asm_lines = fp.readlines()

    with open(opt_analysis_filename, 'r') as fp:
        opt_analysis = fp.readlines()


    for i, line in enumerate(opt_analysis):
        callee_func = line.split(':')[0]
        if line.find(':') != len(line):
            caller_funcs = line.split(':')[1].split(',')[0:-1]

        j = 0
        label_idx = 0
        valid_ret_labels = []
        while j < len(asm_lines):
            instruction = asm_lines[j].split()
            if len(instruction) == 2 and instruction[0] == 'callq' and instruction[1] == callee_func:
                new_label = '.{0}_ret_site_{1}'.format(callee_func, label_idx)
                asm_lines.insert(j+1, '{0}:\n'.format(new_label))
                valid_ret_labels.append(new_label)
                label_idx += 1
                j += 1

            j += 1

        asm_lines = replace_rets(callee_func, asm_lines, valid_ret_labels)


    with open(out_asm_filename, 'w') as fp:
        fp.write(''.join(asm_lines))

    return


if __name__ == '__main__':
    main()
