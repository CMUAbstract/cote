# Example Programs

Example programs using the `cote` software.

## Directory Contents

* [bent-pipe](bent-pipe/README.md): Simulates a constellation of satellites with
  camera sensors that each individually collect observations at the ground track
  frame rate and attempt to downlink all data to ground stations. The included
  scripts generate logs for a constellation in a polar orbit with polar ground
  stations. The scripts sweep through a collection of constellation populations
  and collate the corresponding downlink deficit.
* [close-spaced](close-spaced/README.md): Simulates a constellation of
  satellites with camera sensors that collectively capture tiled observations at
  the ground track frame rate and attempt to downlink all data to ground
  stations. The included scripts generate logs for a close-spaced constellation
  in a polar orbit with polar ground stations. The scripts sweep through a
  collection of constellation populations and collate the corresponding downlink
  deficit.
* [dynamic-tiling](dynamic-tiling/README.md): Simulates a satellite that
  dynamically changes the tile size as altitude changes so that target features
  are scaled to the proper size that maximizes accuracy when analyzed by a CNN
* [frame-spaced](frame-spaced/README.md): Simulates a constellation of
  satellites with camera sensors that collectively capture observations at the
  ground track frame rate and attempt to downlink all data to ground stations.
  The included scripts generate logs for a frame-spaced constellation in a polar
  orbit with polar ground stations. The scripts sweep through a collection of
  constellation populations and collate the corresponding downlink deficit.
* [generate-coms](generate-coms/README.md): Generates communication time logs
  and bitrates for a specified constellation and a ground station network.
* [generate-constellation-configuration](generate-constellation-configuration/README.md):
  Generates constellation configuration files from input parameters
* [generate-constellation-tracks](generate-constellation-tracks/README.md):
  Generates latitude and longitude ground tracks for a satellite constellation
* [generate-deadlines](generate-deadlines/README.md): Generates the ground track
  frame period over the course of a satellite's orbit, which varies with
  altitude
* [generate-ecis](generate-ecis/README.md): Generates ECI coordinates for a
  satellite orbit
* [generate-gnds](generate-gnds/README.md): Generates LLH coordinates for ground
  stations
* [generate-ground-tracks](generate-ground-tracks/README.md): Generates
  satellite ground tracks
* [generate-irradiance-trace](generate-irradiance-trace/README.md): Generates an
  irradiance trace for a satellite as it moves through the specified orbit
* [generate-sat-azel](generate-sat-azel/README.md): Generates the AzEl
  coordinates of a satellite from the perspective of the specified ground
  station
* [generate-sun-azel](generate-sun-azel/README.md): Generates the AzEl
  coordinates of the Sun from the perspective of the specified ground station
* [generate-voltage-trace](generate-voltage-trace/README.md): A full-system
  simulation of a satellite as it moves through its orbit, including energy
  harvesting, energy storage, and energy consumption for attitude control, data
  collection, and computing.
* [README.md](README.md): This document

## License

Written by Bradley Denby  
Other contributors: None

See the top-level LICENSE file for the license.
