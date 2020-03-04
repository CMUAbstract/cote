# Usage: python3 generate_small_step_config_files.py /path/to/sat.tle n ns /path/to/dst/
# Input:
#  /path/to/sat.tle: satellite TLE file
#  n: number of satellite revolutions to simulate
#  ns: nanoseconds between each time step
#  /path/to/dst/: destination directory for time configuration files
# Output:
#  num-steps.dat (number of steps taken during the simulation), time-step.dat
#  (amount of time between each simulation step)

import csv
import math
import os
import sys

tle = ''
n   = 0
ns  = 0
dst = ''

if len(sys.argv)==5:
  tle = sys.argv[1]
  n   = int(sys.argv[2])
  ns  = int(sys.argv[3])
  dst = sys.argv[4]
  if dst[-1] != '/':
    dst += '/'
else:
  print('Usage: python3 generate_small_step_config_files.py /path/to/sat.tle n ns /path/to/dst/')
  exit()

# Read satellite revolutions per day from TLE file
rev_per_day = 0.0
with open(tle, mode='r') as infile:
  lines = infile.readlines()
  i = 0
  while i<len(lines) and lines[i][:2]!='2 ':
    i += 1
  if lines[i][:2]=='2 ':
    rev_per_day = float(lines[i][52:63])
  else:
    print('Incorrect TLE file format, exiting')
    exit()

# Convert
HOUR_PER_DAY = 24.0
MIN_PER_HOUR = 60.0
SEC_PER_MIN = 60.0
NS_PER_SEC = 1000000000.0
ns_per_rev = (1.0/rev_per_day)*HOUR_PER_DAY*MIN_PER_HOUR*SEC_PER_MIN*NS_PER_SEC
step = n*math.ceil(ns_per_rev/float(ns))

# Write out configuration files
with open(dst+'num-steps.dat',mode='w',newline='') as csvfile:
  csvwriter = csv.writer(csvfile)
  csvwriter.writerow(['steps'])
  csvwriter.writerow(['{:019d}'.format(step)])
with open(dst+'time-step.dat',mode='w',newline='') as csvfile:
  csvwriter = csv.writer(csvfile)
  csvwriter.writerow(['hour','minute','second','nanosecond'])
  csvwriter.writerow(['{:02d}'.format(0),'{:02d}'.format(0),'{:02d}'.format(0),'{:09}'.format(ns)])
