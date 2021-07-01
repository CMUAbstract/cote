# Usage: python3 generate_time_step_file.py hour minute second nanosecond /path/to/dst/
# Parameters:
#  hour: hour time step
#  minute: minute time step
#  second: second time step
#  nanosecond: nanosecond time step
#  /path/to/dst/: directory to write time-step.dat
# Output:
#  time-step.dat with the provided hour, minute, second, and nanosecond values

# import Python modules
import csv # writing out CSV file
import sys # accessing script arguments

# initialize script arguments
hour = 0
minute = 0
second = 0
nanosecond = 0
dst = ''

# parse script arguments
if len(sys.argv)==6:
  hour = int(sys.argv[1])
  minute = int(sys.argv[2])
  second = int(sys.argv[3])
  nanosecond = int(sys.argv[4])
  dst = sys.argv[5]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: '\
   'python3 generate_time_step_file.py '\
   'hour minute second nanosecond /path/to/dst/'\
  )
  exit()

# clip time step values
hour = max(0,min(hour,23))
minute = max(0,min(minute,59))
second = max(0,min(second,59))
nanosecond = max(0,min(nanosecond,999999999))

# write out time-step.dat
with open(dst+'time-step.dat',mode='w',newline='') as outfile:
  csvwriter = csv.writer(outfile)
  csvwriter.writerow(\
   ['hour','minute','second','nanosecond']\
  )
  csvwriter.writerow([\
   str(hour).zfill(2),str(minute).zfill(2),str(second).zfill(2),\
   str(nanosecond).zfill(9)\
  ])
