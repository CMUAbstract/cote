# Usage: python3 generate_polar_norad_ids.py /path/to/dst/
# Input:
#  /path/to/dst/: destination directory for the NORAD IDs
# Output:
#  norad-ids.txt

import sys
import urllib.request

class SatDat:
  def __init__(self,norad_id,perigee,apogee):
    self.norad_id = norad_id
    self.perigee  = perigee
    self.apogee   = apogee

dst = ''

if len(sys.argv)==2:
  dst = sys.argv[1]
  if dst[-1] != '/':
    dst += '/'
else:
  print('Usage: python3 generate_polar_norad_ids.py /path/to/dst/')
  exit()

url = 'https://celestrak.com/pub/satcat.txt'
response = urllib.request.urlopen(url)
data = response.read()
text = data.decode('utf-8')
lines = text.splitlines()
satdats = []
all_perigees = []
for line in lines:
  if line[75:85].isspace() and not line[96:101].isspace() and \
     float(line[87:94])<225.0 and 80.0<=float(line[96:101])<=100.0:
    norad_id = line[13:18]
    perigee  = float(line[111:117])
    apogee   = float(line[103:109])
    satdats.append(SatDat(norad_id,perigee,apogee))
    all_perigees.append(perigee)

unique_perigees = set(all_perigees)
unique_perigees_list = list(unique_perigees)
unique_perigees_list.sort()
chosen_satdats = []
for perigee in unique_perigees_list:
  chosen_satdat = None
  for satdat in satdats:
    if satdat.perigee == perigee:
      if chosen_satdat == None:
        chosen_satdat = satdat
      elif chosen_satdat.apogee < satdat.apogee:
        chosen_sat = satdat
  chosen_satdats.append(chosen_satdat)

with open(dst+'norad-ids.txt','w') as outfile:
  for e in chosen_satdats:
    outfile.write(e.norad_id+'\n')
