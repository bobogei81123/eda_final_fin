#!/usr/bin/env python3
import subprocess
from subprocess import DEVNULL, STDOUT
from os.path import join as pjoin
from os import listdir
from termcolor import colored
import time
import sys

files = [f for f in listdir('./circuits')]
testdatas = [f for f in files if f.endswith('.v')]
testdatas_with_ans = [f for f in testdatas if (f[:-2] + '_p1') in files]
# testdatas_only_s27 = ['custom.v']
testdatas_only_s27 = ['s298.v']

test_funcs = []

def testfunction(typ):
    def decorator(fun):
        test_funcs.append((fun, typ))
        return fun
    return decorator

EXEC_PATH = './tests/exec'

@testfunction(0)
def test_re(td):
    ''' Test if program parse/lexer RE '''
    ps = subprocess.run([pjoin(EXEC_PATH, 'test_re'), pjoin('circuits', td)]
                        , stderr=DEVNULL, stdout=DEVNULL)
    return ps.returncode == 0

@testfunction(1)
def test_ans_re(td):
    ''' Test if solver RE '''
    args = [pjoin('circuits', td[:-2]+x) for x in ('.v', '_p1', '_f1')]
    ps = subprocess.run([pjoin(EXEC_PATH, 'test_re_ans'),
                         *args],
                        stderr=STDOUT,
                        stdout=DEVNULL,
                        )
    return ps.returncode == 0

@testfunction(2)
def s27(td):
    ''' Test if solver RE '''
    args = [pjoin('circuits', td[:-2]+x) for x in ('.v', '_p1', '_f1')]
    ps = subprocess.run([pjoin(EXEC_PATH, 'test_s27'),
                         *args],
                        stderr=STDOUT,
                        # stdout=,
                        )
    return ps.returncode == 0

@testfunction(1)
def test_answer(td):
    ''' Test answers '''
    args = [pjoin('circuits', td[:-2]+x) for x in ('.v', '_p1', '_f1')]
    ps = subprocess.run([pjoin(EXEC_PATH, 'test_s27'),
                         *args],
                        stderr=STDOUT,
                        # stdout=,
                        )
    return ps.returncode == 0

def preform_test(fun, typ):
    tds = (testdatas, testdatas_with_ans, testdatas_only_s27)[typ]
    s = fun.__doc__
    print('Running test: %s' % s)
    data_n = len(tds)
    pass_n = 0
    tt = 0.
    for td in tds:
        start_t = time.time()
        ret = fun(td)
        total_t = time.time() - start_t

        if not ret:
            print(colored('[failed]', 'red'), 'Testdata %s failed' % td)
        else:
            pass_n += 1
            tt += total_t
            print(colored('[success]', 'green'), 
                  'Testdata {} success, time = {:.3f}'.format(td, total_t))

    print()
    print('Pass = {}/{}, total time = {:.3f}'.format(pass_n, data_n, tt))
    return data_n == pass_n

def preform_wrapper(fun, flag):
    res = preform_test(fun, flag)
    if res:
        print('->', colored('Test Success.', 'green', attrs=['bold']))
    if not res:
        print('->', colored('Test Failed.', 'red', attrs=['bold']))
        return

def preform_all():
    for fun, flag in test_funcs:
        preform_wrapper(fun, flag)

if __name__ == '__main__':
    if len(sys.argv) == 1:
        preform_all()
    else:
        tt = sys.argv[1:]
        for fun, flag in test_funcs:
            if fun.__name__ in tt:
                preform_wrapper(fun, flag)


