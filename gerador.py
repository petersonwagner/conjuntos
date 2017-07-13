#!/usr/bin/env python

import getopt
import sys
import random


HELP = """Uso: gerador.py -n valn -x valx -s vals -p valp -k valk -i arqi -o arqo
  -n valn      numero de conjuntos inseridos
  -x valx      numero de conjuntos removidos
  -b valb      numero de conjuntos buscados
  -s vals      numero de subconjuntos buscados
  -p valp      numero de superconjuntos buscados
  -k valk      intervalo para os elementos: [1..k]
  -e vale      porcentagem de operacoes + - = que resultam erro
  -o arqo      arquivo com as operacoes
  -r arqr      arquivo os resultados"""


def usage():
    print HELP


def printerr(s):
    sys.stderr.write('erro: ' + s + '\n')


def getargs():
    try:
        opts, args = getopt.gnu_getopt(sys.argv[1:], 
                                       'h:n:x:b:s:p:k:e:o:r:', 
                                       ['help'])
    except getopt.GetoptError, err:
        printerr(str(err))
        usage()
        sys.exit(2)

    if len(opts) < 6:
        usage()
        sys.exit(0)

    conf = dict(n=-1, x=-1, b=-1, s=-1, p=-1, k=-1, e=-1)
    for opt, val in opts:
        if opt in ('-h', '--help'):
            usage()
            sys.exit(0)
        elif opt == '-n':
            if val.isdigit():
                conf['n'] = long(val)
            if conf['n'] <= 0:
                printerr('valor invalido em -n: %s' % val)
                sys.exit(2)
        elif opt == '-x':
            if val.isdigit():
                conf['x'] = long(val)
            if conf['x'] < 0:
                printerr('valor invalido em -x: %s' % val)
                sys.exit(2)
        elif opt == '-b':
            if val.isdigit():
                conf['b'] = long(val)
            if conf['b'] < 0:
                printerr('valor invalido em -b: %s' % val)
                sys.exit(2)
        elif opt == '-s':
            if val.isdigit():
                conf['s'] = long(val)
            if conf['s'] < 0:
                printerr('valor invalido em -s: %s' % val)
                sys.exit(2)
        elif opt == '-p':
            if val.isdigit():
                conf['p'] = long(val)
            if conf['p'] < 0:
                printerr('valor invalido em -p: %s' % val)
                sys.exit(2)
        elif opt == '-k':
            if val.isdigit():
                conf['k'] = long(val)
            if conf['k'] <= 0:
                printerr('valor invalido em -k: %s' % val)
                sys.exit(2)
        elif opt == '-e':
            if val.isdigit():
                conf['e'] = float(val)
            if conf['e'] < 0:
                printerr('valor invalido em -e: %s' % val)
                sys.exit(2)
        elif opt == '-o':
            try:
                conf['o'] = open(val, 'w')
            except IOError, err:
                printerr(str(err))
                sys.exit(2)                
        elif opt == '-r':
            try:
                conf['r'] = open(val, 'w')
            except IOError, err:
                printerr(str(err))
                sys.exit(2)                
        else:
            usage()
            sys.exit(0)
    return conf


def createset(k):
    return frozenset(random.sample(xrange(1, k+1), random.randint(1, k)))


def createsets(s, n, k):
    while n:
        x = createset(k)
        if x not in s:
            s.add(x)
            n = n - 1


def writeset(f, pre, s, pos):
        f.write('%s' % pre)
        f.write('%d' % len(s))
        for j in sorted(s):
            f.write(' %d' % j)
        f.write('%s' % pos)

def sortcmp(x, y):
    res = len(x) - len(y)
    if res == 0:
        if sorted(x) < sorted(y):
            return -1
        else:
            return 1
    else:
        return res

def main():
    conf = getargs()
    n = conf['n']
    b = conf['b']
    x = conf['x']
    s = conf['s']
    p = conf['p']
    k = conf['k']
    e = conf['e']
    ofile = conf['o']
    rfile = conf['r']

    # cria o conjunto de conjuntos
    sets = set()
    createsets(sets, n, k)
    sets_list = list(sets)
    sets_list.sort(sortcmp)
    
    # escreve o valor do maior elemento
    ofile.write('%d\n' % k)

    # inclui os n novos conjuntos 
    for i in sets:
        writeset(ofile, '+ ', i, '\n')
        rfile.write('0\n')

    # inclui +/- e% conjuntos repetidos
    count = long(n * e / 100)
    while count:
        i = random.choice(sets_list)
        writeset(ofile, '+ ', i, '\n')
        rfile.write('E\n0\n')
        count = count - 1

    # busca b conjuntos que existem
    count = b
    while count:
        i = random.choice(sets_list)
        writeset(ofile, '= ', i, '\n')
        rfile.write('0\n')
        count = count - 1

    # busca +/- e% conjuntos que nao existem, aleatorios
    count = long(b * e / 100)
    while count:
        i = createset(k)
        while i in sets:
            i = createset(k)
        writeset(ofile, '= ', i, '\n')
        rfile.write('E\n0\n')
        count = count - 1

    # remove x conjuntos
    count = x
    while count:
        i = random.choice(sets_list)
        sets_list.remove(i)
        sets.remove(i)
        writeset(ofile, '- ', i, '\n')
        rfile.write('0\n')
        count = count - 1

    # remove +/- e% conjuntos que nao existem, aleatorios
    count = long(x * e / 100)
    while count:
        i = createset(k)
        while i in sets:
            i = createset(k)
        writeset(ofile, '- ', i, '\n')
        rfile.write('E\n0\n')
        count = count - 1

    # busca s subconjuntos
    count = s
    while count:
        i = set(random.choice(sets_list))
        # inclui alguns elementos no conjunto i
        alt = random.randint(0, k - len(i))
        while alt:
            novo = random.randint(1, k)
            if novo not in i:
                i.add(novo)
                alt = alt - 1
        writeset(ofile, '< ', i, '\n')
        for j in sets_list:
            if j < i:
                writeset(rfile, '', j, '\n')
        rfile.write('0\n')
        count = count - 1

    # busca p superconjuntos
    count = p
    while count:
        i = set(random.choice(sets_list))
        # exclui alguns elementos do conjunto i
        alt = random.randint(0, len(i) - 1)
        while alt:
            novo = i.pop()
            alt = alt - 1        
        writeset(ofile, '> ', i, '\n')
        for j in sets_list:
            if j > i:
                writeset(rfile, '', j, '\n')
        rfile.write('0\n')
        count = count - 1

    # termina o arquivo com as operacoes
    ofile.write('0\n')
    ofile.close()
    rfile.close()
    

if __name__ == "__main__":
    main()
