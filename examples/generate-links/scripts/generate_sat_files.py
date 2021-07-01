# Usage: python3 generate_sat_files.py /path/to/satellites.txt /path/to/dst/
# Parameters:
#  /path/to/satellites.txt: wget http://celestrak.com/NORAD/elements/?.txt
#  /path/to/dst/: directory to write .sat files with date and time prepended
# Output:
#  .sat files with date and time prepended to the three-line TLE

# import Python modules
import csv  # write CSV
import math # floor function
import os   # accessing files
import re   # regular expression
import sys  # accessing script arguments

# "constants"
HOUR_PER_DAY   = 24
MIN_PER_HOUR   = 60
SEC_PER_MIN    = 60
NS_PER_SEC     = 1000000000
SEC_PER_HOUR   = SEC_PER_MIN*MIN_PER_HOUR
MONTH_MAX      = 12
HOUR_MAX       = 23
MINUTE_MAX     = 59
SECOND_MAX     = 59
NANOSECOND_MAX = 999999999

# helper functions
def is_leap_year(year):
  return ((year%400==0) or (year%100!=0 and year%4==0))

def day_max(year,month):
  DAY_MAXES = [31,28,31,30,31,30,31,31,30,31,30,31]
  day_max = DAY_MAXES[month-1]
  if month==2 and is_leap_year(year):
    day_max += 1
  return day_max

def parse_date_time(tle_line_1):
  year       = 0
  month      = 0
  day        = 0
  hour       = 0
  minute     = 0
  second     = 0
  nanosecond = 0
  epoch = tle_line_1[18:32]
  year = 2000+int(epoch[0:2])
  if year >= 2057:
    year -= 100
  DAYS_EACH_MONTH = [31,28,31,30,31,30,31,31,30,31,30,31]
  if is_leap_year(year):
    DAYS_EACH_MONTH[1] += 1
  tle_ddd = float(epoch[2:])
  tle_doy = math.floor(tle_ddd)
  month = 1 # January (different from DAYS_EACH_MONTH indexing)
  excess = 0
  thresh = DAYS_EACH_MONTH[0]
  i = 1
  while thresh<tle_doy and i<len(DAYS_EACH_MONTH):
    month += 1
    excess = thresh
    thresh += DAYS_EACH_MONTH[i]
    i += 1
  day = tle_doy-excess
  tle_hh = tle_ddd-float(tle_doy)
  hour = math.floor(tle_hh*float(HOUR_PER_DAY))
  tle_mm = tle_hh*float(HOUR_PER_DAY)-float(hour)
  minute = math.floor(tle_mm*float(MIN_PER_HOUR))
  tle_ss = tle_mm*float(MIN_PER_HOUR)-float(minute)
  second = math.floor(tle_ss*float(SEC_PER_MIN ))
  tle_ns = tle_ss*float(SEC_PER_MIN) -float(second)
  nanosecond = round(tle_ns*float(NS_PER_SEC))
  return year, month, day, hour, minute, second, nanosecond

# initialize script arguments
src = '' # Contains TLEs
dst = '' # destination directory for produced configuration files

# parse script arguments
if len(sys.argv)==3:
  src = sys.argv[1]
  dst = sys.argv[2]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: '\
   'python3 generate_sat_files.py '\
   '/path/to/satellites.txt /path/to/dst/'\
  )
  exit()

# Read the first TLE in the file
tle_header = ''
tle_line_1 = ''
tle_line_2 = ''
with open(src, 'r') as infile:
  tle_header = infile.readline()
  tle_line_1 = infile.readline()
  tle_line_2 = infile.readline()

# Initialize data and time values
year, month, day, hour, minute, second, nanosecond = parse_date_time(tle_line_1)

# Collect the TLEs and track the earliest TLE epoch
tles = []
with open(src, 'r') as infile:
  eof = False
  while not eof:
    tle_hd = infile.readline()
    tle_l1 = infile.readline()
    tle_l2 = infile.readline()
    if tle_hd and tle_l1 and tle_l2:
      # Date and time logic: keep the earliest TLE epoch
      year_i, month_i, day_i, hour_i, minute_i, second_i, nanosecond_i = \
       parse_date_time(tle_l1)
      if (year_i<year) or\
         (year_i==year and month_i<month) or\
         (year_i==year and month_i==month and day_i<day) or\
         (year_i==year and month_i==month and day_i==day and hour_i<hour) or\
         (year_i==year and month_i==month and day_i==day and hour_i==hour and minute_i<minute) or\
         (year_i==year and month_i==month and day_i==day and hour_i==hour and minute_i==minute and second_i<second) or\
         (year_i==year and month_i==month and day_i==day and hour_i==hour and minute_i==minute and second_i==second and nanosecond_i<nanosecond):
        year       = year_i
        month      = month_i
        day        = day_i
        hour       = hour_i
        minute     = minute_i
        second     = second_i
        nanosecond = nanosecond_i
      # TLE logic
      tle_hd = \
       re.sub('[^0-9a-z]+', '-', tle_hd.strip().lower()).strip('-')+os.linesep
      tle = [tle_hd, tle_l1, tle_l2]
      tles.append(tle)
    else:
      eof = True

# Write collected TLEs, date, and time into .sat files
for tle in tles:
  sat_id = int(tle[1][2:7])
  filename = 'sat-'+str(sat_id).zfill(10)+'-'+tle[0].strip()+'.sat'
  # Write satellite local date, time, and ID
  with open(dst+filename,mode='w',newline='') as outfile:
    csvwriter = csv.writer(outfile)
    csvwriter.writerow(\
     ['year','month','day','hour','minute','second','nanosecond','id']\
    )
    csvwriter.writerow([\
     str(year).zfill(4),str(month).zfill(2),str(day).zfill(2),\
     str(hour).zfill(2),str(minute).zfill(2),str(second).zfill(2),\
     str(nanosecond).zfill(9),str(sat_id).zfill(10)\
    ])
  # Write TLE
  with open(dst+filename, 'a') as outfile:
    outfile.write(tle[0])
    outfile.write(tle[1])
    outfile.write(tle[2])
