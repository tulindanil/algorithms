f = open('t2.txt', 'w')
n = 100000
c = 0

def fill(i):

    global c

    if c > n: 
        return
    
    c = c + 2
    f.write(str(2*i) + ' ' + str(i) + '\n')
    f.write(str(2*i + 1) + ' ' + str(i) + '\n')

    fill(2*i)
    fill(2*i + 1)


if '__main__' == __name__:

    f.write(str(n) + '\n')
    f.write('1 0\n')

    fill(1)

    f.write(str(1) + '\n')
    f.write(str(2) + ' ' + str(1) + '\n')
