#!/usr/bin/python3

import os

root = os.path.dirname(__file__)
root = os.path.dirname(root)

for path, _, files in os.walk(root):
    for file in files:
        if file.endswith(".gcda"):
            os.remove(os.path.join(path, file))
