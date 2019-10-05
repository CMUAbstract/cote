# Usage: python3 generate_epoch_configuration_file.py /path/to/src/ /path/to/dst/
# Parameters:
#  /path/to/src/: directory containing satellite TLE files
#  /path/to/dst/: directory to write date and time configuration file
# Output:
#  A date and time configuration set to the latest epoch among all TLEs

import csv
import math
import os
import sys

def is_leap_year(year):
  return ((year%400==0) or (year%100!=0 and year%4==0))

src = ''
dst = ''

if len(sys.argv)==3:
  src = sys.argv[1]
  if src[-1] != '/':
    src += '/'
  dst = sys.argv[2]
  if dst[-1] != '/':
    dst += '/'
else:
  print("Usage: python3 generate_epoch_configuration_file.py /path/to/src/ /path/to/dst/")
  exit()

year = 0
month = 0
day = 0
hour = 0
minute = 0
second = 0
nanosecond = 0
tles = os.listdir(src)
tles = [e for e in tles if e[-4:]=='.tle']
for tle in tles:
  with open(src+tle, 'r') as infile:
    header = infile.readline()
    epoch_time = infile.readline().split()[3]
    tle_year = 2000+int(epoch_time[0:2])
    if tle_year >= 2057:
      tle_year -= 100
    day_per_month = [31,28,31,30,31,30,31,31,30,31,30,31]
    if is_leap_year(year):
      day_per_month[1] += 1
    tle_ddd = float(epoch_time[2:])
    tle_doy = math.floor(tle_ddd)
    tle_month = 1
    excess = 0
    thresh = day_per_month[0]
    i = 1
    while thresh<tle_doy and i<12:
      tle_month += 1
      excess = thresh
      thresh += day_per_month[i]
      i += 1
    tle_day = tle_doy
    if tle_month != 1:
      tle_day -= excess
    tle_hh = tle_ddd-tle_doy
    tle_hour = math.floor(tle_hh*24.0)
    tle_mm = tle_hh*24.0-math.floor(tle_hh*24.0)
    tle_minute = math.floor(tle_mm*60.0)
    tle_ss = tle_mm*60.0-math.floor(tle_mm*60.0)
    tle_second = math.floor(tle_ss*60.0)
    tle_ns = tle_ss*60.0-math.floor(tle_ss*60.0)
    tle_nanosecond = round(tle_ns*1.0e9)
    if (tle_year>year) or \
       (tle_year==year and tle_month>month) or \
       (tle_year==year and tle_month==month and tle_day>day) or \
       (tle_year==year and tle_month==month and tle_day==day and tle_hour>hour) or \
       (tle_year==year and tle_month==month and tle_day==day and tle_hour==hour and tle_minute>minute) or \
       (tle_year==year and tle_month==month and tle_day==day and tle_hour==hour and tle_minute==minute and tle_second>second) or \
       (tle_year==year and tle_month==month and tle_day==day and tle_hour==hour and tle_minute==minute and tle_second==second and tle_nanosecond>nanosecond):
      year = tle_year
      month = tle_month
      day = tle_day
      hour = tle_hour
      minute = tle_minute
      second = tle_second
      nanosecond = tle_nanosecond
with open(dst+'date-time.dat',mode='w',newline='') as outfile:
  csvwriter = csv.writer(outfile)
  csvwriter.writerow(['year','month','day','hour','minute','second','nanosecond'])
  csvwriter.writerow([str(year).zfill(4),str(month).zfill(2),str(day).zfill(2),str(hour).zfill(2),str(minute).zfill(2),str(second).zfill(2),str(nanosecond).zfill(9)])
