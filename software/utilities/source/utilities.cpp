// utilities.cpp
// utilities implementation file
//
// Written by Bradley Denby
// Other contributors: Mason Xiao
//
// See the top-level LICENSE file for the license.

// Standard library
#include <array>           // array, round
#include <cmath>           // fmod
#include <cstdint>         // int16_t, uint8_t, int32_t
#include <fstream>         // ifstream
#include <tuple>           // tuple

// cote library
#include <constants.hpp>   // Unit conversion, Space Track Report, etc constants
#include <utilities.hpp>   // sgp4

namespace cote { namespace util {
  double isLeapYear(const int16_t& year) {
    return ((year%400==0) || (year%100!=0 && year%4==0));
  }

  std::tuple<int16_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint32_t>
   getTleEpoch(const std::string& tleFile) {
    // Default values
    std::tuple<int16_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint32_t> dt =
     std::make_tuple(0,0,0,0,0,0,0);
    // Set up to parse TLE file
    std::ifstream tleHandle(tleFile);
    std::string line = "";
    std::getline(tleHandle,line);
    while(line.substr(0,2)!="1 " && std::getline(tleHandle,line)) {}
    if(line.substr(0,2)=="1 ") {
      // Parse TLE year
      int16_t yy   = static_cast<int16_t>(std::stoi(line.substr(18,2)));
      int16_t year = static_cast<int16_t>(static_cast<int16_t>(2000)+yy);
      if(yy>=static_cast<int16_t>(57)) {
        year = static_cast<int16_t>(static_cast<int16_t>(1900)+yy);
      }
      // Parse TLE day into month, day, hour, minute, second, nanosecond
      uint16_t dayPerMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
      if(util::isLeapYear(year)) {
        dayPerMonth[1]+=1;
      }
      double ddd = std::stod(line.substr(20,12));
      uint16_t doy = static_cast<uint16_t>(std::floor(ddd)); // day of year
      uint8_t month = 1;
      uint16_t excess = 0;
      uint16_t thresh = dayPerMonth[0];
      for(size_t i=1; thresh<doy && i<12; i++) {
        month+=1;
        excess = thresh;
        thresh+=dayPerMonth[i];
      }
      uint16_t day16 = doy;
      if(month!=1) {
        day16-=excess;
      }
      uint8_t day = static_cast<uint8_t>(day16);
      double hh = ddd-static_cast<double>(doy);
      uint8_t hour = static_cast<uint8_t>(std::floor(hh*24.0));
      double mm = hh*24.0-std::floor(hh*24.0);
      uint8_t minute = static_cast<uint8_t>(std::floor(mm*60.0));
      double ss = mm*60.0-std::floor(mm*60.0);
      uint8_t second = static_cast<uint8_t>(std::floor(ss*60.0));
      double ns = ss*60.0-std::floor(ss*60.0);
      uint32_t nanosecond = static_cast<uint32_t>(std::round(ns*1.0e9));
      // Record TLE epoch
      std::get<0>(dt) = year;
      std::get<1>(dt) = month;
      std::get<2>(dt) = day;
      std::get<3>(dt) = hour;
      std::get<4>(dt) = minute;
      std::get<5>(dt) = second;
      std::get<6>(dt) = nanosecond;
    }
    return dt;
  }

  double calcJulianDayFromYMD(
   const int16_t& year, const uint8_t& month, const uint8_t& day
  ) {
    const int32_t JD =
     static_cast<int32_t>(day)-32075+1461*(
      static_cast<int32_t>(year)+4800+(static_cast<int32_t>(month)-14)/12
     )/4+367*(
      static_cast<int32_t>(month)-2-(static_cast<int32_t>(month)-14)/12*12
     )/12-3*(
      (static_cast<int32_t>(year)+4900+(static_cast<int32_t>(month)-14)/12)/100
     )/4;
    return static_cast<double>(JD)-0.5;
  }

  uint32_t calcSecSinceMidnight(
   const uint8_t& hour, const uint8_t& minute, const uint8_t& second
  ) {
    return
     static_cast<uint32_t>(second)+
     static_cast<uint32_t>(cnst::SEC_PER_MIN)*(
      static_cast<uint32_t>(minute)+
      static_cast<uint32_t>(cnst::MIN_PER_HOUR)*static_cast<uint32_t>(hour)
     );
  }

  double calcTdiffMin(
   const int16_t& yearEvent, const uint8_t& monthEvent, const uint8_t& dayEvent,
   const uint8_t& hourEvent, const uint8_t& minuteEvent,
   const uint8_t& secondEvent, const uint32_t& nanosecondEvent,
   const int16_t& yearEpoch, const uint8_t& monthEpoch, const uint8_t& dayEpoch,
   const uint8_t& hourEpoch, const uint8_t& minuteEpoch,
   const uint8_t& secondEpoch, const uint32_t& nanosecondEpoch
  ) {
    double   eventJD = calcJulianDayFromYMD(yearEvent,monthEvent,dayEvent);
    uint32_t eventSC =
     static_cast<uint32_t>(secondEvent)+
     static_cast<uint32_t>(cnst::SEC_PER_MIN)*(
      static_cast<uint32_t>(minuteEvent)+
      static_cast<uint32_t>(cnst::MIN_PER_HOUR)*static_cast<uint32_t>(hourEvent)
     );
    uint32_t eventNS = nanosecondEvent;
    double   epochJD = calcJulianDayFromYMD(yearEpoch,monthEpoch,dayEpoch);
    uint32_t epochSC =
     static_cast<uint32_t>(secondEpoch)+
     static_cast<uint32_t>(cnst::SEC_PER_MIN)*(
      static_cast<uint32_t>(minuteEpoch)+
      static_cast<uint32_t>(cnst::MIN_PER_HOUR)*static_cast<uint32_t>(hourEpoch)
     );
    uint32_t epochNS = nanosecondEpoch;
    return
     (eventJD-epochJD)*static_cast<double>(cnst::MIN_PER_DAY)+
     (
      static_cast<double>(eventSC)-static_cast<double>(epochSC)
     )/static_cast<double>(cnst::SEC_PER_MIN)+
     (
      static_cast<double>(eventNS)-static_cast<double>(epochNS)
     )/(
      static_cast<double>(cnst::NS_PER_SEC)*
      static_cast<double>(cnst::SEC_PER_MIN)
     );
  }

