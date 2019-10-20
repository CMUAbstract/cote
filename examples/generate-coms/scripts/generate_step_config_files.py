# Usage: python3 generate_step_configuration_files.py /path/to/sat.tle n h m s ns /path/to/dst/
# Input:
#  /path/to/sat.tle: satellite TLE file
#  n: number of satellite revolutions to simulate
#  h: hours between each time step
#  m: minutes between each time step
#  s: seconds between each time step
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
h   = 0
m   = 0
s   = 0
ns  = 0
dst = ''

if len(sys.argv)==8:
  tle = sys.argv[1]
  n   = int(sys.argv[2])
  h   = int(sys.argv[3])
  m   = int(sys.argv[4])
  s   = int(sys.argv[5])
  ns  = int(sys.argv[6])
  dst = sys.argv[7]
  if dst[-1] != '/':
    dst += '/'
else:
  print('Usage: python3 generate_step_configuration_files.py /path/to/sat.tle n h m s ns /path/to/dst/')
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

# Convert revolutions per day into orbit period
HOUR_PER_DAY = 24.0
hour_per_rev = HOUR_PER_DAY/rev_per_day
rev_h = math.floor(hour_per_rev)
MIN_PER_HOUR = 60.0
min_per_rev = (hour_per_rev-rev_h)*MIN_PER_HOUR
rev_m = math.floor(min_per_rev)
SEC_PER_MIN = 60.0
sec_per_rev = (min_per_rev-rev_m)*SEC_PER_MIN
rev_s = math.floor(sec_per_rev)
NS_PER_SEC = 1000000000.0
ns_per_rev = (sec_per_rev-rev_s)*NS_PER_SEC
rev_ns = math.floor(ns_per_rev)

# Determine number of steps
MAX_NS = 1000000000
MAX_SEC = 60
MAX_MIN = 60
curr_h = 0
curr_m = 0
curr_s = 0
curr_ns = 0
step = 0
revs = 0
while revs < n:
  curr_h += h
  curr_m += m
  curr_s += s
  curr_ns += ns
  while curr_ns >= MAX_NS:
    curr_s += 1
    curr_ns -= MAX_NS
  while curr_s >= MAX_SEC:
    curr_m += 1
    curr_s -= MAX_SEC
  while curr_m >= MAX_MIN:
    curr_h += 1
    curr_m -= MAX_MIN
  step += 1
  if (float(curr_h)+float(curr_m)/MIN_PER_HOUR+float(curr_s)/(SEC_PER_MIN*MIN_PER_HOUR)+float(curr_ns)/(NS_PER_SEC*SEC_PER_MIN*MIN_PER_HOUR)) > \
     (float(rev_h)+float(rev_m)/MIN_PER_HOUR+float(rev_s)/(SEC_PER_MIN*MIN_PER_HOUR)+float(rev_ns)/(NS_PER_SEC*SEC_PER_MIN*MIN_PER_HOUR)):
    curr_h -= rev_h
    curr_m -= rev_m
    curr_s -= rev_s
    curr_ns -= rev_ns
    revs += 1

# Write out configuration files
with open(dst+'num-steps.dat',mode='w',newline='') as csvfile:
  csvwriter = csv.writer(csvfile)
  csvwriter.writerow(['steps'])
  csvwriter.writerow(['{:019d}'.format(step)])
with open(dst+'time-step.dat',mode='w',newline='') as csvfile:
  csvwriter = csv.writer(csvfile)
  csvwriter.writerow(['hour','minute','second','nanosecond'])
  csvwriter.writerow(['{:02d}'.format(h),'{:02d}'.format(m),'{:02d}'.format(s),'{:09}'.format(ns)])
