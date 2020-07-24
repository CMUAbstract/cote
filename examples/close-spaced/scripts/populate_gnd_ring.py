#!/usr/bin/env python3
# Usage: python3 populate_gnd_ring.py lat lon n id0 /path/to/dst/
# Input:
#  lat: latitude of all ground stations to be generated
#  lon: longitude of the first ground station to be generated (id0)
#  n: number of stations to be evenly spaced eastward around lat starting at lon
#  id0: id of the first ground station, to be incremented by 1 for each n
#  /path/to/dst/: Directory to place output files
# Output:
#  CSV file with [id,lat(deg),lon(deg),hae(km)]
# Todo:
#  Automatically determine height above ellipsoid (HAE) given (lat,lon)

import csv
import sys

lat = 0.0
lon = 0.0
n   = 1
id0 = 0
dst = ''

if len(sys.argv)==6:
  lat = max(-90.0,min(float(sys.argv[1]),90.0))
  lon = float(sys.argv[2])%360.0
  n = int(sys.argv[3])
  id0 = int(sys.argv[4])
  dst = sys.argv[5]
  if dst[-1] != '/':
    dst += '/'
  for i in range(0,n):
    with open(dst+'gnd-'+str(id0+i).zfill(10)+'.dat',mode='w',newline='') as outfile:
      csvwriter = csv.writer(outfile)
      csvwriter.writerow(['id','lat','lon','hae'])
else:
  print("Usage: python3 populate_gnd_ring.py lat lon n id0 /path/to/dst/")
  exit()

lons = [lon]
dlon = 360.0/float(n)
haes = [0.0]
for i in range(1,n):
  lons.append((lon+i*dlon)%360.0)
  haes.append(0.0)
for i in range(0,len(lons)):
  lons[i] = ((lons[i]+180.0)%360.0)-180.0
  with open(dst+'gnd-'+str(id0+i).zfill(10)+'.dat',mode='a',newline='') as outfile:
    csvwriter = csv.writer(outfile)
    csvwriter.writerow([str(id0+i).zfill(10),'{:+013.9f}'.format(lat),'{:+014.9f}'.format(lons[i]),'{:+013.9f}'.format(haes[i])])