  std::array<float,3> sgp4(
   const float& bstar, const float& i0, const float& o0, const float& e0,
   const float& w0, const float& m0, const float& n0, const float& tsince
  ) {
    // Recover mean motion and semimajor axis     // line001-line013 boilerplate
    const float a1 =                                             // eq01,line014
     std::pow(cnst::STR3_KE/n0,cnst::STR3_TWO_THIRDS);
    const float cosi0 = std::cos(i0);                            //      line015
    const float thetar2 = cosi0*cosi0;                           // eq10,line016
    const float thetar2t3m1 = thetar2*3.0f-1.0f;       // X3THM1 //      line017
                                                                 // omit line018
    const float beta0r2 = 1.0f-e0*e0;                            //      line019
    const float beta0 = std::sqrt(beta0r2);                      // eq12,line020
    const float delta1 =                                         // eq02,line021
     1.5f*cnst::STR3_K2*thetar2t3m1/(a1*a1*beta0*beta0r2);
    const float a0 =                                             // eq03,line022
     a1*(
      1.0f-0.5f*cnst::STR3_TWO_THIRDS*delta1-delta1*delta1-
      (134.0f/81.0f)*delta1*delta1*delta1
     );
    const float delta0 =                                         // eq04,line023
     1.5f*cnst::STR3_K2*thetar2t3m1/(a0*a0*beta0*beta0r2);
    const float n0pp = n0/(1.0f+delta0);                         // eq05,line024
    const float a0pp = a0/(1.0f-delta0);                         // eq06,line025
    // Check if perigee height is less than 220 km   // line026-line033 comments
    bool isimp = false;                                          //      line034
    // a0pp*(1.0f-e0)/ae and 220.0f/kmper+ae are distances to Earth center
    if(
     a0pp*(1.0f-e0)/cnst::STR3_DU_PER_ER <
     (220.0f/cnst::STR3_KM_PER_ER+cnst::STR3_DU_PER_ER)
    ) {
      isimp = true;                                              //      line035
    }                                                // line036-line039 comments
    // Set constants based on perigee height
    float q0msr4temp =                                 // QOMS2T // dr26,line041
     std::pow(
      (cnst::STR3_Q0-cnst::STR3_S0)*cnst::STR3_DU_PER_ER/cnst::STR3_KM_PER_ER,
      4.0f
     );
    float stemp =                                           // S // dr27,line040
     cnst::STR3_DU_PER_ER*(1.0f+cnst::STR3_S0/cnst::STR3_KM_PER_ER);
    const float perigee =                                        //      line042
     (a0pp*(1.0f-e0)-cnst::STR3_DU_PER_ER)*cnst::STR3_KM_PER_ER;
    if(perigee <= 98.0f) {                                       //      line045
      stemp = 20.0f;                                             //      line046
      q0msr4temp =                                               //      line047
       std::pow(
        (cnst::STR3_Q0-stemp)*cnst::STR3_DU_PER_ER/cnst::STR3_KM_PER_ER,
        4.0f
       );
      stemp = stemp/cnst::STR3_KM_PER_ER+cnst::STR3_DU_PER_ER;   // eq08,line048
    } else if(perigee < 156.0f) {                                //      line043
      stemp = perigee-cnst::STR3_S0;                             //      line044
      q0msr4temp =                                               //      line047
       std::pow(
        (cnst::STR3_Q0-stemp)*cnst::STR3_DU_PER_ER/cnst::STR3_KM_PER_ER,
        4.0f
       );
      stemp = stemp/cnst::STR3_KM_PER_ER+cnst::STR3_DU_PER_ER;   // eq07,line048
    }
    const float q0msr4 = q0msr4temp;                   // QOMS24 // eq09,line041
    const float s = stemp;                                 // S4 //      line040
    const float xi = 1.0f/(a0pp-s);                       // TSI // eq11,line050
    const float eta = a0pp*e0*xi;                                // eq13,line051
    const float etar2 = eta*eta;                                 //      line052
    const float e0eta = e0*eta;                          // EETA //      line053
    const float psi = std::abs(1.0f-etar2);             // PSISQ //      line054
    const float q0msr4txir4 = q0msr4*std::pow(xi,4.0f);  // COEF //      line055
    const float q0msr4txir4dpsir3p5 =                   // COEF1 //      line056
     q0msr4txir4/std::pow(psi,3.5f);
    const float c2 =                                             // eq14,line057
     q0msr4txir4dpsir3p5*n0pp*(                                  // thru line058
      a0pp*(1.0f+1.5f*etar2+4.0f*e0eta+e0eta*etar2)+
      0.75f*cnst::STR3_K2*xi/psi*thetar2t3m1*(8.0f+24.0f*etar2+3.0f*etar2*etar2)
     );
    const float c1 = bstar*c2;                                   // eq15,line059
    const float sini0 = std::sin(i0);                            //      line060
    const float a30dk2 =                               // A3OVK2 //omit? line061
     cnst::STR3_A30/cnst::STR3_K2*std::pow(cnst::STR3_DU_PER_ER,3.0f);
    const float c3 =                                             // eq16,line062
     q0msr4txir4*xi*a30dk2*n0pp*cnst::STR3_DU_PER_ER*sini0/e0;
    const float nthetar2a1 = -1.0f*thetar2+1.0f;       // X1MTH2 //      line063
    const float c4 =                                             // eq17,line064
     2.0f*n0pp*q0msr4txir4dpsir3p5*a0pp*beta0r2*(                // thru line068
      (2.0f*eta*(1.0f+e0eta)+0.5f*e0+0.5f*eta*etar2)-
      (2.0f*cnst::STR3_K2*xi/(a0pp*psi))*
      (
       -3.0f*thetar2t3m1*(1.0f+1.5f*etar2-2.0f*e0eta-0.5f*e0eta*etar2)+
       0.75f*nthetar2a1*(2.0f*etar2-e0eta-e0eta*etar2)*std::cos(2.0f*w0)
      )
     );
    const float thetar4 = thetar2*thetar2;                       //      line070
    const float pinvsq = 1.0f/(a0pp*a0pp*beta0r2*beta0r2);       //      line049
    const float k2m3pinvsqn0pp = cnst::STR3_K2*3.0f*pinvsq*n0pp; //TEMP1 line071
    const float k2r2m3pinvsqr2n0pp =                    // TEMP2 //      line072
     k2m3pinvsqn0pp*cnst::STR3_K2*pinvsq;
    const float k4m1p25pinvsqr2n0pp =                   // TEMP3 //      line073
     cnst::STR3_K4*1.25f*pinvsq*pinvsq*n0pp;
    const float mdt =                                            //      line074
     n0pp+k2m3pinvsqn0pp*thetar2t3m1*beta0*0.5f+                 // thru line075
     k2r2m3pinvsqr2n0pp*(13.0f-78.0f*thetar2+137.0f*thetar4)*beta0*0.0625f;
    const float n5thetar2a1 = -5.0f*thetar2+1.0f;      // X1M5TH //      line076
    const float wdt =                                            //      line077
     k2m3pinvsqn0pp*n5thetar2a1*-0.5f+                           // thru line078
     k2r2m3pinvsqr2n0pp*(7.0f-114.0f*thetar2+395.0f*thetar4)*0.0625f+
     k4m1p25pinvsqr2n0pp*(3.0f-36.0f*thetar2+49.0f*thetar4);
                                                                 // omit line079
    const float odt =                                            //      line080
     -1.0f*k2m3pinvsqn0pp*cosi0+                                 // thru line081
     k2r2m3pinvsqr2n0pp*(4.0f*cosi0-19.0f*cosi0*thetar2)*0.5f+
     k4m1p25pinvsqr2n0pp*cosi0*(3.0f-7.0f*thetar2)*2.0f;
    const float mdf = m0+mdt*tsince;                             // eq22,line105
    const float wdf = w0+wdt*tsince;                             // eq23,line106
    const float odf = o0+odt*tsince;                             // eq24,line107
                                                                 // omit line082
                                                                 // omit line083
    const float uo = -3.5f*beta0r2*k2m3pinvsqn0pp*cosi0*c1;      // l79+ line084
                                                                 // omit line085
                                                                 // omit line088
                                                                 // omit line089
                                                                 // omit line090
    const float tsincer2 = tsince*tsince;                        //      line110
    float mptemp = mdf;                                          //      line109
    float wtemp = wdf;                                           //      line108
    float uatemp = 1.0f-c1*tsince;                      // TEMPA //      line112
    float uetemp = bstar*c4*tsince;                     // TEMPE //      line113
    float ultemp = 1.5f*c1*tsincer2;                    // TEMPL // l85+ line114
    if(!isimp) {                                              // line091,line115
      const float c1r2 = c1*c1;                                  //      line092
      const float c5 =                                           // eq18,line069
       2.0f*q0msr4txir4dpsir3p5*a0pp*beta0r2*(
        1.0f+2.75f*(etar2+e0eta)+e0eta*etar2
       );
      const float d2 = 4.0f*a0pp*xi*c1r2;                        // eq19,line093
                                                                 // omit line094
      const float d3 = d2*xi*c1*(17.0f*a0pp+s)/3.0f;             // eq20,line095
      const float d4 =                                           // eq21,line096
       0.5f*d2*xi*xi*c1r2*a0pp*(221.0f*a0pp+31.0f*s)/3.0f;
      const float deltaw =                                       // eq25,line116
       bstar*c3*std::cos(w0)*tsince;                             // l82+
      const float deltam =                                       // eq26,line117
       -1.0f*cnst::STR3_TWO_THIRDS*q0msr4txir4*bstar*cnst::STR3_DU_PER_ER/e0eta*
       (std::pow(1.0f+eta*std::cos(mdf),3.0f)-                   // l83+
        std::pow(1.0f+eta*std::cos(m0),3.0f));                   // l88+
                                                                 // omit line118
      mptemp = mptemp+deltaw+deltam;                             //      line119
      wtemp = wtemp-deltaw-deltam;                               //      line120
                                                                 // omit line121
                                                                 // omit line122
      uatemp =                                                   //      line123
       uatemp-d2*tsincer2-d3*tsincer2*tsince-d4*tsince*tsincer2*tsince;
      uetemp =                                                   //      line124
       uetemp+bstar*c5*(std::sin(mptemp)-std::sin(m0));
      ultemp =                                                   //      line125
       ultemp+                                                   // thru line126
       (d2+2.0f*c1r2)*tsincer2*tsince+                           // l97+
       0.25f*(3.0f*d3+12.0f*c1*d2+10.0f*c1r2*c1)*tsince*tsincer2*tsince+
       0.2f*(                                                    // l98+
        3.0f*d4+12.0f*c1*d3+6.0f*d2*d2+30.0f*c1r2*d2+15.0f*c1r2*c1r2
       )*tsince*tsincer2*tsince*tsince;                          // l99+ l100+
    }                                                // line102-line104 comments
                                                                 // omit line097
                                                                 // omit line098
                                                                 // omit line099
                                                                 // omit line100
                                                                 // omit line101
    const float mp = mptemp;                                     // eq27,line109
    const float w = wtemp;                                       // eq28,line108
    const float o = odf+uo*tsincer2;                             // eq29,line111
    const float a = a0pp*std::pow(uatemp,2.0f);                  // eq31,line127
    const float e = e0-uetemp;                                   // eq30,line128
    const float l = mp+w+o+n0pp*ultemp;                          // eq32,line129
    const float beta = std::sqrt(1.0f-e*e);                      // eq33,line130
    const float n = cnst::STR3_KE/std::pow(a,1.5f);              // eq34,line131
    // Long period periodics                         // line132-line134 comments
    const float axn = e*std::cos(w);                             // eq35,line135
    const float ull =                                   // XLCOF //      line086
     0.125f*a30dk2*sini0*(3.0f+5.0f*cosi0)/(1.0f+cosi0);         // omit line136
    const float ll = axn*ull/(a*beta*beta);                      // eq36,line137
    const float uaynl = 0.25f*a30dk2*sini0;             // AYCOF //      line087
    const float aynl = uaynl/(a*beta*beta);                      // eq37,line138
    const float lt = l+ll;                                       // eq38,line139
    const float ayn = e*std::sin(w)+aynl;                        // eq39,line140
    // Solve Kepler's equation                       // line141-line143 comments
    // FMOD /////////////////////////////////////////////////////// // FUNC FMOD
    float utemp = std::fmod(lt-o,cnst::STR3_TWO_PI);             // eq40,line144
    if(utemp<0.0) {
      utemp+=cnst::STR3_TWO_PI;
    }
    const float u = utemp;
    // FMOD /////////////////////////////////////////////////////// // END  FMOD
    float eawprev = u;                                           // eq43,line145
    for(size_t i=0; i<10; i++) {                                 //      line146
      float eawcurr =                                            // eq41,line153
       eawprev+                                                  // eq42
       (u-ayn*std::cos(eawprev)+axn*std::sin(eawprev)-eawprev)/  // omit line149
       (1.0f-ayn*std::sin(eawprev)-axn*std::cos(eawprev));       // omit line150
      if(std::abs(eawcurr-eawprev) <= 1.0e-6f) {                 //      line154
        break;                                                   // omit line151
      }                                                          // omit line152
      eawprev = eawcurr;                                         //      line155
    }                                                // line156-line158 comments
    const float eaw = eawprev;
    // Short period periodics
    const float sineaw = std::sin(eaw);                          //      line147
    const float coseaw = std::cos(eaw);                          //      line148
    const float ecose = axn*coseaw+ayn*sineaw;                   // eq44,line159
    const float esine = axn*sineaw-ayn*coseaw;                   // eq45,line160
    const float elr2 = axn*axn+ayn*ayn;                          // eq46,line161
                                                                 // omit line162
    const float pl = a*(1.0f-elr2);                              // eq47,line163
    const float r = a*(1.0f-ecose);                              // eq48,line164
                                                                 // omit line165
    const float rdt = cnst::STR3_KE*std::sqrt(a)*esine/r;        // eq49,line166
    const float rfdt = cnst::STR3_KE*std::sqrt(pl)/r;            // eq50,line167
                                                                 // omit line168
                                                                 // omit line169
                                                                 // omit line170
    const float cosu =                                           // eq51,line171
     a*(coseaw-axn+ayn*esine/(1.0f+std::sqrt(1.0f-elr2)))/r;
    const float sinu =                                           // eq52,line172
     a*(sineaw-ayn-axn*esine/(1.0f+std::sqrt(1.0f-elr2)))/r;
    // ACTAN /////////////////////////////////////////////////// // FUNC ACTAN
    float lowerutemp = 0.0f;
    if(cosu==0.0f) {
      if(sinu==0.0f) {
        lowerutemp = 0.0f;
      } else if(sinu>0.0f) {
        lowerutemp = cnst::STR3_HALF_PI;
      } else {
        lowerutemp = cnst::STR3_THREE_HALVES_PI;
      }
    } else if(cosu>0.0f) {
      if(sinu==0.0f) {
        lowerutemp = 0.0f;
      } else if(sinu>0.0f) {
        lowerutemp = std::atan(sinu/cosu);
      } else {
        lowerutemp = cnst::STR3_TWO_PI+std::atan(sinu/cosu);
      }
    } else {
      lowerutemp = cnst::STR3_PI+std::atan(sinu/cosu);
    }
    // ACTAN /////////////////////////////////////////////////// // END  ACTAN
    const float loweru = lowerutemp;                             // eq53,line173
    const float sin2u = 2.0f*sinu*cosu;                          //      line174
    const float cos2u = 2.0f*cosu*cosu-1.0f;                     //      line175
                                                                 // omit line176
                                                                 // omit line177
                                                                 // omit line178
                                                     // line179-line181 comments
    const float deltar = 0.5f*cnst::STR3_K2*nthetar2a1*cos2u/pl; // eq54fline182
    const float deltau =                                         // eq55fline183
     -0.25f*cnst::STR3_K2*(7.0f*thetar2-1.0f)*sin2u/(pl*pl);
    const float deltao = 1.5f*cnst::STR3_K2*cosi0*sin2u/(pl*pl); // eq56fline184
    const float deltai =                                         // eq57fline185
     1.5f*cnst::STR3_K2*cosi0*sini0*cos2u/(pl*pl);
    const float deltardt =                                       // eq58fline186
     -1.0f*cnst::STR3_K2*n*nthetar2a1*sin2u/pl;
    const float deltarfdt =                                      // eq59fline187
     cnst::STR3_K2*n*(nthetar2a1*cos2u+1.5*thetar2t3m1)/pl;
    const float rk =                                             // eq60,line182
     r*(1.0f-1.5f*cnst::STR3_K2*std::sqrt(1.0f-elr2)*thetar2t3m1/(pl*pl))+
     deltar;
    const float uk = u+deltau;                                   // eq61,line183
    const float ok = o+deltao;                                   // eq62,line184
    const float ik = i0+deltai;                                  // eq63,line185
    const float rkdt = rdt+deltardt;                             // eq64,line186
    const float rfkdt = rfdt+deltarfdt;                          // eq65,line187
    // Unit orientation vectors                      // line188-line190 comments
    const float sinuk = std::sin(uk);                            //      line191
    const float cosuk = std::cos(uk);                            //      line192
    const float sinik = std::sin(ik);                            //      line193
    const float cosik = std::cos(ik);                            //      line194
    const float sinok = std::sin(ok);                            //      line195
    const float cosok = std::cos(ok);                            //      line196
    const float mx = -1.0f*sinok*cosik;                          // eq68,line197
    const float my = cosok*cosik;                                // eq69,line198
    const float mz = sinik;                                      // eq70
    const float nx = cosok;                                      // eq71
    const float ny = sinok;                                      // eq72
    const float nz = 0.0f;                                       // eq73
    const float ux = mx*sinuk+nx*cosuk;                          // eq66,line199
    const float uy = my*sinuk+ny*cosuk;                          // |   ,line200
    const float uz = mz*sinuk+nz*cosuk;                          // -   ,line201
    const float vx = mx*cosuk-nx*sinuk;                          // eq67,line202
    const float vy = my*cosuk-ny*sinuk;                          // |   ,line203
    const float vz = mz*cosuk-nz*sinuk;                          // -   ,line204
    // Position and velocity                         // line205-line207 comments
    const float px = rk*ux;                                      // eq74,line208
    const float py = rk*uy;                                      // |   ,line209
    const float pz = rk*uz;                                      // -   ,line210
    const float sx = rkdt*ux+rfkdt*vx;                           // eq75,line211
    const float sy = rkdt*uy+rfkdt*vy;                           // |   ,line212
    const float sz = rkdt*uz+rfkdt*vz;                           // -   ,line213
    // Return ECI position
    std::array<float,3> eciPosn = {
     px*cnst::STR3_KM_PER_ER/cnst::STR3_DU_PER_ER,
     py*cnst::STR3_KM_PER_ER/cnst::STR3_DU_PER_ER,
     pz*cnst::STR3_KM_PER_ER/cnst::STR3_DU_PER_ER
    };
    return eciPosn;
  }

