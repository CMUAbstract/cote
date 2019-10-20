# Usage: python3 generate_sat_config_file.py /path/to/default/sat.tle /path/to/dst/
# Input:
#  /path/to/dst/: Directory to place output file
# Output:
#  Satellite TLE file. This script attempt to pull an up-to-date TLE from
#  the CelesTrak website; upon failure, a hard-coded fallback TLE is used.

import sys
import urllib.request

default_sat_tle = ''
dst = ''

if len(sys.argv)==3:
  default_sat_tle = sys.argv[1]
  dst = sys.argv[2]
  if dst[-1] != '/':
    dst += '/'
else:
  print("Usage: python3 generate_sat_config_file.py /path/to/default/sat.tle /path/to/dst/")
  exit()

# Load fallback TLE
tle = []
with open(default_sat_tle, 'r') as infile:
  tle = infile.readlines()
# Try to pull and parse from CelesTrak
with urllib.request.urlopen('http://celestrak.com/NORAD/elements/planet.txt') as response:
  html = response.read()
  lines = html.decode('utf-8').split('\r\n')
  i = 0
  while i<len(lines):
    if lines[i][:11] == 'FLOCK 3K-3 ':
      tle = [lines[i].rstrip().lower().replace('-',' ')+'\n', lines[i+1].rstrip()+'\n', lines[i+2].rstrip()+'\n']
    i += 1
# Write out result
filename = '-'.join(tle[0].split())+'.tle'
with open(dst+filename, 'w') as outfile:
  for line in tle:
    outfile.write(line)
