def main():
    whichDo = input("Thing to do: ")
    match whichDo:
        case 'b':
            inp = input("Board:")
            inp2 = int(input("shift:")) #lowkey messed up, just put something <=0 for no shift
            var = intb(inp, inp2)
            output(var)
        case 'r':
            inp = input("Row: ")
            var = rowDuper(inp) 
            output(var)
        case 'c':
            inp = input("Column: ")
            output(columnDuper(inp))
    print()

def initFunction():
    printFiles()
    print("Rows: ")
    printRows()

def printBoard(num: int):
    binary = bin(num).split('b')[1]
    toAdd = 49 - len(binary)
    newStr = ('0' * toAdd + binary)
    for i in range(0, 49, 7):
        print(newStr[i:i+7])

def intb(benny: str, shift: int):
    if shift > 0:
        return int(benny, 2) << shift
    else:
        return int(benny, 2)

def rowDuper(benny: str):
    #assume str is size 7
    return int(benny * 7, 2)

def columnDuper(benny: str):
    #assume str is the column with a size of 7
    return int(benny * 7, 2)

def output(num: int):
    printBoard(num)
    print("Int: " + str(num))
    print("Hex: " + hex(num))

def printFiles(verify = False):
    for i in range(7):
        string = ''
        string += '0' * i
        string += '1'
        string += '0' * (6 - i)
        if verify:
            printBoard(columnDuper(string))
        print(hex(columnDuper(string)))

def printRows(verify = False):
    for i in range(7):
        string = ''
        string += ('0' * 7) * i
        string += ('1' * 7)
        string += ('0' * 7) * (6 - i)
        if verify:
            printBoard(int(string, 2))
        print(hex(int(string, 2)))

initFunction()
while True:
    main()
