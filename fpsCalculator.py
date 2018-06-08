#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from math import *

drawFile = open("tests/fpsResults.log", "a+")

drawGraphFile = open("tests/fpsGraph.plot", "a+")

nObjects = int(sys.argv[1])

drawFile.write("%d\n" % (nObjects))

with open("tests/fps.log") as data:
    rows = data.read().splitlines()

average= 0
elements = []

for row in rows:
    average+= float(row)
    elements.append(float(row))

average/= len(rows)

drawFile.write("Average fps: %f\n" % (average))

drawGraphFile.write("%d\t%f\n" % (nObjects, average))

variance = 0

for i in range(len(elements)):
    variance += (elements[i] - average) ** 2

variance /= len(elements)

stdDeviation = sqrt(variance)

drawFile.write("Std deviation fps: %f\n\n\n" % (stdDeviation))
