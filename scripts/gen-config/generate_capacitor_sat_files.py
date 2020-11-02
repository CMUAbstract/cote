# Usage: python3 generate_capacitor_sat_files.py count /path/to/seed.tle /path/to/seed.dat /path/to/dst/
# Parameters:
#  count: number of satellites in the constellation
#  /path/to/seed.tle: 3-line TLE (header, line one, line two) with seed NORAD ID
#  /path/to/seed.dat: capacitor.dat seed file without ID field
#  /path/to/dst/: directory to write .dat files with satellite IDs added
# Output:
#  .dat files with satellite IDs added

# import Python modules
import sys # accessing script arguments

# initialize script arguments
cnt = 0  # constellation count
tle = '' # seed TLE file
dat = '' # seed DAT file
dst = '' # destination directory for produced configuration files

# parse script arguments
if len(sys.argv)==5:
  cnt = int(sys.argv[1])
  tle = sys.argv[2]
  dat = sys.argv[3]
  dst = sys.argv[4]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: '\
   'python3 generate_capacitor_sat_files.py '\
   'count /path/to/seed.tle /path/to/seed.dat /path/to/dst/'\
  )
  exit()

# read the seed TLE file
tle_header = ''
tle_line_1 = ''
tle_line_2 = ''
with open(tle, 'r') as infile:
  tle_header = infile.readline()
  tle_line_1 = infile.readline()
  tle_line_2 = infile.readline()

# read the seed DAT file
dat_header = ''
dat_line_1 = ''
with open(dat, 'r') as infile:
  dat_header = infile.readline()
  dat_line_1 = infile.readline()

# write out .dat files with IDs
norad_id = int(tle_line_1[2:7])
for i in range(0,cnt):
  # calculate ID
  sat_id = norad_id*10000+i
  id_str = str(sat_id).zfill(10)
  # generate file name
  filename = 'capacitor-sat-'+id_str+'.dat'
  # write seed data plus ID
  with open(dst+filename, 'w') as outfile:
    outfile.write(dat_header.rstrip()+',id\n')
    outfile.write(dat_line_1.rstrip()+','+id_str+'\n')
