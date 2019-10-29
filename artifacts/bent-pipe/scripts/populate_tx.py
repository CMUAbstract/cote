#!/usr/bin/env python3
# Usage: python3 populate_tx.py /path/to/src/ ext power_w line_loss_db max_gain_db center_frequency_hz bandwidth_hz /path/to/dst/
# Input:
#  /path/to/src/: directory where the devices are read
#  ext: only pay attention to files with this extension
#  power_w: TX power in watts
#  line_loss_db: TX line loss in dB
#  max_gain_db: TX max gain in dB
#  center_frequency_hz: center frequency of the channel in Hz
#  bandwidth_hz: bandwidth of the channel in Hz
#  /path/to/dst/: directory where the files are written
# Output:
#  CSV file with [id,power(W),line_loss(dB),max_gain(dB),center_frequency(Hz),bandwidth(Hz)]

import csv
import os
import sys

src = ''
ext = ''
power_w = 0.0
line_loss_db = 0.0
max_gain_db = 0.0
center_frequency_hz = 8.0e9
bandwidth_hz = 20.0e6
dst = ''

if len(sys.argv)==9:
  src = sys.argv[1]
  if src[-1] != '/':
    src += '/'
  ext = sys.argv[2]
  power_w = max(0.0,float(sys.argv[3]))
  line_loss_db = min(float(sys.argv[4]),0.0)
  max_gain_db = max(0.0,float(sys.argv[5]))
  center_frequency_hz = max(0.0,float(sys.argv[6]))
  bandwidth_hz = max(0.0,min(float(sys.argv[7]),2.0*center_frequency_hz))
  dst = sys.argv[8]
  if dst[-1] != '/':
    dst += '/'
else:
  print("Usage: python3 populate_tx.py /path/to/src/ power_w line_loss_db max_gain_db center_frequency_hz bandwidth_hz /path/to/dst/")
  exit()

devices = os.listdir(src)
temp = []
for device in devices:
  if device.endswith(ext):
    temp.append(device)
devices = temp
for device in devices:
  id = 0
  if device.endswith('.tle'):
    with open(src+device,'r') as infile:
      header = infile.readline()
      id = 1000*int(infile.readline()[2:7])
  elif device.endswith('.dat'):
    with open(src+device,'r') as infile:
      header = infile.readline()
      id = int(infile.readline().split(',')[0])
  with open(dst+'tx-'+str(id).zfill(10)+'.dat',mode='w',newline='') as outfile:
    csvwriter = csv.writer(outfile)
    csvwriter.writerow(['id','power','line_loss','gain','center_frequency','bandwidth'])
    csvwriter.writerow([str(id).zfill(10),'{:+016.9f}'.format(power_w),'{:+013.9f}'.format(line_loss_db),'{:+013.9f}'.format(max_gain_db),'{:022.9f}'.format(center_frequency_hz),'{:023.9f}'.format(bandwidth_hz)])
