#!/usr/bin/env python
# -*- coding: utf-8
#
#* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
# File Name : diff.py
# Creation Date : 06-11-2012
# Last Modified : Fri 25 Jan 2013 05:54:15 PM EET
# Created By : Greg Liras <gregliras@gmail.com>
#_._._._._._._._._._._._._._._._._._._._._.*/

from __future__ import print_function
from sys import argv

try:
    from termcolor import colored
except:
    def colored(st, c):
        return "\033[0;34m{0}\033[0m".format(st)

def help():
    print("""Usage: {0} <file1> <file2>""".format(argv[0]))
    exit(1)

def parse(f):
    return [map(float, line.split()) for line in f.readlines()[1:-1]]


def diff(d1,d2):
    diffed = []
    for l1,l2 in zip(d1,d2):
        diffed.append([abs(x-y) for (x,y) in zip(l1, l2)])
    return diffed
def main():
    PASS_COLOR="green"
    FAIL_COLOR="red"
    if len(argv) != 3:
        help()
    else:
        f1 = open(argv[1], 'r')
        d1 = parse(f1)
        f1.close()
        f2 = open(argv[2], 'r')
        d2 = parse(f2)
        f2.close()

        err = (0, 0, 0)
        diffed = diff(d1, d2)
        for i,vi in enumerate(diffed):
            for j,vj in enumerate(vi):
                if vj > 1e-02:
                    err = max((err, (vj, i, j)))
                    #print(colored("<{0}> <{1}>: Failed with error value {2} on A[{3}][{4}]".format(argv[1], argv[2], vj, i, j), FAIL_COLOR))
                    #exit(1)
        print(colored("<{0}> <{1}>: Failed with error value {2}".format(argv[1], argv[2], err), FAIL_COLOR))
        print(colored( "<{0}> <{1}> Pass :)".format(argv[1], argv[2]), PASS_COLOR ))
        exit(0)



if __name__=="__main__":
    main()

