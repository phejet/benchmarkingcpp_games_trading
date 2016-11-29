#!/usr/bin/env python

import argparse
import subprocess
import os

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

WIN_PREFIX = 'python'

def parse_command_line():
    parser = argparse.ArgumentParser()
    parser.add_argument('-b', '--build', action='store_true', help='Build project')
    parser.add_argument('-m', '--benchmark', action='store_true', help='Run system benchmarks')
    parser.add_argument('-s', '--system-test', action='store_true', help='Run system tests')
    parser.add_argument('-t', '--type', help='Build type', choices=['debug', 'release'])

    args = parser.parse_args()
    return args

def benchmark():
    print 'Running system benchmark'
    subprocess.call([WIN_PREFIX, os.path.join(ROOT_DIR, 'sys_tests', 'benchmark_server.py')])

def build(args):
    '''Generate CMake output and trigger build'''
    build_dir = os.path.join(ROOT_DIR, 'build', args.type)
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    print 'Running build %s' % args.type
    # generate cmake files
    subprocess.Popen(['cmake', ROOT_DIR], cwd=build_dir, shell=True).wait()
    # run build
    subprocess.Popen(['cmake', '--build', '.', '--target', 'ALL_BUILD', '--config', args.type], cwd=build_dir, shell=True).wait()

def main():
    args = parse_command_line()

    if args.build:
        build(args)
    if args.benchmark:
        benchmark()
    if args.system_test:
        system_test()

if __name__ == '__main__':
    main()