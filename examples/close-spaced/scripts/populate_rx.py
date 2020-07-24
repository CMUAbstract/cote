#!/usr/bin/env python3
# Usage: python3 populate_rx.py /path/to/src/ ext max_gain_db center_frequency_hz bandwidth_hz /path/to/dst/
# Input:
#  /path/to/src/: directory where the devices are read
#  ext: only pay attention to files with this extension
#  max_gain_db: RX max gain in dB
#  center_frequency_hz: center frequency of the channel in Hz
#  bandwidth_hz: bandwidth of the channel in Hz
#  /path/to/dst/: directory where the files are written
# Output:
#  CSV file with [id,max_gain(dB),center_frequency(Hz),bandwidth(Hz)]

import csv
import os
import sys

src = ''
ext = ''
max_gain_db = 0.0
center_frequency_hz = 8.0e9
bandwidth_hz = 20.0e6
dst = ''

if len(sys.argv)==7:
  src = sys.argv[1]
  if src[-1] != '/':
    src += '/'
  ext = sys.argv[2]
  max_gain_db = max(0.0,float(sys.argv[3]))
  center_frequency_hz = max(0.0,float(sys.argv[4]))
  bandwidth_hz = max(0.0,min(float(sys.argv[5]),2.0*center_frequency_hz))
  dst = sys.argv[6]
  if dst[-1] != '/':
    dst += '/'
else:
  print("Usage: python3 populate_rx.py /path/to/src/ max_gain_db center_frequency_hz bandwidth_hz /path/to/dst/")
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
  with open(dst+'rx-'+str(id).zfill(10)+'.dat',mode='w',newline='') as outfile:
    csvwriter = csv.writer(outfile)
    csvwriter.writerow(['id','gain','center_frequency','bandwidth'])
    csvwriter.writerow([str(id).zfill(10),'{:+013.9f}'.format(max_gain_db),'{:022.9f}'.format(center_frequency_hz),'{:023.9f}'.format(bandwidth_hz)])
