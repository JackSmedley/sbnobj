// icaruscode includes
#include "icaruscode/CRT/CRTProducts/CRTHit.hh"
#include "icaruscode/CRT/CRTUtils/CRTTrueHitRecoAlg.h"

// Framework includes
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Event.h" 
#include "canvas/Persistency/Common/Ptr.h" 
#include "canvas/Persistency/Common/PtrVector.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Persistency/Common/PtrMaker.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// C++ includes
#include <memory>
#include <iostream>
#include <map>
#include <iterator>
#include <algorithm>
#include <vector>
#include <utility>

// LArSoft
#include "lardataobj/Simulation/AuxDetSimChannel.h"
#include "larcore/Geometry/Geometry.h"
#include "larcore/Geometry/AuxDetGeometry.h"
#include "larcorealg/Geometry/GeometryCore.h"
#include "larcorealg/Geometry/PlaneGeo.h"
#include "lardata/Utilities/AssociationUtil.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"

using std::vector;
using sim::AuxDetIDE;

namespace icarus {
 namespace crt {
    class CRTTrueHitProducer;
 }
}

class icarus::crt::CRTTrueHitProducer : public art::EDProducer {
public:

  explicit CRTTrueHitProducer(fhicl::ParameterSet const & p);

  // The destructor generated by the compiler is fine for classes
  // without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  CRTTrueHitProducer(CRTTrueHitProducer const &) = delete;
  CRTTrueHitProducer(CRTTrueHitProducer &&) = delete;
  CRTTrueHitProducer & operator = (CRTTrueHitProducer const &) = delete; 
  CRTTrueHitProducer & operator = (CRTTrueHitProducer &&) = delete;

  // Required functions.
  void produce(art::Event & e) override;

  // Selected optional functions.
  void reconfigure(fhicl::ParameterSet const & p);

private:

  // Params from fcl file.......
  art::InputTag fCrtModuleLabel;      ///< name of crt producer
 
  CRTTrueHitRecoAlg hitAlg;

}; // class CRTTrueHitProducer

//constructor 
icarus::crt::CRTTrueHitProducer::CRTTrueHitProducer(fhicl::ParameterSet const & p)
  : EDProducer{p}, hitAlg(p.get<fhicl::ParameterSet>("HitAlg"))
  // Initialize member data here, if know don't want to reconfigure on the fly
{
  this->reconfigure(p);

 // Call appropriate produces<>() functions here.
  produces< vector<CRTHit> >();
  produces< vector<AuxDetIDE> >();
  produces< art::Assns<CRTHit , AuxDetIDE> >();

} // CRTTrueHitProducer()

void icarus::crt::CRTTrueHitProducer::reconfigure(fhicl::ParameterSet const & p)
{
  fCrtModuleLabel = (p.get<art::InputTag> ("CrtModuleLabel")); 
} // CRTTrueHitProducer::reconfigure()

void icarus::crt::CRTTrueHitProducer::produce(art::Event & event)
{
    std::unique_ptr< vector<CRTHit> > CRTHitcol( new vector<CRTHit>);
    std::unique_ptr< vector<AuxDetIDE> > IDEcol( new vector<AuxDetIDE>);
    std::unique_ptr< art::Assns<CRTHit, AuxDetIDE> > Hitassn( new art::Assns<CRTHit,AuxDetIDE>);
    art::PtrMaker<CRTHit> makeHitPtr(event);
    art::PtrMaker<AuxDetIDE> makeAdPtr(event);
  
    int nHits = 0;
  
    // Retrieve list of CRT hits
    art::Handle< vector<sim::AuxDetSimChannel>> adscListHandle;
    vector<art::Ptr<sim::AuxDetSimChannel> > adscList;
  
    if (event.getByLabel(fCrtModuleLabel, adscListHandle))
        art::fill_ptr_vector(adscList, adscListHandle);
  
    mf::LogInfo("CRTTrueHitProducer")
      <<"Number of AuxDetSimChannels = "<<adscList.size();
  
    vector<std::pair<CRTHit, vector<AuxDetIDE>>> crtHitPairs = hitAlg.CreateCRTHits(adscList);
  
    for(auto const& crtHitPair : crtHitPairs){
  
        CRTHitcol->push_back(crtHitPair.first);
        art::Ptr<CRTHit> hitPtr = makeHitPtr(CRTHitcol->size()-1);
        nHits++;
  
        for(auto const& ide : crtHitPair.second){
  
            IDEcol->push_back(ide);
            art::Ptr<AuxDetIDE> idePtr = makeAdPtr(IDEcol->size()-1);
            Hitassn->addSingle(hitPtr, idePtr);
        }
    }
      
    event.put(std::move(CRTHitcol));
    event.put(std::move(IDEcol));
    event.put(std::move(Hitassn));
  
    mf::LogInfo("CRTTrueHitProducer")
      <<"Number of CRT hits produced = "<<nHits;
  
} // CRTTrueHitProducer::produce()

DEFINE_ART_MODULE(icarus::crt::CRTTrueHitProducer)
