#!/usr/bin/env python3
# Usage: python3 plot_ground_tracks.py /path/to/logs/ /path/to/plot.ext
# Input:
#  /path/to/logs/: contains ground track logs
#  /path/to/plot.ext: specifies name and location of output file
# Output:
#  PDF file plotting ground tracks

import cartopy.crs as ccrs
import csv
import math
import matplotlib.pyplot as plt
import os
import sys

LON = 0
LAT = 1

src = ''
dst = ''

if len(sys.argv)==3:
  src = sys.argv[1]
  if src[-1] != '/':
    src += '/'
  dst = sys.argv[2]
else:
  print('Usage: python3 plot_ground_tracks.py /path/to/logs/ /path/to/plot.ext')
  exit()

# Read in latitudes and longitudes
all_files = os.listdir(src)
logs = []
for file in all_files:
  if file[-3:]=='csv':
    logs.append(file)
logs.sort()

ground_tracks = {}
for log in logs:
  id = int(log.split('-')[2])
  if id not in ground_tracks:
    ground_tracks[id] = [[],[]]
  if log.split('-')[3]=='lon':
    # Read entire log file and append to ground_tracks[id][LON]
    with open(src+log, newline='') as csvfile:
      csvreader = csv.reader(csvfile)
      next(csvreader) # skip header
      for row in csvreader:
        ground_tracks[id][LON].append(float(row[1]))
  elif log.split('-')[3]=='lat':
    # Read entire log file and append to ground_tracks[id][LAT]
    with open(src+log, newline='') as csvfile:
      csvreader = csv.reader(csvfile)
      next(csvreader) # skip header
      for row in csvreader:
        ground_tracks[id][LAT].append(float(row[1]))

# Generate colors
step = math.floor(255.0/(float(len(ground_tracks)+1)/3.0))
curr_color = [0,0,0]
colors = []
for i in range(0,len(ground_tracks)):
  curr_color[i%3] = (curr_color[i%3]+step)%256
  colors.append('#'+hex(curr_color[0])[2:].zfill(2)+hex(curr_color[1])[2:].zfill(2)+hex(curr_color[2])[2:].zfill(2))

# Plot the results
ax = plt.axes(projection=ccrs.PlateCarree())
ax.stock_img()
color_index = 0
for id in ground_tracks:
  plt.plot(ground_tracks[id][LON],ground_tracks[id][LAT],label=str(id),linestyle='-',color=colors[color_index],linewidth=1, marker='None',transform=ccrs.Geodetic())
  color_index += 1
plt.legend(loc='lower center',bbox_to_anchor=(0.5, -0.3),ncol=int(len(ground_tracks)/10),prop={'size':2})
plt.savefig(dst)
