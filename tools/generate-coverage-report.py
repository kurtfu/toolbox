#!/usr/bin/python3

import os
import argparse
import shutil
import subprocess
import sys

if shutil.which('gcovr') is None:
    sys.exit('Gcovr not found in the path!')

parser = argparse.ArgumentParser(description='Generate test coverage report.')

parser.add_argument('compiler', nargs='?', type=str,
                    choices=['clang', 'gnu'], default='gnu',
                    help='the compiler used to build the program (default: gnu)')

args = parser.parse_args()

root = os.path.dirname(__file__)
root = os.path.dirname(root)

output = os.path.join(root, 'docs')
output = os.path.join(output, 'coverage')
output = os.path.join(output, 'coverage.html')

if not os.path.exists(os.path.dirname(output)):
    os.makedirs(os.path.dirname(output))

gcov = 'llvm-cov gcov' if args.compiler == 'clang' else 'gcov'

subprocess.run(['gcovr', '--gcov-executable', gcov, '--html-details',
                '-r', root, '-o', output,
                '--exclude', '.*build.*', '--exclude', '.*tests.*'
                ])
