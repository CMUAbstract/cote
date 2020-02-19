# Usage: python3 parse_and_collate_logs.py /path/to/logs/ /path/to/dest/
# Parameters:
#  /path/to/logs/: contains directories, each containing log files
#  /path/to/dest/: where the parsed and collated file will be placed
# Output:
#  A CSV file of altitude-deadline pairs

import csv
import os
import sys

def calc_julian_day_from_ymd(year,month,day):
  monthm14d12 = int((month-14)/12)
  jd = day-32075+1461*(year+4800+monthm14d12)//4+367*(month-2-monthm14d12*12)//12-3*((year+4900+monthm14d12)//100)//4
  return float(jd)-0.5

def calc_sec_since_midnight(hour,minute,second):
  return second+60*(minute+60*hour)

src = ''
dst = ''

if len(sys.argv)==3:
  src = sys.argv[1]
  if src[-1] != '/':
    src += '/'
  dst = sys.argv[2]
  if dst[-1] != '/':
    dst += '/'
  with open(dst+'collated.csv',mode='w',newline='') as csv_o:
    csvwriter = csv.writer(csv_o)
    csvwriter.writerow(['altitude-km','deadline-s'])
else:
  print("Usage: python3 parse_and_collate_logs.py /path/to/logs/ /path/to/dest/")
  exit()

src_contents = os.listdir(src)
src_contents.sort()
for e in src_contents:
  if os.path.isdir(src+e):
    logdir = src+e+'/'
    idstr  = e[3:]
    with open(logdir+'meas-sat-'+idstr+'-gtfs.csv',newline='') as csv_i:
      #print('meas-sat-'+idstr+'-gtfs.csv')
      csvreader = csv.reader(csv_i,delimiter=',')
      header = next(csvreader)
      row_prev = next(csvreader)
      row_curr = next(csvreader)
      while row_prev and row_curr:
        # Get the years
        year_prev = int(row_prev[0][0:4])
        year_curr = int(row_curr[0][0:4])
        # Get the months
        month_prev = int(row_prev[0][5:7])
        month_curr = int(row_curr[0][5:7])
        # Get the days
        day_prev = int(row_prev[0][8:10])
        day_curr = int(row_curr[0][8:10])
        # Get the hours
        hour_prev = int(row_prev[0][11:13])
        hour_curr = int(row_curr[0][11:13])
        # Get the minutes
        minute_prev = int(row_prev[0][14:16])
        minute_curr = int(row_curr[0][14:16])
        # Get the seconds
        second_prev = int(row_prev[0][17:19])
        second_curr = int(row_curr[0][17:19])
        # Get the nanoseconds
        nanosecond_prev = int(row_prev[0][20:29])
        nanosecond_curr = int(row_curr[0][20:29])
        # Calculate the difference in days
        jd_prev = calc_julian_day_from_ymd(year_prev,month_prev,day_prev)
        jd_curr = calc_julian_day_from_ymd(year_curr,month_curr,day_curr)
        jd_diff = jd_curr-jd_prev
        # Calculate the difference in seconds
        sec_prev = calc_sec_since_midnight(hour_prev,minute_prev,second_prev)
        sec_curr = calc_sec_since_midnight(hour_curr,minute_curr,second_curr)
        sec_diff = sec_curr-sec_prev
        # Calculate the difference in nanoseconds
        nanosecond_diff = nanosecond_curr-nanosecond_prev
        # Calculate the total difference in seconds
        deadline = jd_diff*86400.0+float(sec_diff)+float(nanosecond_diff)/1000000000.0
        # Average the altitudes
        altitude = (float(row_prev[1])+float(row_curr[1]))/2.0
        # Write out
        with open(dst+'collated.csv',mode='a',newline='') as csv_o:
          csvwriter = csv.writer(csv_o)
          csvwriter.writerow(['{:+05.6f}'.format(altitude),'{:+03.6f}'.format(deadline)])
        # Loop update
        try:
          row_prev = next(csvreader)
          row_curr = next(csvreader)
        except StopIteration:
          break
      #print('  complete')
