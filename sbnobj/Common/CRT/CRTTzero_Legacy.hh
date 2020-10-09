/**
 * \class CRTData
 *
 * \ingroup crt
 *
 * \brief CRT Track Info
 *
 * \author $Author: David Lorca $
 *
 * NOTE! THIS IS PURELY LEGACY HEADERS FOR THE PREV NAMESPACES! 
 * DO NOT MAKE CHANGES HERE! GO TO CRTTzero.hh
 */

#ifndef CRTtzero_LEGACY_hh_
#define CRTtzero_LEGACY_hh_

#include <cstdint>
#include <vector>
#include <map>

namespace sbnd::crt {
  
  struct CRTTzero{

    uint32_t ts0_s;
    uint16_t ts0_s_err;
    uint32_t ts0_ns;
    uint16_t ts0_ns_err;
    int32_t ts1_ns; 
    uint16_t ts1_ns_err;                    

    int nhits[7];
    
    double pes[7];
    // double xpos[7];
    // double xerr[7];
    // double ypos[7];
    // double yerr[7];
    // double zpos[7];
    // double zerr[7];
       
    CRTTzero() {}
    
  };
  
}

namespace icarus::crt {
  
  struct CRTTzero{

    uint32_t ts0_s;
    uint16_t ts0_s_err;
    uint32_t ts0_ns;
    uint16_t ts0_ns_err;
    int32_t ts1_ns; 
    uint16_t ts1_ns_err;                    

    int nhits[7];
    
    double pes[7];
    // double xpos[7];
    // double xerr[7];
    // double ypos[7];
    // double yerr[7];
    // double zpos[7];
    // double zerr[7];
       
    CRTTzero() {}
    
  };
  
}


#endif
