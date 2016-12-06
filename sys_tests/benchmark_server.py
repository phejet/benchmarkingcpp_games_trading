#!/usr/bin/env python

import unittest
import os
import platform
import subprocess
import shutil
import random
from math import sqrt
from timelog_parser import parse_timings

IS_WINDOWS = os.name == 'nt'
IS_LINUX = os.name == 'posix' and platform.system() != "Darwin"

SYS_TEST_DIR = os.path.dirname(os.path.abspath(__file__))
BINARY_NAME = 'fizzbuzz_server'
WORKSPACE_NAME = 'workspace'
if IS_WINDOWS:
    BINARY_EXT = '.exe'


TIMINGS_LOG = 'fizzbuzz_timings.log'

if IS_WINDOWS:
    FULL_BINARY_PATH = os.path.join(SYS_TEST_DIR, '..', 'build', BINARY_NAME, 'Release', BINARY_NAME + BINARY_EXT)
else:
    FULL_BINARY_PATH = os.path.join(SYS_TEST_DIR, '..', 'build', 'release', BINARY_NAME, BINARY_NAME)


SIMULATION_FILENAME = 'simulation_data.txt'
random.seed(42)

class Color:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    FAIL1 = '\033[95m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

class FizzBuzzServerBenchmark(unittest.TestCase):

    def _make_link(self, link, target):
        if IS_WINDOWS:
            subprocess.call(['mklink', link, os.path.abspath(target)],
                             stdout=subprocess.PIPE, shell=True)
        else:
            os.symlink(os.path.abspath(target), link)

    def _print_stats(self, entries):
        def _percentiles(name, v):
            v = sorted(v)
            l = len(v)
            mean = sum(v) / l
            stddev = sqrt(sum([(x - mean) ** 2 for x in v]) / (l - 1))
            return '{:>10}{:>11}{:>12}{:>13}{:>14}{:>15}{:>16}{:>17}{:>18}'.format(
                name, mean, v[int(l * 0.25)], v[int(l * 0.5)], v[int(l * 0.75)], v[int(l * 0.9)], v[int(l * 0.99)], v[int(l * 0.999)], '%.2f' % stddev)

        print Color.HEADER + '{:>10}{:>11}{:>12}{:>13}{:>14}{:>15}{:>16}{:>17}{:>18}'.format('Name', 'avg', '25%', '50%', '75%', '90%', '99%', '99.9%', 'stddev') + Color.ENDC
        print _percentiles('Parsing', entries['Parsing'])
        print _percentiles('Processing', entries['Processing'])
        print _percentiles('Send', entries['Send'])
        print Color.OKBLUE + _percentiles('Total', entries['Total']) + Color.ENDC

    def _run_benchmark(self):
        '''Run process under test on simulation data and print timings'''
        os.chdir(self.workspace_dir)
        cmd = '%s %s > output.txt' % (FULL_BINARY_PATH, SIMULATION_FILENAME)
        if IS_LINUX:
            cmd = 'taskset -c 1 ' + cmd
        os.system(cmd)

        print '\n' + self._testMethodName
        parsed_timings = parse_timings(os.path.join(self.workspace_dir, TIMINGS_LOG))['TimeLogEntry']
        self._print_stats(parsed_timings)
 
    def _configure_workspace(self):
        self.workspace_dir = os.path.join(SYS_TEST_DIR, WORKSPACE_NAME, *self.id().split('.')[1:])
        # clean previous workspace and create a new one
        shutil.rmtree(self.workspace_dir, ignore_errors=True)
        if not os.path.exists(self.workspace_dir):
            os.makedirs(self.workspace_dir)
        self._make_link(os.path.join(self.workspace_dir, BINARY_NAME), FULL_BINARY_PATH)

    def setUp(self):
        # configure workspace, simlink binary into the workspace directory
        self._configure_workspace()
        super(FizzBuzzServerBenchmark, self).setUp()

    def test_bursts_large_numbers(self):
        # generate test data
        NUM_REQUESTS = 1000000

        file = ''
        for i in range(NUM_REQUESTS):
            file += '%d %d\n' % (random.randint(1000, 1000000), 0)
        with open(os.path.join(self.workspace_dir, SIMULATION_FILENAME), 'w') as f:
            f.write(file)

        self._run_benchmark()

    def test_bursts_small_numbers(self):
        # generate test data, small inputs
        NUM_REQUESTS = 1000000

        file = ''
        for i in range(NUM_REQUESTS):
            file += '%d %d\n' % (random.randint(1, 10), 0)
        with open(os.path.join(self.workspace_dir, SIMULATION_FILENAME), 'w') as f:
            f.write(file)

        self._run_benchmark()

if __name__ == '__main__':
    unittest.main()