  double calcGMSTRadFromUT1(
   const double& julianDay, const uint32_t& second, const uint32_t& nanosecond
  ) {
    // T_u: Julian centuries since J2000; treated as unitless for some reason
    const double T_u = (julianDay-2451545.0)/36525.0;
    // gmst0hSec: GMST (seconds) at 0h UT1 of julianDay given by the GMST1 equ.
    double gmst0hSec =
     24110.54841+8640184.812866*T_u+0.093104*T_u*T_u-6.2e-6*T_u*T_u*T_u;
    // Calculate the sidereal time (GMST) to solar time (UT1) ratio, i.e. 1/r'
    const double rpinv = 1.002737909350795+5.9006e-11*T_u-5.9e-15*T_u*T_u;
    // Convert the extra seconds and nanoseconds from solar time to sidereal
    // time and add them to GMST
    double gmstSec =
     gmst0hSec+rpinv*(
      static_cast<double>(second)+
      static_cast<double>(nanosecond)/static_cast<double>(cnst::NS_PER_SEC)
     );
    // Modulo 86400 seconds
    gmstSec = std::fmod(gmstSec,static_cast<double>(cnst::SEC_PER_DAY));
    if(gmstSec<0.0) {
      gmstSec += static_cast<double>(cnst::SEC_PER_DAY);
    }
    // Return GMST modulo 86400 seconds converted to radians
    return gmstSec*(cnst::TWO_PI/static_cast<double>(cnst::SEC_PER_DAY));
  }

