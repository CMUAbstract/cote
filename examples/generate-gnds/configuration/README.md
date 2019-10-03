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

To the extent possible under law, the author(s) have dedicated all copyright and
related and neighboring rights to this work to the public domain worldwide. This
work is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication with this
work. If not, see <https://creativecommons.org/publicdomain/zero/1.0/>.
