#!/usr/bin/env python3
import sys

sys.stderr.write("\nPython: launching infinite loop.\n")

while (1):
	continue

sys.stderr.write("Python: this is after loop and will never get printed.\n")