  double calcERARadFromUT1(
   const double& julianDay, const uint32_t& second, const uint32_t& nanosecond
  ) {
    return calcGMSTRadFromUT1(julianDay,second,nanosecond);
  }

  std::array<double,3> dtlla2eci(
   const double& julianDay, const uint32_t& second, const uint32_t& nanosecond,
   const double& lat, const double& lon, const double& alt
  ) {
    const double zrot = calcGMSTRadFromUT1(julianDay, second, nanosecond);
    const double C =
     cnst::WGS_84_A/
     std::sqrt(
      1.0+cnst::WGS_84_F*(cnst::WGS_84_F-2.0)*std::pow(std::sin(lat),2.0)
     );
    const double S = std::pow(cnst::WGS_84_F-1.0,2.0)*C;
    std::array<double,3> eciPosn = {
     (C+alt)*std::cos(lat)*std::cos(zrot+lon),
     (C+alt)*std::cos(lat)*std::sin(zrot+lon),
     (S+alt)*std::sin(lat)
    };
    return eciPosn;
  }

  std::array<double,3> dteci2sez(
   const double& julianDay, const uint32_t& second, const uint32_t& nanosecond,
   const double& lat, const double& lon, const std::array<double,3>& eciVector
  ) {
    const double zrot = calcGMSTRadFromUT1(julianDay, second, nanosecond);
    const double S =
     std::sin(lat)*std::cos(zrot+lon)*eciVector.at(0)+
     std::sin(lat)*std::sin(zrot+lon)*eciVector.at(1)-
     std::cos(lat)*eciVector.at(2);
    const double E =
     -1.0*std::sin(zrot+lon)*eciVector.at(0)+std::cos(zrot+lon)*eciVector.at(1);
    const double Z =
     std::cos(lat)*std::cos(zrot+lon)*eciVector.at(0)+
     std::cos(lat)*std::sin(zrot+lon)*eciVector.at(1)+
     std::sin(lat)*eciVector.at(2);
    std::array<double,3> sezVector = {S, E, Z};
    return sezVector;
  }

