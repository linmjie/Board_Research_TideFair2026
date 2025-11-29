def printBoard(num: int):
    binary = bin(num).split('b')[1]
    toAdd = 49 - len(binary)
    newStr = ('0'*toAdd + binary)
    for i in range(0, 49, 7):
        print(newStr[i:i+7])

def intb(benny: str, shift: int):
    if shift > 0:
        return int(benny,2) << shift
    else:
        return int(benny,2 )

while True:
    inp = input("Board:")
    inp2 = int(input("shift:")) #lowkey messed up, just put something <=0 for no shift
    var = intb(inp, inp2)
    printBoard(var)
    print("Int: ", var)
    print("Hex: ", hex(var))
    print()
