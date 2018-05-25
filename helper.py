#! /usr/bin/env python

def h1():
    s = ""
    for i in range(8):
        s += "B[block_j+%d][i]=t%d;" % (i, i)
        if i == 3:
            s += "\n"
        else:
            s += " "
    print(s)

def h2():
    s = ""
    for i in range(4):
        s += "t%d=B[block_j+i][block_i+%d];" % (i, i+4)
        if i == 3:
            s += "\n"
        else:
            s += " "
    print(s)

def h3():
    s = ""
    for i in range(4):
        s += "t%d=A[block_i+%d][block_j+i];" % (i+4, i+4)
        if i == 3:
            s += "\n"
        else:
            s += " "
    print(s)

def h4():
    s = ""
    for i in range(4):
        s += "B[block_j+i][block_i+%d]=t%d;" % (i+4, i+4)
        if i == 3:
            s += "\n"
        else:
            s += " "
    print(s)

def h5():
    s = ""
    for i in range(4):
        s += "B[block_j+4+i][block_i+%d]=t%d;" % (i, i)
        if i == 3:
            s += "\n"
        else:
            s += " "
    print(s)

def h6():
    s = ""
    for i in range(4):
        s += "t%d=A[block_i+i][block_j+%d];" % (i, i+4)
        if i == 3:
            s += "\n"
        else:
            s += " "
    print(s)

def h7():
    s = ""
    for i in range(4):
        s += "B[block_j+%d][block_i+4+i]=t%d;" % (i, i)
        if i == 3:
            s += "\n"
        else:
            s += " "
    print(s)

#h2()
#h3()
#h4()
#h5()

h7()