  double calcAzimuthDeg(
   const double& julianDay, const uint32_t& second, const uint32_t& nanosecond,
   const double& lat, const double& lon, const double& alt,
   const std::array<double,3>& eciPosnSat
  ) {
    const std::array<double,3> eciPosnGnd =
     dtlla2eci(julianDay, second, nanosecond, lat, lon, alt);
    const double DX = eciPosnSat.at(0)-eciPosnGnd.at(0);
    const double DY = eciPosnSat.at(1)-eciPosnGnd.at(1);
    const double DZ = eciPosnSat.at(2)-eciPosnGnd.at(2);
    const double RANGE = std::sqrt(DX*DX+DY*DY+DZ*DZ);
    std::array<double,3> eciVector = {DX, DY, DZ};
    std::array<double,3> sezVector =
     dteci2sez(julianDay,second,nanosecond,lat,lon,eciVector);
    return std::atan2(sezVector.at(1),-1.0*sezVector.at(0))/cnst::RAD_PER_DEG;
  }

  double calcElevationDeg(
   const double& julianDay, const uint32_t& second, const uint32_t& nanosecond,
   const double& lat, const double& lon, const double& alt,
   const std::array<double,3>& eciPosnSat
  ) {
    const std::array<double,3> eciPosnGnd =
     dtlla2eci(julianDay, second, nanosecond, lat, lon, alt);
    const double DX = eciPosnSat.at(0)-eciPosnGnd.at(0);
    const double DY = eciPosnSat.at(1)-eciPosnGnd.at(1);
    const double DZ = eciPosnSat.at(2)-eciPosnGnd.at(2);
    const double RANGE = std::sqrt(DX*DX+DY*DY+DZ*DZ);
    std::array<double,3> eciVector = {DX, DY, DZ};
    std::array<double,3> sezVector =
     dteci2sez(julianDay,second,nanosecond,lat,lon,eciVector);
    return std::asin(sezVector.at(2)/RANGE)/cnst::RAD_PER_DEG;
  }

