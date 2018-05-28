#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from math import *

filename = sys.argv[1]

with open(filename) as data:
    rows = data.read().splitlines()

costAverage= 0
costList = []

for row in rows:
    costAverage+= float(row)
    costList.append(float(row))

costAverage/= len(rows)

print("Average Cost: ",costAverage)

costVariance = 0

for i in range(len(costList)):
    costVariance += (costList[i] - costAverage) ** 2

costVariance /= len(costList)

costStandardDeviation = sqrt(costVariance)

print("Cost Standard Deviation: ",costStandardDeviation)
