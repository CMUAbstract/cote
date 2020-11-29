# Usage: python3 generate_constellation_state_pie_chart.py /path/to/num-steps.dat /path/to/logs/ device-name /path/to/dst/
# Parameters:
#  /path/to/num-steps.dat: config files used to specify simulation step count
#  /path/to/logs/: path to the directory containing simulation logs
#  device-name: e.g. adacs; replaces * in meas-sat-NNNNNNNNNN-*-state.csv
#  /path/to/dst/: directory to write pie-chart-*.pdf
# Output:
#  pie-chart-*.pdf where * is the supplied device-name

# import Python modules
import csv                      # read and write CSV files
import math                     # floor, ceil, sqrt
import matplotlib.pyplot as plt # generate a pie chart
import statistics               # mean and population standard deviation
import os                       # list directory
import re                       # regular expressions
import sys                      # script arguments

# initialize script arguments
nsd = '' # simulation step count file
log = '' # log directory containing meas-sat-NNNNNNNNNN-*-state.csv files
dev = '' # device name used to replace *
dst = '' # destination directory for produced chart file

# parse script arguments
if len(sys.argv)==5:
  nsd = sys.argv[1]
  log = sys.argv[2]
  if log[-1] != '/':
    log += '/'
  dev = sys.argv[3]
  dst = sys.argv[4]
  if dst[-1] != '/':
    dst += '/'
else:
  print(\
   'Usage: '\
   'python3 generate_constellation_state_pie_chart.py '\
   '/path/to/num-steps.dat /path/to/logs/ device-name /path/to/dst/'\
  )
  exit()

# read num-steps.dat file
nsd_header = ''
nsd_line_1 = ''
with open(nsd, 'r') as infile:
  nsd_header = infile.readline()
  nsd_line_1 = infile.readline()

# Get the max "tick" number
cnt = int(nsd_line_1)

# Collect all file names matching meas-sat-NNNNNNNNNN-[dev]-state.csv
logs = [f for f in os.listdir(log) \
 if re.match(r'meas-sat-[0-9]{10}-'+re.escape(dev)+r'-state.csv',f) \
]
logs.sort()

# Collect fine-grained statistics
dev_states = []
sat_ids    = []
dev_state_to_dict_of_sat_total_tick_cnts = {}
for logfile in logs:
  with open(log+logfile,mode='r',newline='') as csv_i:
    # Get satellite ID string from file name
    sat_id = logfile[9:19]
    # Add satellite ID string to list of sat IDs
    if sat_id not in sat_ids:
      sat_ids.append(sat_id)
    # Initialize CSV reader
    csvreader = csv.reader(csv_i,delimiter=',')
    # There should always be a header
    head_row = next(csvreader)
    # There should always be at least one line of data
    prev_row = next(csvreader)
    # Add the state to the list of states of it's not already there
    if prev_row[1] not in dev_states:
      dev_states.append(prev_row[1])
    # Need to add an empty dictionary of the state isn't there yet
    if prev_row[1] not in dev_state_to_dict_of_sat_total_tick_cnts:
      dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]] = {}
    # Sat ID shouldn't be in the dictionary, but safety check
    if sat_id not in dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]]:
      dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]][sat_id] = 0
    # Initialize curr_row so that it's in the correct scope
    curr_row = []
    try:
      curr_row = next(csvreader)
    except StopIteration:
      # The first and only record for this satellite, so use cnt for upper tick
      dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]][sat_id] += \
       cnt-int(prev_row[0])
    else:
      # The first of multiple records for this satellite, so use curr_row
      dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]][sat_id] += \
       int(curr_row[0])-int(prev_row[0])
    while curr_row != []:
      prev_row = curr_row
      # Add the state to the list of states of it's not already there
      if prev_row[1] not in dev_states:
        dev_states.append(prev_row[1])
      # Need to add an empty dictionary of the state isn't there yet
      if prev_row[1] not in dev_state_to_dict_of_sat_total_tick_cnts:
        dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]] = {}
      # Since state prev_row[1] can change, set to zero only of Sat ID not there
      if sat_id not in dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]]:
        dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]][sat_id] = 0
      # Set curr_row to [] before trying next
      curr_row = []
      try:
        curr_row = next(csvreader)
      except StopIteration:
        # End for this satellite, so use cnt for upper tick
        dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]][sat_id] += \
         cnt-int(prev_row[0])
      else:
        # Use curr_row
        dev_state_to_dict_of_sat_total_tick_cnts[prev_row[1]][sat_id] += \
         int(curr_row[0])-int(prev_row[0])

# Fill in any missing zeros and convert to percentages
for dev_state in dev_states:
  for sat_id in sat_ids:
    if sat_id not in dev_state_to_dict_of_sat_total_tick_cnts[dev_state]:
      dev_state_to_dict_of_sat_total_tick_cnts[dev_state][sat_id] = 0.0
    else:
      dev_state_to_dict_of_sat_total_tick_cnts[dev_state][sat_id] = 100.0* \
       float(dev_state_to_dict_of_sat_total_tick_cnts[dev_state][sat_id])/ \
       float(cnt)

# For each state, harvest avg, pdv, min, and max
dev_state_to_dict_of_stats = {}
for dev_state in dev_states:
  population = []
  for sat_id in sat_ids:
    population.append(\
     dev_state_to_dict_of_sat_total_tick_cnts[dev_state][sat_id]\
    )
  dev_state_to_dict_of_stats[dev_state] = {}
  dev_state_to_dict_of_stats[dev_state]['avg'] = statistics.mean(population)
  dev_state_to_dict_of_stats[dev_state]['pdv'] = statistics.pstdev(population)
  dev_state_to_dict_of_stats[dev_state]['min'] = min(population)
  dev_state_to_dict_of_stats[dev_state]['max'] = max(population)

# Write out CSV summary file
with open(dst+'pie-chart-'+dev+'.csv',mode='w',newline='') as csv_o:
  csvwriter = csv.writer(csv_o)
  csvwriter.writerow(['state','avg','pdv','min','max'])
  for dev_state in dev_states:
    csvwriter.writerow([\
     dev_state,\
     '{:.6f}'.format(dev_state_to_dict_of_stats[dev_state]['avg']),\
     '{:.6f}'.format(dev_state_to_dict_of_stats[dev_state]['pdv']),\
     '{:.6f}'.format(dev_state_to_dict_of_stats[dev_state]['min']),\
     '{:.6f}'.format(dev_state_to_dict_of_stats[dev_state]['max']) \
    ])

# Write out pie chart PDF file
labs = []
vals = []
for dev_state in dev_states:
  labs.append(dev_state)
  vals.append(dev_state_to_dict_of_stats[dev_state]['avg'])
ttl = dev.upper()
fig = plt.figure()
ax  = plt.axes(title=ttl, aspect='equal')
ax.pie(vals, labels=labs)
fig.savefig(dst+'pie-chart-'+dev+'.pdf',bbox_inches='tight')
plt.close(fig)
