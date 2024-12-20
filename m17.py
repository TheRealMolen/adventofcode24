
def read_input():
    with open('data/day17.txt') as inf:
        return inf.readlines()
    

def extract_reg(input, regIx):
    regline = input[regIx]
    return int(regline.split(': ')[1])
def extract_program(input):
    progline = input[-1]
    return [int(i) for i in progline.split(": ")[1].split(',')]


TRUE_OPCODES = [ 'ADV', 'BXL', 'BST', 'JNZ', 'BXC', 'OUT', 'BDV', 'CDV' ]
OPCODES = [ 'SHR', 'XOR', 'AND', 'JNZ', 'XOR', 'OUT', 'SHR', 'SHR' ]
OPERAND_PREFIX = [ 'A, ', '', '', '', '', '', 'B, ', 'C, ' ]

def decodeCombo(operand):
    if operand < 4:
        return str(operand)
    return 'ABC'[operand - 4]

def disassemble(programOctets):
    for ip in range (0, len(programOctets), 2):
        opcode = programOctets[ip]
        operand = programOctets[ip+1]

        opcodeStr = OPCODES[opcode]
        operandsStr = ''

        if opcode in [0, 6, 7]:
            operandsStr = OPERAND_PREFIX[opcode] + 'A, ' + decodeCombo(operand)
        elif opcode == 1:
            operandsStr = 'B, B, ' + str(operand)
        elif opcode == 2:
            operandsStr = 'B, ' + decodeCombo(operand) + ', 7'
        elif opcode == 3:
            operandsStr = str(operand)
        elif opcode == 4:
            operandsStr = 'B, B, C'
        elif opcode == 5:
            operandsStr += decodeCombo(operand) + ' [mod8]'
        else:
            raise Exception(f'bad opcode {opcode}')

        print(f'{opcode},{operand}\t{opcodeStr}\t{operandsStr}')


INPUT = read_input()
disassemble(extract_program(INPUT))



# 2,4     AND     B, A, 7
# 1,1     XOR     B, B, 1
# 7,5     SHR     C, A, B
# 1,4     XOR     B, B, 4
# 0,3     SHR     A, A, 3
# 4,5     XOR     B, B, C
# 5,5     OUT     B [mod8]
# 3,0     JNZ     0
def runProg(initA):
    a = initA
    b = 0
    c = 0

    output = []
    while a != 0:
        b = a & 7
        b = b ^ 1
        c = a >> b

        b = b ^ 4
        b = b ^ c

        output.append(b & 0x7)

        a = a // 8

    return output


def decodeRemainder(target, i, a):
    if i == len(target):
        return a
    
    for guess in range(8):
        aposs = (a * 8) + guess
        bposs = guess ^ 1
        cposs = aposs >> bposs
        bposs = bposs ^ 4
        bposs = bposs ^ cposs
        bposs = bposs & 7

        if bposs == target[i]:
            decoded = decodeRemainder(target, i+1, aposs)
            if decoded:
                return decoded
            
    return None


def runProgReverse(output):
    
    target = [v for v in reversed(output)]
    a = decodeRemainder(target, 0, 0)

    # for v in reversed(output):
    #     print(f'reversing {v}. existing A={a}');

    #     for guess in range(8):
    #         aposs = (a * 8) + guess
    #         bposs = guess ^ 1
    #         cposs = aposs >> bposs
    #         bposs = bposs ^ 4
    #         bposs = bposs ^ cposs
    #         bposs = bposs & 7

    #         if bposs == v:
    #             a = aposs
    #     else:
    #         print('badness')
    #         a = a * 8

    return a


print(f'Initial run')
print(runProg(extract_reg(INPUT, 0)))
print('\n\n')


A = runProgReverse(extract_program(INPUT))
print(A)
print('GOAL:\n  ' + repr(extract_program(INPUT)))
print('ACTUAL:\n  ' + repr(runProg(A)))