  double calcAltitudeKm(const std::array<double,3>& eciPosnSat) {
    const double r =
     std::sqrt(std::pow(eciPosnSat.at(0),2.0)+std::pow(eciPosnSat.at(1),2.0));
    // If r==0, return magnitude minus semi-minor axis
    if(r==0) {
      return magnitude(eciPosnSat)-cnst::WGS_84_A*(1.0-cnst::WGS_84_F);
    }
    const double z = eciPosnSat.at(2);
    // If z==0, return magnitude minus semi-major axis
    if(z==0) {
      return magnitude(eciPosnSat)-cnst::WGS_84_A;
    }
    const double a = cnst::WGS_84_A;
    const double b = (z<0.0 ? -1.0 : 1.0)*cnst::WGS_84_A*(1.0-cnst::WGS_84_F);
    const double E = (b*z-(a*a-b*b))/(a*r);
    const double F = (b*z+(a*a-b*b))/(a*r);
    const double P = 4.0*(E*F+1.0)/3.0;
    const double Q = 2.0*(E*E-F*F);
    const double D = std::pow(P,3.0)+std::pow(Q,2.0);
    double vp = 0.0;
    if(D<0.0) {
      vp =
       2.0*std::sqrt(-1.0*P)*std::cos(std::acos(Q/(P*std::sqrt(-1.0*P)))/3.0);
    } else {
      vp =
       std::pow(std::sqrt(D)-Q,1.0/3.0)-std::pow(std::sqrt(D)+Q,1.0/3.0);
    }
    if(vp*vp<std::abs(P)) {
      vp = -1.0*(std::pow(vp,3.0)+2.0*Q)/(3.0*P);
    }
    const double v = vp;
    const double G = (std::sqrt(E*E+v)+E)/2.0;
    const double t = std::sqrt(G*G+(F-v*G)/(2.0*G-E))-G;
    const double lat = std::atan(a*(1.0-t*t)/(2.0*b*t));
    return (r-a*t)*std::cos(lat)+(z-b)*std::sin(lat);
  }

  double calcSubpointLatitude(const std::array<double,3>& eciPosnSat) {
    const double r =
     std::sqrt(std::pow(eciPosnSat.at(0),2.0)+std::pow(eciPosnSat.at(1),2.0));
    // If r==0, return magnitude minus semi-minor axis
    if(r==0) {
      return magnitude(eciPosnSat)-cnst::WGS_84_A*(1.0-cnst::WGS_84_F);
    }
    const double z = eciPosnSat.at(2);
    // If z==0, return magnitude minus semi-major axis
    if(z==0) {
      return magnitude(eciPosnSat)-cnst::WGS_84_A;
    }
    const double a = cnst::WGS_84_A;
    const double b = (z<0.0 ? -1.0 : 1.0)*cnst::WGS_84_A*(1.0-cnst::WGS_84_F);
    const double E = (b*z-(a*a-b*b))/(a*r);
    const double F = (b*z+(a*a-b*b))/(a*r);
    const double P = 4.0*(E*F+1.0)/3.0;
    const double Q = 2.0*(E*E-F*F);
    const double D = std::pow(P,3.0)+std::pow(Q,2.0);
    double vp = 0.0;
    if(D<0.0) {
      vp =
       2.0*std::sqrt(-1.0*P)*std::cos(std::acos(Q/(P*std::sqrt(-1.0*P)))/3.0);
    } else {
      vp =
       std::pow(std::sqrt(D)-Q,1.0/3.0)-std::pow(std::sqrt(D)+Q,1.0/3.0);
    }
    if(vp*vp<std::abs(P)) {
      vp = -1.0*(std::pow(vp,3.0)+2.0*Q)/(3.0*P);
    }
    const double v = vp;
    const double G = (std::sqrt(E*E+v)+E)/2.0;
    const double t = std::sqrt(G*G+(F-v*G)/(2.0*G-E))-G;
    return std::atan(a*(1.0-t*t)/(2.0*b*t))/cnst::RAD_PER_DEG;
  }

  double calcSubpointLongitude(
   const double& julianDay, const uint32_t& second, const uint32_t& nanosecond,
   const std::array<double,3>& eciPosnSat
  ) {
    const double zrot = calcGMSTRadFromUT1(julianDay, second, nanosecond);
    // Shifts lons to familiar range but lowers performance
    //double longitudeRad = (std::atan2(eciPosnSat.at(1),eciPosnSat.at(0))-zrot);
    //while(longitudeRad<0.0) {
    //  longitudeRad+=cnst::TWO_PI;
    //}
    //return longitudeRad/cnst::RAD_PER_DEG;
    return
     (std::atan2(eciPosnSat.at(1),eciPosnSat.at(0))-zrot)/cnst::RAD_PER_DEG;
  }

