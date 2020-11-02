# Usage: python3 generate_gnd_files.py count seed /path/to/gnds.csv /path/to/dst/
# Parameters:
#  count: number of ground stations to generate
#  seed: provide a positive integer to select a repeatable, random subset of
#   gnds.csv; provide 0 to select randomly from gnds.csv
#  /path/to/gnds.csv: format each row as name,latitude,longitude,hae,id
#  /path/to/dst/: directory to write .gnd files
# Output:
#  .gnd files with ID, latitude, longitude, and height above ellipsoid (HAE)

# import Python modules
import csv    # reading and writing CSV files
import random # random selection
import re     # regex for file name generation
import sys    # accessing script arguments

# initialize script arguments
cnt  = 0  # ground station count
seed = 0  # seed to be used if repeatably, randomly selecting subset of gnds
src  = '' # source CSV of ground station data
dst  = '' # destination directory for produced configuration files

# parse script arguments
if len(sys.argv)==5:
  cnt  = int(sys.argv[1])
  seed = int(sys.argv[2])
  src  = sys.argv[3]
  dst  = sys.argv[4]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: '\
   'python3 generate_gnd_files.py '\
   'count seed /path/to/gnds.csv /path/to/dst/'\
  )
  exit()

# Parse input data
id2filename = {}
id2filefill = {}
with open(src,mode='r',newline='') as infile:
  csvreader = csv.reader(infile)
  header = next(csvreader)
  for row in csvreader:
    gnd_id = int(row[4])
    id2filename[gnd_id] =\
     'gnd-'+str(gnd_id).zfill(10)+'-'+\
     re.sub('[^0-9a-z]+', '-', row[0].strip().lower()).strip('-')+'.gnd'
    id2filefill[gnd_id] =\
     [gnd_id,float(row[1]),float(row[2]),float(row[3])]

# Select data for output
if seed>0:
  random.seed(seed)
out_ids = random.sample(list(id2filename.keys()),cnt)

# Write .gnd files
for out_id in out_ids:
  with open(dst+id2filename[out_id],mode='w',newline='') as outfile:
    csvwriter = csv.writer(outfile)
    csvwriter.writerow(['id','lat','lon','hae-km'])
    csvwriter.writerow([\
     str(id2filefill[out_id][0]).zfill(10),
     '{:+013.9f}'.format(id2filefill[out_id][1]),
     '{:+014.9f}'.format(id2filefill[out_id][2]),
     '{:+013.9f}'.format(id2filefill[out_id][3])\
    ])
