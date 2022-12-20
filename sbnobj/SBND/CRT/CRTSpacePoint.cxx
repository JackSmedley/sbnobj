#ifndef SBND_CRTSPACEPOINT_CXX
#define SBND_CRTSPACEPOINT_CXX

#include "sbnobj/SBND/CRT/CRTSpacePoint.hh"

namespace sbnd {

  namespace crt {

    CRTSpacePoint::CRTSpacePoint()
      : x  (0.)
      , ex (0.)
      , y  (0.)
      , ey (0.)
      , z  (0.)
      , ez (0.)
      , pe (0.)
    {}

    CRTSpacePoint::CRTSpacePoint(double _x, double _ex, double _y, double _ey, double _z, double _ez, double _pe)
      : x  (_x)
      , ex (_ex)
      , y  (_y)
      , ey (_ey)
      , z  (_z)
      , ez (_ez)
      , pe (_pe)
    {}

    CRTSpacePoint::CRTSpacePoint(TVector3 _pos, TVector3 _err, double _pe)
      : x  (_pos.X())
      , ex (_err.X())
      , y  (_pos.Y())
      , ey (_err.Y())
      , z  (_pos.Z())
      , ez (_err.Z())
      , pe (_pe)
    {}

    CRTSpacePoint::~CRTSpacePoint() {}

    double CRTSpacePoint::X() const { return x; }
    double CRTSpacePoint::XErr() const { return ex; }
    double CRTSpacePoint::Y() const { return y; }
    double CRTSpacePoint::YErr() const { return ey; }
    double CRTSpacePoint::Z() const { return z; }
    double CRTSpacePoint::ZErr() const { return ez; }
    double CRTSpacePoint::PE() const { return pe; }
  }
}

#endif