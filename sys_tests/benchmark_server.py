#!/usr/bin/env python

import unittest
import os
import subprocess
import shutil
import random
from operator import sub
from math import sqrt

IS_WINDOWS = os.name == 'nt'
SYS_TEST_DIR = os.path.dirname(os.path.abspath(__file__))
BINARY_NAME = 'fizzbuzz_server'
WORKSPACE_NAME = 'workspace'
if IS_WINDOWS:
    BINARY_EXT = '.exe'


TIMINGS_LOG = 'fizzbuzz_timings.log'

if IS_WINDOWS:
    FULL_BINARY_PATH = os.path.join(SYS_TEST_DIR, '..', 'build', 'release', BINARY_NAME, 'Release', BINARY_NAME + BINARY_EXT)

SIMULATION_FILENAME = 'simulation_data.txt'
random.seed(42)

class Color:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

class FizzBuzzServerBenchmark(unittest.TestCase):

    def _make_link(self, link, target):
        if IS_WINDOWS:
            subprocess.call(['mklink', link, os.path.abspath(target)],
                             stdout=subprocess.PIPE, shell=True)
        else:
            raise NotImplementedError()

    def _print_stats(self, entries):
        dur = {}
        # discard first 10 entries
        dur['Parsing'] = sorted(map(sub, entries['finishParsingTS'], entries['startTS'])[10:])
        dur['Processing'] = sorted(map(sub, entries['finishProcessingTS'], entries['finishParsingTS'])[10:])
        dur['Send'] = sorted(map(sub, entries['finishSendTS'], entries['finishProcessingTS'])[10:])
        dur['Total'] = sorted(map(sub, entries['finishSendTS'], entries['startTS'])[10:])

        def _print_percentiles(name, v):
            l = len(v)
            mean = v[l / 2]
            sample_stddev = sqrt(sum([(x - mean) ** 2 for x in v]) / (l - 1))
            print '{:>10}{:>11}{:>12}{:>13}{:>14}{:>15}{:>16}{:>17}'.format(
                name, v[int(l * 0.25)], v[int(l * 0.5)], v[int(l * 0.75)], v[int(l * 0.9)], v[int(l * 0.99)], v[int(l * 0.999)], '%.2f' % sample_stddev)

        print '{:>10}{:>11}{:>12}{:>13}{:>14}{:>15}{:>16}{:>17}'.format('Name', '25%', '50%', '75%', '90%', '99%', '99.9%', 'stddev')
        _print_percentiles('Parsing', dur['Parsing'])
        _print_percentiles('Processing', dur['Processing'])
        _print_percentiles('Send', dur['Send'])
        _print_percentiles('Total', dur['Total'])

    def _parse_timings(self):
        '''Load timings log and parse it'''
        headers = {}
        entries = {}
        with open(os.path.join(self.workspace_dir, TIMINGS_LOG), 'r') as f:
            for line in f.readlines():
                if line.startswith('Header'):
                    record_name, fields = line.split(':')[1:]
                    record_name = record_name.strip()
                    entries[record_name] = {}
                    header = []
                    for i, field in enumerate(fields.strip().split(',')):
                        field_name, field_type = field.split('=')
                        header.append((field_name, field_type))
                        entries[record_name][field_name] = []
                    headers[record_name] = header
                elif line.startswith('Data'):
                    record_name, fields = line.split(':')[1:]
                    record_name = record_name.strip()
                    assert record_name in headers
                    fields = fields.split(',')
                    header = headers[record_name]
                    for i, field in enumerate(fields):
                        field_value = field.split('=')[1]
                        entries[record_name][header[i][0]].append(field_value.strip())
        # convert types
        for record_name, header in headers.iteritems():
            for field_name, field_type in header:
                if field_type in ['uint64', 'timestamp']:
                    entries[record_name][field_name] = [int(val) for val in entries[record_name][field_name]]

        return entries['TimeLogEntry']

    def _run_benchmark(self):
        '''Run process under test on simulation data and print timings'''
        p = subprocess.Popen([FULL_BINARY_PATH, SIMULATION_FILENAME], cwd=self.workspace_dir, stdout=subprocess.PIPE)
        out, _ = p.communicate()
        with open(os.path.join(self.workspace_dir, 'output.log'), 'w') as f:
            f.write(out)
        self._print_stats(self._parse_timings())

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

    def test_bursts(self):
        # generate test data
        NUM_REQUESTS = 100000

        file = ''
        for i in range(NUM_REQUESTS):
            file += '%d %d\n' % (random.randint(1000, 1000000), random.randint(0, 100))
        with open(os.path.join(self.workspace_dir, SIMULATION_FILENAME), 'w') as f:
            f.write(file)

        self._run_benchmark()

    def test_one_by_one(self):
        pass

if __name__ == '__main__':
    unittest.main()