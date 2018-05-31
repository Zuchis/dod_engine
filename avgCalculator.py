#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from math import *

drawFile = open("tests/drawResults.log", "a+")
updateFile = open("tests/updateResults.log", "a+")
colisionFile = open("tests/colisionResults.log", "a+")

drawGraphFile = open("tests/drawGraph.plot", "a+")
updateGraphFile = open("tests/updateGraph.plot", "a+")
colisionGraphFile = open("tests/colisionGraph.plot", "a+")

nObjects = int(sys.argv[1])

drawFile.write("%d\n" % (nObjects))
updateFile.write("%d\n" % (nObjects))
colisionFile.write("%d\n" % (nObjects))

with open("tests/draw.log") as data:
    rows = data.read().splitlines()

average= 0
elements = []

for row in rows:
    average+= float(row)
    elements.append(float(row))

average/= len(rows)

drawFile.write("Average draw: %f\n" % (average))

drawGraphFile.write("%d\t%f\n" % (nObjects, average))

variance = 0

for i in range(len(elements)):
    variance += (elements[i] - average) ** 2

variance /= len(elements)

stdDeviation = sqrt(variance)

drawFile.write("Std deviation draw: %f\n\n\n" % (stdDeviation))

# -----------------------------------------------------

with open("tests/update.log") as data:
    rows = data.read().splitlines()

average= 0
elements = []

for row in rows:
    average+= float(row)
    elements.append(float(row))

average/= len(rows)

updateFile.write("Average update: %f\n" % (average))

updateGraphFile.write("%d\t%f\n" % (nObjects, average))

variance = 0

for i in range(len(elements)):
    variance += (elements[i] - average) ** 2

variance /= len(elements)

stdDeviation = sqrt(variance)

updateFile.write("Std deviation update: %f\n\n\n" % (stdDeviation))

# --------------------------------------------------

with open("tests/colision.log") as data:
    rows = data.read().splitlines()

average= 0
elements = []

for row in rows:
    average+= float(row)
    elements.append(float(row))

average/= len(rows)

colisionFile.write("Average colision: %f\n" % (average))

colisionGraphFile.write("%d\t%f\n" % (nObjects, average))

variance = 0

for i in range(len(elements)):
    variance += (elements[i] - average) ** 2

variance /= len(elements)

stdDeviation = sqrt(variance)

colisionFile.write("Std deviation colision: %f\n\n\n" % (stdDeviation))
