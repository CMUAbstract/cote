# Usage: python3 generate_tx_gnd_files.py /path/to/src/ /path/to/seed.dat /path/to/dst/
# Parameters:
#  /path/to/src/: directory containing *.gnd files
#  /path/to/seed.dat: tx-gnd.dat seed file with ignored leading ID field
#  /path/to/dst/: directory to write .dat files with ground station IDs added
# Output:
#  .dat files with populated ground station IDs

# import Python modules
import os  # accessing directory contents
import sys # accessing script arguments

# initialize script arguments
src = '' # source directory containing *.gnd files
dat = '' # seed DAT file
dst = '' # destination directory for produced configuration files

# parse script arguments
if len(sys.argv)==4:
  src = sys.argv[1]
  if src[-1] != '/':
    src += '/'
  dat = sys.argv[2]
  dst = sys.argv[3]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: '\
   'python3 generate_tx_gnd_files.py '\
   '/path/to/src/ /path/to/seed.dat /path/to/dst/'\
  )
  exit()

# collect the *.gnd files and parse ID
gnd_ids = []
for f in os.listdir(src):
  if f.endswith('.gnd'):
    with open(src+f,'r') as infile:
      header = infile.readline()
      gnd_ids.append(int(infile.readline()[0:10]))

# read the seed DAT file
dat_header = ''
dat_line_1 = ''
with open(dat, 'r') as infile:
  dat_header = infile.readline()
  dat_line_1 = ','.join(infile.readline().split(',')[1:])

# write out .dat files with IDs
for gnd_id in gnd_ids:
  # ID string
  id_str = str(gnd_id).zfill(10)
  # generate file name
  filename = 'tx-gnd-'+id_str+'.dat'
  # write seed data plus ID
  with open(dst+filename, 'w') as outfile:
    outfile.write(dat_header)
    outfile.write(id_str+','+dat_line_1.rstrip()+'\n')
