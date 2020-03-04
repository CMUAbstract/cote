# Usage: python3 parse_and_collate_logs.py /path/to/logs/ /path/to/dest/
# Parameters:
#  /path/to/logs/: contains directories, each containing log files
#  /path/to/dest/: where the parsed and collated file will be placed
# Output:
#  A CSV file of altitude, tile-count pairs

import csv
import os
import sys

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
    csvwriter.writerow(['altitude-km','tile-count'])
else:
  print("Usage: python3 parse_and_collate_logs.py /path/to/logs/ /path/to/dest/")
  exit()

src_contents = os.listdir(src)
src_contents.sort()
for e in src_contents:
  if os.path.isdir(src+e):
    logdir = src+e+'/'
    idstr  = e[3:]
    with open(logdir+'meas-sat-'+idstr+'-alt.csv',newline='') as csv_alt,\
         open(logdir+'meas-sat-'+idstr+'-tile-count.csv',newline='') as csv_tlc:
      csvreader_alt = csv.reader(csv_alt,delimiter=',')
      csvreader_tlc = csv.reader(csv_tlc,delimiter=',')
      header_alt = next(csvreader_alt)
      header_tlc = next(csvreader_tlc)
      row_alt = next(csvreader_alt)
      row_tlc = next(csvreader_tlc)
      while row_alt and row_tlc:
        altitude = float(row_alt[1])
        tile_cnt = float(row_tlc[1])
        # Write out
        with open(dst+'collated.csv',mode='a',newline='') as csv_o:
          csvwriter = csv.writer(csv_o)
          csvwriter.writerow(['{:05.6f}'.format(altitude),'{:06.6f}'.format(tile_cnt)])
        # Loop update
        try:
          row_alt = next(csvreader_alt)
          row_tlc = next(csvreader_tlc)
        except StopIteration:
          break
