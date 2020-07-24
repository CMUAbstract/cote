# Configuration Files

Place configuration files in this directory. Examples are provided.

## Directory Contents

* [date-time.dat](date-time.dat): Specifies program start date and time
* [ground-station-*.dat](ground-station-0000040079.dat): Ground station files
* [num-steps.dat](num-steps.dat): Number of program time steps
* [time-step.dat](time-step.dat): Time interval between program time steps
* [README.md](README.md): This document

## Date and Time Configuration File Format

```
year,month,day,hour,minute,second,nanosecond
YYYY,MM,DD,HH,MM,SS,NNNNNNNNN
```

## Time Step File Format

```
hour,minute,second,nanosecond
HH,MM,SS,NNNNNNNNN
```

## Number of Time Steps File Format

```
steps
NNNNNNNNNNNNNNNNNNN
```

## Ground Station Count Configuration File Format

```
count
NNNNNNNNNN
```

## Ground Station File Format

Here, `N` indicates a decimal digit (0-9) and `S` indicates a sign symbol (+/-).

```
id,lat,lon,hae
NNNNNNNNNN,SNN.NNNNNNNNN,SNNN.NNNNNNNNN,SNN.NNNNNNNNN
```

## License

Written by Bradley Denby  
Other contributors: None

See the top-level LICENSE file for the license.
