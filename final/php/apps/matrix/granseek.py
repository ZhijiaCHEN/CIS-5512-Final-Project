#!/usr/bin/python
import os

for factor in range(166,500,1):
	os.system("cp matrix.mdl matrix.csl");
	os.system("sed -i s/GRANULARITY/"+str(factor)+"/g matrix.csl")
	os.system("prun matrix")
	os.system("sleep 1m")
