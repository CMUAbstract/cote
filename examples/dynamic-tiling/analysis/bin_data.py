# Usage: python3 bin_data.py /path/to/collated.csv /path/to/dest/
# Parameters:
#  /path/to/collated.csv: the collated CSV file
#  /path/to/dest/: where the binned file will be placed
# Output:
#  A CSV file of binned altitude, tile-count pairs

import csv
import math
import os
import sys

src = ''
dst = ''

if len(sys.argv)==3:
  src = sys.argv[1]
  dst = sys.argv[2]
  if dst[-1] != '/':
    dst += '/'
  with open(dst+'binned.csv',mode='w',newline='') as csv_o:
    csvwriter = csv.writer(csv_o)
    csvwriter.writerow(['alt-km','tile-count','alt-stdev-km','tile-count-stdev'])
else:
  print("Usage: python3 bin_data.py /path/to/collated.csv /path/to/dest/")
  exit()

# Find the minimum and maximum altitudes
min_alt = 100000.0
max_alt = -1.0
with open(src,newline='') as csv_i:
  csvreader = csv.reader(csv_i,delimiter=',')
  header = next(csvreader)
  for row in csvreader:
    alt = float(row[0])
    if alt < min_alt:
      min_alt = alt
    if alt > max_alt:
      max_alt = alt

# Setup bins
BIN = 5.0
min_bin = math.floor(min_alt/BIN)*BIN
max_bin = math.floor(max_alt/BIN)*BIN

keys = []
altitude_sums = {}
tile_cnt_sums = {}
counts = {}
with open(src,newline='') as csv_i:
  csvreader = csv.reader(csv_i,delimiter=',')
  header = next(csvreader)
  for row in csvreader:
    altitude = float(row[0])
    tile_cnt = float(row[1])
    alt_bin = int(math.floor(altitude/BIN)*BIN)
    if alt_bin not in keys:
      keys.append(alt_bin)
    if alt_bin not in altitude_sums:
      altitude_sums[alt_bin] = 0.0
    if alt_bin not in tile_cnt_sums:
      tile_cnt_sums[alt_bin] = 0.0
    if alt_bin not in counts:
      counts[alt_bin] = 0
    altitude_sums[alt_bin] += altitude
    tile_cnt_sums[alt_bin] += tile_cnt
    counts[alt_bin] += 1

keys.sort()
with open(dst+'binned.csv',mode='a',newline='') as csv_o:
  csvwriter = csv.writer(csv_o)
  for key in keys:
    csvwriter.writerow(['{:05.6f}'.format(altitude_sums[key]/counts[key]),'{:06.6f}'.format(tile_cnt_sums[key]/counts[key])])