  double calcGreatCircleArc(
   const double& az1Deg, const double& el1Deg,
   const double& az2Deg, const double& el2Deg
  ) {
    const double az1 = az1Deg*cnst::RAD_PER_DEG;
    const double el1 = el1Deg*cnst::RAD_PER_DEG;
    const double az2 = az2Deg*cnst::RAD_PER_DEG;
    const double el2 = el2Deg*cnst::RAD_PER_DEG;
    const double daz = std::abs(az1-az2);
    return std::atan2(
     std::sqrt(
      std::pow(std::cos(el2)*std::sin(daz),2.0)+
      std::pow(
       std::cos(el1)*std::sin(el2)-std::sin(el1)*std::cos(el2)*std::cos(daz),2.0
      )
     ),
     std::sin(el1)*std::sin(el2)+std::cos(el1)*std::cos(el2)*std::cos(daz)
    );
  }

  double magnitude(const std::array<double,3>& cartesian) {
    return std::sqrt(
     std::pow(cartesian.at(0),2.0)+
     std::pow(cartesian.at(1),2.0)+
     std::pow(cartesian.at(2),2.0)
    );
  }

  double dB2Dec(const double& db) {
    return std::pow(10.0,db/10.0);
  }

  uint64_t calcMaxBitsPerSec(
   const double& txPowerW, const double& txLineLossFactor,
   const double& txGainFactor, const double& atmoLossFactor,
   const double& rxGainFactor, const double& centerFreqHz,
   const double& rangeKm, const double& systemNoiseTempK,
   const double& bandwidthHz
  ) {
    const double C =
     txPowerW*txLineLossFactor*txGainFactor*atmoLossFactor*rxGainFactor*
     std::pow(
      (cnst::WGS_84_C/centerFreqHz)/(4.0*cnst::PI*rangeKm*cnst::M_PER_KM),2.0
     );
    const double N = cnst::BOLTZMANN_CONSTANT*systemNoiseTempK*bandwidthHz;
    return bandwidthHz*std::log2(1.0+C/N);
  }

  std::array<double,3> calcSeparationVector(
   const std::array<double,3>& end, const std::array<double,3>& start
  ) {
    std::array<double,3> separationVector = {
     end.at(0)-start.at(0), end.at(1)-start.at(1), end.at(2)-start.at(2)
    };
    return separationVector;
  }

  std::array<double,3> calcSunEciPosnKm(
   const double& julianDay, const uint32_t& second, const uint32_t& nanosecond
  ) {
    const double JD =
     julianDay+(
      static_cast<double>(second)+
      static_cast<double>(nanosecond)/static_cast<double>(cnst::NS_PER_SEC)
     )/static_cast<double>(cnst::SEC_PER_DAY);
    const double n = JD-2451545.0;
    const double g_deg = 357.528+0.9856003*n;
    const double g_rad = cnst::RAD_PER_DEG*g_deg;
    const double R_au =
     1.00014-0.01671*std::cos(g_rad)-0.00014*std::cos(2.0*g_rad);
    const double L_deg = 280.460+0.9856474*n;
    const double lambda_deg =
     L_deg+1.915*std::sin(g_rad)+0.020*std::sin(2.0*g_rad);
    const double lambda_rad = cnst::RAD_PER_DEG*lambda_deg;
    const double epsilon_deg = 23.439-0.0000004*n;
    const double epsilon_rad = cnst::RAD_PER_DEG*epsilon_deg;
    std::array<double,3> sunEciPosnKm = {
     cnst::KM_PER_AU*R_au*std::cos(lambda_rad),
     cnst::KM_PER_AU*R_au*std::cos(epsilon_rad)*std::sin(lambda_rad),
     cnst::KM_PER_AU*R_au*std::sin(epsilon_rad)*std::sin(lambda_rad)
    };
    return sunEciPosnKm;
  }

  double calcAngularRadius(const double& radiusKm, const double& distanceKm) {
    return std::asin(radiusKm/distanceKm);
  }

  double dotProduct(
   const std::array<double,3>& vector1, const std::array<double,3>& vector2
  ) {
    return
     vector1.at(0)*vector2.at(0)+vector1.at(1)*vector2.at(1)+
     vector1.at(2)*vector2.at(2);
  }

  double calcAngleBetween(
   const std::array<double,3>& vector1, const std::array<double,3>& vector2
  ) {
    return std::acos(
     dotProduct(vector1,vector2)/(magnitude(vector1)*magnitude(vector2))
    );
  }

  double calcSunOcclusionFactor(
   const std::array<double,3>& satEciKm, const std::array<double,3>& sunEciKm
  ) {
    std::array<double,3> earthEciKm = {0.0,0.0,0.0};
    std::array<double,3> satToEarth = calcSeparationVector(earthEciKm,satEciKm);
    std::array<double,3> satToSun   = calcSeparationVector(sunEciKm,  satEciKm);
    double angRadiusEarth =
     calcAngularRadius(cnst::WGS_84_A,util::magnitude(satToEarth));
    double angRadiusSun =
     calcAngularRadius(cnst::SUN_RADIUS_KM,util::magnitude(satToSun));
    double angBetweenEarthSun = calcAngleBetween(satToEarth,satToSun);
    double intersectionChordLengthRadicand =
     (-1.0*angBetweenEarthSun+angRadiusSun-angRadiusEarth)*
     (-1.0*angBetweenEarthSun-angRadiusSun+angRadiusEarth)*
     (-1.0*angBetweenEarthSun+angRadiusSun+angRadiusEarth)*
     (     angBetweenEarthSun+angRadiusSun+angRadiusEarth);
    if(angBetweenEarthSun>=(angRadiusEarth+angRadiusSun)) {
      return 0.0;
    } else if(intersectionChordLengthRadicand<=0.0) {
      return 1.0;
    } else {
      return
       (
        std::pow(angRadiusSun,2.0)*std::acos(
         (std::pow(angBetweenEarthSun,2.0)+std::pow(angRadiusSun,2.0)-
          std::pow(angRadiusEarth,2.0))/(2.0*angBetweenEarthSun*angRadiusSun)
        )+
        std::pow(angRadiusEarth,2.0)*std::acos(
         (std::pow(angBetweenEarthSun,2.0)+std::pow(angRadiusEarth,2.0)-
          std::pow(angRadiusSun,2.0))/(2.0*angBetweenEarthSun*angRadiusEarth)
        )-
        0.5*std::sqrt(
         (-1.0*angBetweenEarthSun+angRadiusSun+angRadiusEarth)*
         (     angBetweenEarthSun+angRadiusSun-angRadiusEarth)*
         (     angBetweenEarthSun-angRadiusSun+angRadiusEarth)*
         (     angBetweenEarthSun+angRadiusSun+angRadiusEarth)
        )
       )/(cnst::PI*std::pow(angRadiusSun,2.0));
    }
  }

