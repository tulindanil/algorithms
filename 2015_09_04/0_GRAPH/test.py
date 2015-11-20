from pygraph.classes.digraph import digraph
from sets import Set
import time, os, pipes, sys
from subprocess import PIPE, Popen

def generateGraph(graph, n = 100, avgNeigbors = 10):
    from random import randint
    for i in range(n):
        try:
            graph.add_node(i)
        except Exception as e:
            ()

        for m in range(avgNeigbors-randint(-avgNeigbors/2,avgNeigbors/2)):
            try:
                graph.add_node(m)
            except Exception as e:
                ()

            graph.add_edge((i, m))

def generateDescription(graph):

    cmd = str(len(g.nodes())) + ' ' + str(len(g.edges())) + '\n'
          
    for edge in g.edges():  
        cmd += str(edge[0]) + ' ' + str(edge[1]) + '\n'

    return cmd

def dfs_test(g):

    data = dfs(g)
    schedule = (data[1], data[2])

    print schedule

    p = Popen('./test', stdin=PIPE, stdout=PIPE, bufsize=1)
          
    p.stdin.write('dfs\n')
    p.stdin.write(generateDescription(g))
             
    for line in p.stdout:
        print line

def scc_test(g):
    
    p = Popen('./test', stdin = PIPE, stdout = PIPE, bufsize = 1)

    p.stdin.write('scc\n')
    p.stdin.write(generateDescription(g))
    p.stdin.write('c\n')

    test_scc1 = Set()

    for line in p.stdout:
        test_scc1.add(Set(line[:-2].split()))

    p = Popen('./test', stdin = PIPE, stdout = PIPE, bufsize=1)
    
    p.stdin.write('scc\n')
    p.stdin.write(generateDescription(g))
    p.stdin.write('k\n')
    
    test_scc2 = Set()
    
    for line in p.stdout:
        test_scc2.add(Set(line[:-2].split()))

    if Set(test_scc1) != Set(test_scc2):
        print 'ooops'

def condensed(g):

    p = Popen('./test', stdin = PIPE, stdout = PIPE, bufsize = 1)

    p.stdin.write('condensed\n')
    p.stdin.write(generateDescription(g))

    print generateDescription(g)

    for line in p.stdout:
        print line[:-1]

if __name__ == '__main__':

    os.system('make')

    for x in range(10, 11):

        g = digraph()
        generateGraph(g, 4, 2)
        condensed(g)
