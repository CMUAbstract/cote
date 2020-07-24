// constants.hpp
// Constants header file
//
// Written by Bradley Denby
// Other contributors: None
//
// See the top-level LICENSE file for the license.

// Standard library
#include <cstdint>  // uint8_t, uint32_t

// cote library
// None

namespace cote { namespace cnst {
  // Contemporary precision constants
  const double QUARTER_PI         = 0.78539816339744830962; // pi/4
  const double HALF_PI            = 1.57079632679489661923; // pi/2
  const double PI                 = 3.14159265358979323846; // pi (ref. value)
  const double TWO_PI             = 6.28318530717958647692; // 2*pi
  const double INV_QUARTER_PI     = 1.27323954473516268615; // 4/pi
  const double INV_HALF_PI        = 0.63661977236758134308; // 2/pi
  const double INV_PI             = 0.31830988618379067154; // 1/pi
  const double INV_TWO_PI         = 0.15915494309189533577; // 1/(2*pi)
  const double BOLTZMANN_CONSTANT = 1.380649e-23;           // J/K

  // Unit conversions
  const uint8_t  MONTH_PER_YEAR = 12;                     // month per year
  const uint8_t  HOUR_PER_DAY   = 24;                     // hours per day
  const uint8_t  MIN_PER_HOUR   = 60;                     // minutes per hour
  const uint8_t  SEC_PER_MIN    = 60;                     // seconds per minute
  const uint32_t NS_PER_SEC     = 1000000000;             // nanoseconds per sec
  const uint16_t MIN_PER_DAY    = 1440;                   // minutes per day
  const uint32_t SEC_PER_DAY    = 86400;                  // seconds per day
  const double   RAD_PER_DEG    = 0.01745329251994329577; // radians per degree
  const double   RAD_PER_REV    = 6.28318530717958647692; // rads per revolution
  const double   M_PER_KM       = 1000.0;                 // meters per km
  const double   KM_PER_AU      = 149597870.700;          // kilometers per AU

  // Space Track Report No. 3
  // See cote/references/hoots1980models.pdf
  // STR3_KE: XKE, Driver line 14/17
  // STR3_DU_PER_ER: AE, Driver line 14/17
  // STR3_J2: XJ2, Driver line 13/16
  // STR3_J3: XJ3, Driver line 13/16
  // STR3_J4: XJ4, Driver line 14/17
  // STR3_K2: CK2, Driver line 24
  // STR3_K4: CK4, Driver line 25
  // STR3_RAD_PER_DEG: DE2RA, Driver line 13/14
  // STR3_PI: PI, Driver line 13/15
  // STR3_HALF_PI: PIO2, Driver line 13/15
  // STR3_Q0: Q0, Driver line 13/15
  // STR3_S0: S0, Driver line 13/15
  // STR3_TWO_THIRDS: TOTHRD, Driver line 13/15
  // STR3_TWO_PI: TWOPI, Driver line 13/16
  // STR3_THREE_HALVES_PI: X3PIO2, Driver line 13/16
  // STR3_KM_PER_ER: XKMPER, Driver line 14/17
  // STR3_MIN_PER_DAY: XMNPDA, Driver line 14/16
  // STR3_A30: -1.0*STR3_J3*STR3_DU_PER_ER^3, SGP4 line 61
  // STR3_RAD_PER_REV: STR3_TWO_PI
  const float STR3_KE =  0.743669161e-1f; // sqrt(GM) (Earth radii / minute)^1.5
  const float STR3_DU_PER_ER = 1.0f;      // distance units / Earth radii
  const float STR3_J2 =  1.082616e-3f;    // 2nd Earth grav. zonal harmonic 
  const float STR3_J3 = -0.253881e-5f;    // 3rd Earth grav. zonal harmonic
  const float STR3_J4 = -1.655970e-6f;    // 4th Earth grav. zonal harmonic
  const float STR3_K2 =  5.413080e-4f;    // 0.5*J2*(eq. Earth rad.)^2
  const float STR3_K4 =  0.62098875e-6f;  // -0.375*J4*(eq. Earth rad.)^4
  const float STR3_RAD_PER_DEG = 0.174532925e-1f; // radian / degree
  const float STR3_PI = 3.14159265f;              // pi
  const float STR3_HALF_PI = 1.57079633f;         // pi / 2
  const float STR3_Q0 = 120.0f;                   // density function parameter
  const float STR3_S0 = 78.0f;                    // density function parameter
  const float STR3_TWO_THIRDS = 0.66666667f;      // two thirds
  const float STR3_TWO_PI = 6.2831853f;           // two pi
  const float STR3_THREE_HALVES_PI = 4.71238898f; // 3*pi/2
  const float STR3_KM_PER_ER = 6378.135f;         // kilometers per Earth radii
  const float STR3_MIN_PER_DAY = 1440.0f;         // time units per day
  const float STR3_A30 = 0.253881e-5f;            // -J3*(eq. Earth rad.)^3
  const float STR3_RAD_PER_REV = 6.2831853f;      // radians per revolution

  // World Geodetic System 1972 (WGS 72)
  // See cote/references/seppelin1974department.pdf
  const double WGS_72_A  = 6378.135;        // Earth semimajor axis
  const double WGS_72_WE = 0.7292115147e-4; // Earth angular velocity rad/sec
  const double WGS_72_F  = 1.0/298.26;      // Flattening

  // World Geodetic System 1984 (WGS 84)
  // See cote/references/dod2014wgs.pdf
  const double WGS_84_A  = 6378.137;          // Earth semimajor axis km
  const double WGS_84_WE = 7.292115e-5;       // Earth angular velocity rad/sec
  const double WGS_84_F  = 1.0/298.257223563; // Flattening
  const double WGS_84_C  = 2.99792458e8;      // Speed of light in vacuum m/s

  // Space Mission Analysis and Design
  // See cote/references/wertz1999space.pdf
  // TODO: replace with crane model
  const uint8_t CHANNEL_TYPE_DOWNLINK  = 0;
  const uint8_t CHANNEL_TYPE_CROSSLINK = 1;
  const uint8_t CHANNEL_TYPE_UPLINK    = 2;
  const double SMAD_T0 = 290.0;        // Noise reference temperature (Kelvin)
  const double SMAD_LR = 0.8912509381; // Receiver line loss factor

  // ASTM E-490-00 Standard Extraterrestrial Spectrum Reference 2000
  // See https://www.nrel.gov/grid/solar-resource/spectra-astm-e490.html
  const double SOLAR_CONSTANT = 1366.1; // Units: watt per meter squared

  // Other
  const double SUN_RADIUS_KM = 695700.0; // radius of the Sun in kilometers
}}
