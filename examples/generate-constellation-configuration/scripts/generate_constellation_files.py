# Usage: python3 generate_constellation_files.py pix_size pix_count focal_len n /path/to/config/ /path/to/logs/ /path/to/dst/
# Input:
#  /path/to/sat.tle: satellite TLE file
#  pix_size: physical size length of pixel sensor in micrometers
#  pix_count: number of pixels (image width) parallel to the velocity vector
#  focal_len: distance between camera pinhole and image sensor in centimeters
#  n: number of satellites in the generated constellation
#  /path/to/config/: directory containing configuration files
#  /path/to/logs/: directory containing log files
#  /path/to/dst/: destination directory for constellation configuration files
# Output:
#  close-spaced.dat (all satellites over a single ground track frame),
#  frame-spaced.dat (one satellite per ground track frame), orbit-spaced.dat
#  (satellites are evenly spaced throughout the orbit)

import csv
import math
import os
import sys

pix_size  = 0.0
pix_count = 0.0
focal_len = 0.0
n         = 0.0
cfg       = ''
log       = ''
dst       = ''

if len(sys.argv)==8:
  pix_size  = float(sys.argv[1])*1.0e-9
  pix_count = float(sys.argv[2])
  focal_len = float(sys.argv[3])*1.0e-5
  n         = float(sys.argv[4])
  cfg = sys.argv[5]
  if cfg[-1] != '/':
    cfg += '/'
  log = sys.argv[6]
  if log[-1] != '/':
    log += '/'
  dst = sys.argv[7]
  if dst[-1] != '/':
    dst += '/'
else:
  print('Usage: python3 generate_constellation_files.py pix_size_um pix_count focal_len_cm n /path/to/config/ /path/to/logs/ /path/to/dst/')
  exit()

# Read time step file
h_step = 0
m_step = 0
s_step = 0
n_step = 0
with open(cfg+'time-step.dat',newline='') as csvfile:
  csvreader = csv.reader(csvfile)
  header = next(csvfile)
  t_step = next(csvfile).split(',')
  h_step = int(t_step[0])
  m_step = int(t_step[1])
  s_step = int(t_step[2])
  n_step = int(t_step[3])
MIN_PER_HOUR = 60.0
SEC_PER_MIN = 60.0
NS_PER_SEC = 1000000000.0
step_s = float(n_step)/NS_PER_SEC+float(s_step)+(float(m_step)+float(h_step)*MIN_PER_HOUR)*SEC_PER_MIN
# Determine minimum ground track frame distance in km
gtfd = float('inf')
with open(log+'meas-hae-km.csv',newline='') as csvfile:
  csvreader = csv.reader(csvfile)
  header = next(csvfile)
  for row in csvfile:
    local_gtfd = pix_count*((pix_size*float(row.split(',')[1]))/focal_len)
    if local_gtfd < gtfd:
      gtfd = local_gtfd
# Collect ECIs
ecis = []
with open(log+'meas-eci-x-km.csv',newline='') as csvfile:
  csvreader = csv.reader(csvfile)
  header = next(csvfile)
  for row in csvfile:
    ecis.append([float(row.split(',')[1])])
with open(log+'meas-eci-y-km.csv',newline='') as csvfile:
  csvreader = csv.reader(csvfile)
  header = next(csvfile)
  i = 0
  for row in csvfile:
    ecis[i].append(float(row.split(',')[1]))
    i+=1
with open(log+'meas-eci-z-km.csv',newline='') as csvfile:
  csvreader = csv.reader(csvfile)
  header = next(csvfile)
  i = 0
  for row in csvfile:
    ecis[i].append(float(row.split(',')[1]))
    i+=1
# Determine average speed in km/sec
acc_speed = 0.0
acc_count = 0.0
for i in range(0,len(ecis)-1):
  acc_speed += math.sqrt(pow(ecis[i+1][0]-ecis[i][0],2.0)+pow(ecis[i+1][1]-ecis[i][1],2.0)+pow(ecis[i+1][2]-ecis[i][2],2.0))/step_s
  acc_count += 1.0
