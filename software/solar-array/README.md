# Solar Array Class

The `SolarArray` class manages simulation of a solar array. A solar array is
modeled as a monolithic solar cell, which is equivalent to a diode in parallel
with a current source. In this case, the diode is modeled as an ideal, offset
diode. The current source is modeled as an ideal source, but the current depends
on the incident irradiance and array efficiency so that the offset current
changes with the light level.

As a result, the open circuit voltage remains constant while the short circuit
current varies depending on the light level. Since the offset diode is ideal,
the maximum power point occurs at (Voc, Isc) where Isc must be calculated from
the provided solar array surface area, the provided solar array efficiency, and
the irradiance at the given time.

## Directory Contents

* [build](build/README.md): Compile and run the test program
* [include](include/SolarArray.hpp): Header files
* [source](source/SolarArray.cpp): Implementation files
* [test](test/test-solar-array.cpp): Test program
* [README.md](README.md): This document

## License

Written by Bradley Denby  
Other contributors: None

See the top-level LICENSE file for the license.
