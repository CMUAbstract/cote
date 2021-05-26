# Usage: python3 generate_date_time_file.py /path/to/src/ /path/to/dst/
# Parameters:
#  /path/to/src/: directory containing *.sat files
#  /path/to/dst/: directory to write date-time.dat
# Output:
#  date-time.dat with the earliest date and time from all *.sat files

# import Python modules
import csv # writing out CSV file
import os  # accessing directory contents
import sys # accessing script arguments

# initialize script arguments
src = '' # source directory containing *.sat files
dst = '' # destination directory for date-time.dat

# parse script arguments
if len(sys.argv)==3:
  src = sys.argv[1]
  if src[-1] != '/':
    src += '/'
  dst = sys.argv[2]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: python3 generate_date_time_file.py /path/to/src/ /path/to/dst/'\
  )
  exit()

# collect the *.sat files and sort
sat_files = [f for f in os.listdir(src) if f.endswith('.sat')]
sat_files.sort()

# initialize date and time values
sat_file_0 = open(src+sat_files[0],'r')
sat_file_0_header = sat_file_0.readline()
sat_file_0_line_1 = sat_file_0.readline().strip().split(',')
sat_file_0.close()
year       = int(sat_file_0_line_1[0])
month      = int(sat_file_0_line_1[1])
day        = int(sat_file_0_line_1[2])
hour       = int(sat_file_0_line_1[3])
minute     = int(sat_file_0_line_1[4])
second     = int(sat_file_0_line_1[5])
nanosecond = int(sat_file_0_line_1[6])

# check .sat files for earlier date and time
for sat_file in sat_files:
  with open(src+sat_file,'r') as infile:
    header = infile.readline()
    line_1 = infile.readline().strip().split(',')
    year_i       = int(line_1[0])
    month_i      = int(line_1[1])
    day_i        = int(line_1[2])
    hour_i       = int(line_1[3])
    minute_i     = int(line_1[4])
    second_i     = int(line_1[5])
    nanosecond_i = int(line_1[6])
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

# write out date-time.dat
with open(dst+'date-time.dat',mode='w',newline='') as outfile:
  csvwriter = csv.writer(outfile)
  csvwriter.writerow(\
   ['year','month','day','hour','minute','second','nanosecond']\
  )
  csvwriter.writerow([\
   str(year).zfill(4),str(month).zfill(2),str(day).zfill(2),\
   str(hour).zfill(2),str(minute).zfill(2),str(second).zfill(2),\
   str(nanosecond).zfill(9)\
  ])
