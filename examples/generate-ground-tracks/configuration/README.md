# Configuration Files

Place configuration files in this directory. Examples are provided.

## Directory Contents

* [date-time.dat](date-time.dat): Specifies program start date and time
* [num-steps.dat](num-steps.dat): Number of program time steps
* [time-step.dat](time-step.dat): Time interval between program time steps
* [*.tle](README.md): Satellite TLE files specifying orbit properties
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

## Two-Line Element Set File Format

See `cote/references/hoots1980models-original.pdf` page 91 (PDF page 96). TLEs
can be obtained from `http://celestrak.com/NORAD/elements/`.

## License

Written by Bradley Denby  
Other contributors: None

See the top-level LICENSE file for the license.