  uint64_t calcTileCountForMaxAcc(
   const std::array<double,3>& satEciPosn, const double& focalLengthM,
   const double& pixelSizeM, const double& featureSizeM,
   const uint32_t& nnTargetFeatureSizePx, const uint32_t& nnInputSizePx,
   const uint32_t& gtfHPx, const uint32_t& gtfWPx
  ) {
    const double satAltKm = cote::util::calcAltitudeKm(satEciPosn);
    const double featureSizePx =
     (focalLengthM/(pixelSizeM*satAltKm*cote::cnst::M_PER_KM))*featureSizeM;
    const uint32_t tileSizePx = static_cast<uint32_t>(std::round(
     (featureSizePx/static_cast<double>(nnTargetFeatureSizePx))*
     static_cast<double>(nnInputSizePx)
    ));
    const uint64_t tileCount = static_cast<uint64_t>(
     std::round(static_cast<double>(gtfHPx)/static_cast<double>(tileSizePx))*
     std::round(static_cast<double>(gtfWPx)/static_cast<double>(tileSizePx)));
    return tileCount;
  }

  double calcNodeVoltageDiscriminant(
   const double& chargeC, const double& capacitanceF, const double& currentA,
   const double& esrOhm, const double& powerW
  ) {
    return std::pow(chargeC/capacitanceF+currentA*esrOhm,2.0)-4.0*powerW*esrOhm;
  }

  double calcNodeVoltage(
   const double& discriminant, const double& chargeC,
   const double& capacitanceF, const double& currentA, const double& esrOhm
  ) {
    return 0.5*(chargeC/capacitanceF+currentA*esrOhm+std::sqrt(discriminant));
  }

  bool crosslinkLoSAvailable(
   const std::array<double,3>& srcSatEciPosnKm,
   const std::array<double,3>& dstSatEciPosnKm
  ) {
    const std::array<double,3> o_vec = srcSatEciPosnKm;
    const std::array<double,3> u_vec =
     calcSeparationVector(dstSatEciPosnKm,srcSatEciPosnKm);
    const double u_mag = magnitude(u_vec);
    const std::array<double,3> u_hat = {
     u_vec.at(0)/u_mag, u_vec.at(1)/u_mag, u_vec.at(2)/u_mag
    };
    const double r = cnst::WGS_72_A + 100.0;
    const double d_disc =
     std::pow(dotProduct(u_hat,o_vec),2.0)-
     std::pow(magnitude(o_vec),2.0)+
     std::pow(r,2.0);
    if(d_disc<0.0) {       // LoS does not intersect with Earth; dst sat visible
      return true;
    } else {
      const double d = -1.0*dotProduct(u_hat,o_vec)+std::sqrt(d_disc);
      if(d<0.0) {          // Earth in opposite direction from dst sat; visible
        return true;
      } else if(d<u_mag) { // d to Earth less than d to dst sat; LoS blocked
        return false;
      } else {             // d to Earth greater than d to dst sat; dst visible
        return true;
      }
    }
  }

  double calcAtmosphericLoss() {
    return 1.0;
  }

  double calcSystemNoiseTemp(
   const uint8_t& channelType, const double& centerFrequencyHz
  ) {
    // See cote/reference/wertz1999space.pdf pg. 558 Table 13-10
    double t_ant = 290.0;
    double t_line = cnst::SMAD_T0*(1.0-cnst::SMAD_LR)/cnst::SMAD_LR;
    double noiseFactor = std::pow(10.0,5.0/10.0);
    if(channelType==cnst::CHANNEL_TYPE_DOWNLINK) {
      if(centerFrequencyHz==0.2e9) {
        t_ant = 150.0;
        noiseFactor = std::pow(10.0,0.5/10.0);
      } else if(centerFrequencyHz>=2.0e9 && centerFrequencyHz<=12.0e9) {
        t_ant = 25.0;
        noiseFactor = std::pow(10.0,1.0/10.0);
      } else if(centerFrequencyHz==20.0e9) {
        t_ant = 100.0;
        noiseFactor = std::pow(10.0,3.0/10.0);
      }
    } else if(channelType==cnst::CHANNEL_TYPE_CROSSLINK) {
      t_ant = 20.0;
      noiseFactor = std::pow(10.0,5.0/10.0);
    } else if(channelType==cnst::CHANNEL_TYPE_UPLINK) {
      if(centerFrequencyHz>=0.2e9 && centerFrequencyHz<=20.0e9) {
        t_ant = 290.0;
        noiseFactor = std::pow(10.0,3.0/10.0);
      } else if(centerFrequencyHz==40.0e9) {
        t_ant = 290.0;
        noiseFactor = std::pow(10.0,4.0/10.0);
      }
    }
    double t_rx = cnst::SMAD_T0*(noiseFactor-1.0)/cnst::SMAD_LR;
    return t_ant+t_line+t_rx;
  }
}}
