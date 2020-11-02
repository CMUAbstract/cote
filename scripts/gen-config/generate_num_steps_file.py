# Usage: python3 generate_num_steps_file.py rev_count /path/to/seed.tle /path/to/time-step.dat /path/to/dst/
# Parameters:
#  rev_count: number of satellite revolutions to simulate
#  /path/to/seed.tle: a three-line TLE file (header, line one, line two)
#  /path/to/time-step.dat: time-step.dat configuration file
#  /path/to/dst/: directory to write num-steps.dat
# Output:
#  num-steps.dat configuration file

# import Python modules
import csv  # writing out configuration file
import math # ceiling function
import sys  # accessing script arguments

# "constants"
HOUR_PER_DAY = 24.0
MIN_PER_HOUR = 60.0
SEC_PER_MIN = 60.0
NS_PER_SEC = 1000000000.0

# initialize script arguments
cnt = 0.0 # revolution count
tle = ''  # seed TLE file
dat = ''  # time-step.dat
dst = ''  # destination directory for produced configuration file

# parse script arguments
if len(sys.argv)==5:
  cnt = float(sys.argv[1])
  tle = sys.argv[2]
  dat = sys.argv[3]
  dst = sys.argv[4]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: '\
   'python3 generate_num_steps_file.py '\
   'rev_count /path/to/seed.tle /path/to/time-step.dat /path/to/dst/'\
  )
  exit()

# read the seed TLE file
tle_header = ''
tle_line_1 = ''
tle_line_2 = ''
with open(tle, 'r') as infile:
  tle_header = infile.readline()
  tle_line_1 = infile.readline()
  tle_line_2 = infile.readline()

# parse TLE revolutions per day
rev_per_day = float(tle_line_2[52:63])

# convert revolutions per day to nanoseconds per revolution
ns_per_rev = (1.0/rev_per_day)*HOUR_PER_DAY*MIN_PER_HOUR*SEC_PER_MIN*NS_PER_SEC

# read the time-step.dat file
dat_header = ''
dat_line_1 = ''
with open(dat, 'r') as infile:
  dat_header = infile.readline()
  dat_line_1 = infile.readline()

# parse nanoseconds per step
dat_line_1_list = dat_line_1.strip().split(',')
hour = float(dat_line_1_list[0])
minute = float(dat_line_1_list[1])
second = float(dat_line_1_list[2])
nanosecond = float(dat_line_1_list[3])
ns = ((hour*MIN_PER_HOUR+minute)*SEC_PER_MIN+second)*NS_PER_SEC+nanosecond

# calculate step count
steps = math.ceil(cnt*ns_per_rev/ns)

# Write out configuration files
with open(dst+'num-steps.dat',mode='w',newline='') as csvfile:
  csvwriter = csv.writer(csvfile)
  csvwriter.writerow(['steps'])
  csvwriter.writerow(['{:019d}'.format(steps)])
