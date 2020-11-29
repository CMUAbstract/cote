# Usage: python3 generate_3d_scatter_plots.py /path/to/logs/ /path/to/dst/
# Parameters:
#  /path/to/logs/: path to the directory containing simulation logs
#  /path/to/dst/: directory to write 3d-scatter-*.pdf
# Output:
#  pie-chart-*.pdf where * is the satellite ID

# import Python modules
import csv                      # read and write CSV files
import matplotlib.pyplot as plt # generate a pie chart
import os                       # list directory
import re                       # regular expressions
import sys                      # script arguments

# initialize script arguments
log = '' # log directory containing meas-sat-NNNNNNNNNN-*-{x|y|z}-km.csv files
dst = '' # destination directory for produced chart files

# parse script arguments
if len(sys.argv)==3:
  log = sys.argv[1]
  if log[-1] != '/':
    log += '/'
  dst = sys.argv[2]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: '\
   'python3 generate_3d_scatter_plots.py '\
   '/path/to/logs/ /path/to/dst/'\
  )
  exit()

# Collect all file names matching meas-sat-NNNNNNNNNN-[dev]-state.csv
logs = [f for f in os.listdir(log) \
 if re.match(r'meas-sat-[0-9]{10}-[xyz]-km.csv',f) \
]
logs.sort()

# Collect fine-grained statistics
sat_ids = []
sat_id_to_dict_of_xyz_lists = {}
for logfile in logs:
  with open(log+logfile,mode='r',newline='') as csv_i:
    # Get satellite ID string from file name
    sat_id = logfile[9:19]
    # Add satellite ID string to list of sat IDs
    if sat_id not in sat_ids:
      sat_ids.append(sat_id)
    # Add to dictionary (shouldn't be there yet, but check anyway)
    if sat_id not in sat_id_to_dict_of_xyz_lists:
      sat_id_to_dict_of_xyz_lists[sat_id] = {}
    # Get x, y, or z from file name
    xyz_id = logfile[20:21]
    # Add to dictionary (shouldn't be there yet, but check anyway)
    if xyz_id not in sat_id_to_dict_of_xyz_lists[sat_id]:
      sat_id_to_dict_of_xyz_lists[sat_id][xyz_id] = []
    # Initialize CSV reader
    csvreader = csv.reader(csv_i,delimiter=',')
    # There should always be a header
    head_row = next(csvreader)
    # Try to get a first row
    row = []
    try:
      row = next(csvreader)
    except StopIteration:
      # Nothing to do aside for catching the exception
      pass
    # Collect a list of coordinate values
    coords = []
    while row != []:
      coords.append(float(row[1]))
      row = []
      try:
        row = next(csvreader)
      except StopIteration:
        # Nothing to do aside for catching the exception
        pass
    sat_id_to_dict_of_xyz_lists[sat_id][xyz_id].extend(coords)

# Fill in any missing entries
for sat_id in sat_ids:
  for xyz_id in ['x','y','z']:
    if xyz_id not in sat_id_to_dict_of_xyz_lists[sat_id]:
      sat_id_to_dict_of_xyz_lists[sat_id][xyz_id] = []

# Write out many 3D scatter PDF files
for sat_id in sat_ids:
  ttl = 'SAT ID '+sat_id
  fig = plt.figure()
  ax  = plt.axes(title=ttl, projection='3d')
  ax.scatter(\
   sat_id_to_dict_of_xyz_lists[sat_id]['x'],\
   sat_id_to_dict_of_xyz_lists[sat_id]['y'],\
   sat_id_to_dict_of_xyz_lists[sat_id]['z'],\
   marker='o', s=(72.0/fig.dpi)**2\
  )
  ax.set_xlabel("x km")
  ax.set_ylabel("y km")
  ax.set_zlabel("z km")
  fig.savefig(dst+'3d-scatter-'+sat_id+'.pdf',bbox_inches='tight')
  plt.close(fig)