avg_speed = acc_speed/acc_count
# Generate close-spaced configuration file
cs_h = 0
cs_m = 0
cs_s = 0
cs_n = 0
cs_step_s = (gtfd/avg_speed)/n
cs_step_s = max(0.005,cs_step_s) # minimum separation of 0.005 seconds
SEC_PER_HOUR = SEC_PER_MIN*MIN_PER_HOUR
while cs_step_s>=SEC_PER_HOUR:
  cs_h += 1
  cs_step_s -= SEC_PER_HOUR
while cs_step_s>=SEC_PER_MIN:
  cs_m += 1
  cs_step_s -= SEC_PER_MIN
while cs_step_s>=1.0:
  cs_s += 1
  cs_step_s -= 1.0
cs_n = int(round(cs_step_s*NS_PER_SEC))
with open(dst+'close-spaced.dat',mode='w',newline='') as csvfile:
  csvwriter = csv.writer(csvfile)
  csvwriter.writerow(['count','hour','minute','second','nanosecond'])
  csvwriter.writerow(['{:05d}'.format(int(n)),'{:02d}'.format(cs_h),'{:02d}'.format(cs_m),'{:02d}'.format(cs_s),'{:09}'.format(cs_n)])
# Generate frame-spaced configuration file
fs_h = 0
fs_m = 0
fs_s = 0
fs_n = 0
fs_step_s = (gtfd/avg_speed)
fs_step_s = max(0.005,fs_step_s) # minimum separation of 0.005 seconds
while fs_step_s>=SEC_PER_HOUR:
  fs_h += 1
  fs_step_s -= SEC_PER_HOUR
while fs_step_s>=SEC_PER_MIN:
  fs_m += 1
  fs_step_s -= SEC_PER_MIN
while fs_step_s>=1.0:
  fs_s += 1
  fs_step_s -= 1.0
fs_n = int(round(fs_step_s*NS_PER_SEC))
with open(dst+'frame-spaced.dat',mode='w',newline='') as csvfile:
  csvwriter = csv.writer(csvfile)
  csvwriter.writerow(['count','hour','minute','second','nanosecond'])
  csvwriter.writerow(['{:05d}'.format(int(n)),'{:02d}'.format(fs_h),'{:02d}'.format(fs_m),'{:02d}'.format(fs_s),'{:09}'.format(fs_n)])
# Read satellite revolutions per day from TLE file
config_files = os.listdir(cfg)
config_files.sort()
config_files = [e for e in config_files if e[-4:]=='.tle']
rev_per_day = 0.0
with open(cfg+config_files[0], mode='r') as infile:
  lines = infile.readlines()
  i = 0
  while i<len(lines) and lines[i][:2]!='2 ':
    i += 1
  if lines[i][:2]=='2 ':
    rev_per_day = float(lines[i][52:63])
  else:
    print('Incorrect TLE file format, exiting')
    exit()
# Generate orbit-spaced configuration file
HOUR_PER_DAY = 24.0
os_h = 0
os_m = 0
os_s = 0
os_n = 0
os_step_s = ((HOUR_PER_DAY*MIN_PER_HOUR*SEC_PER_MIN)/rev_per_day)/n
os_step_s = max(0.005,os_step_s) # minimum separation of 0.005 seconds
while os_step_s>=SEC_PER_HOUR:
  os_h += 1
  os_step_s -= SEC_PER_HOUR
while os_step_s>=SEC_PER_MIN:
  os_m += 1
  os_step_s -= SEC_PER_MIN
while os_step_s>=1.0:
  os_s += 1
  os_step_s -= 1.0
os_n = int(round(os_step_s*NS_PER_SEC))
with open(dst+'orbit-spaced.dat',mode='w',newline='') as csvfile:
  csvwriter = csv.writer(csvfile)
  csvwriter.writerow(['count','hour','minute','second','nanosecond'])
  csvwriter.writerow(['{:05d}'.format(int(n)),'{:02d}'.format(os_h),'{:02d}'.format(os_m),'{:02d}'.format(os_s),'{:09}'.format(os_n)])
