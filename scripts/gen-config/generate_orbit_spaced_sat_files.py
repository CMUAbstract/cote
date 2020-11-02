# Usage: python3 generate_orbit_spaced_sat_files.py count /path/to/src/seed.tle /path/to/dst/
# Parameters:
#  count: number of satellites in the orbit-spaced constellation
#  /path/to/src/seed.tle: a three-line TLE file (header, line one, line two)
#  /path/to/dst/: directory to write .sat files with local date/time prepended
# Output:
#  .sat files with local date/time prepended to the three-line TLE

# import Python modules
import csv  # writing out configuration files
import math # floor function
import re   # regex for file name generation
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

# initialize script arguments
cnt = 0  # constellation count
src = '' # seed TLE file
dst = '' # destination directory for produced configuration files

# parse script arguments
if len(sys.argv)==4:
  cnt = int(sys.argv[1])
  src = sys.argv[2]
  dst = sys.argv[3]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: '\
   'python3 generate_orbit_spaced_sat_files.py '\
   'count /path/to/src/seed.tle /path/to/dst/'\
  )
  exit()

# read the seed TLE file
tle_header = ''
tle_line_1 = ''
tle_line_2 = ''
with open(src, 'r') as infile:
  tle_header = infile.readline()
  tle_line_1 = infile.readline()
  tle_line_2 = infile.readline()

# parse TLE epoch time
sat_year       = 0
sat_month      = 0
sat_day        = 0
sat_hour       = 0
sat_minute     = 0
sat_second     = 0
sat_nanosecond = 0
epoch = tle_line_1[18:32]
sat_year = 2000+int(epoch[0:2])
if sat_year >= 2057:
  sat_year -= 100
DAYS_EACH_MONTH = [31,28,31,30,31,30,31,31,30,31,30,31]
if is_leap_year(sat_year):
  DAYS_EACH_MONTH[1] += 1
tle_ddd = float(epoch[2:])
tle_doy = math.floor(tle_ddd)
sat_month = 1 # January (different from DAYS_EACH_MONTH indexing)
excess = 0
thresh = DAYS_EACH_MONTH[0]
i = 1
while thresh<tle_doy and i<len(DAYS_EACH_MONTH):
  sat_month += 1
  excess = thresh
  thresh += DAYS_EACH_MONTH[i]
  i += 1
sat_day = tle_doy-excess
tle_hh = tle_ddd-float(tle_doy)
sat_hour = math.floor(tle_hh*float(HOUR_PER_DAY))
tle_mm = tle_hh*float(HOUR_PER_DAY)-float(sat_hour)
sat_minute = math.floor(tle_mm*float(MIN_PER_HOUR))
tle_ss = tle_mm*float(MIN_PER_HOUR)-float(sat_minute)
sat_second = math.floor(tle_ss*float(SEC_PER_MIN ))
tle_ns = tle_ss*float(SEC_PER_MIN) -float(sat_second)
sat_nanosecond = round(tle_ns*float(NS_PER_SEC))

# parse TLE revolutions per day
rev_per_day = float(tle_line_2[52:63])

# generate orbit-spaced local time step
stp_h = 0 # step hour
stp_m = 0 # step minute
stp_s = 0 # step second
stp_n = 0 # step nanosecond
ttl_s = ((HOUR_PER_DAY*MIN_PER_HOUR*SEC_PER_MIN)/rev_per_day)/cnt # step total s
while ttl_s>=float(SEC_PER_HOUR):
  stp_h += 1
  ttl_s -= float(SEC_PER_HOUR)
while ttl_s>=float(SEC_PER_MIN):
  stp_m += 1
  ttl_s -= float(SEC_PER_MIN)
while ttl_s>=1.0:
  stp_s += 1
  ttl_s -= 1.0
stp_n = int(round(ttl_s*float(NS_PER_SEC)))

# write out orbit-spaced .sat configuration files
norad_id = int(tle_line_1[2:7])
i = cnt
while i>0:
  # calculate virtual satellite ID
  sat_id = norad_id*10000+(i-1)
  # generate file name
  filename =\
   'sat-'+str(sat_id).zfill(10)+'-'+\
   re.sub('[^0-9a-z]+', '-', tle_header.strip().lower()).strip('-')+'.sat'
  # write satellite local date, time, and ID
  with open(dst+filename,mode='w',newline='') as outfile:
    csvwriter = csv.writer(outfile)
    csvwriter.writerow(\
     ['year','month','day','hour','minute','second','nanosecond','id']\
    )
    csvwriter.writerow([\
     str(sat_year).zfill(4),str(sat_month).zfill(2),str(sat_day).zfill(2),\
     str(sat_hour).zfill(2),str(sat_minute).zfill(2),str(sat_second).zfill(2),\
     str(sat_nanosecond).zfill(9),str(sat_id).zfill(10)\
    ])
  # write TLE
  with open(dst+filename, 'a') as outfile:
    outfile.write(tle_header)
    outfile.write(tle_line_1)
    outfile.write(tle_line_2)
  # step satellite date and time forward
  sat_nanosecond += stp_n
  if sat_nanosecond > NANOSECOND_MAX:
    sat_nanosecond = (sat_nanosecond % NS_PER_SEC)
    sat_second += 1
  sat_second += stp_s
  if sat_second > SECOND_MAX:
    sat_second = (sat_second % SEC_PER_MIN)
    sat_minute += 1
  sat_minute += stp_m
  if sat_minute > MINUTE_MAX:
    sat_minute = (sat_minute % MIN_PER_HOUR)
    sat_hour += 1
  sat_hour += stp_h
  if sat_hour > HOUR_MAX:
    sat_hour = (sat_hour % HOUR_PER_DAY)
    sat_day += 1
    if sat_day > day_max(sat_year,sat_month):
      sat_day = (sat_day % day_max(sat_year,sat_month))
      sat_month += 1
      if sat_month > MONTH_MAX:
        sat_month = (sat_month % MONTH_PER_YEAR)
        sat_year += 1
  # increment loop index
  i-=1
