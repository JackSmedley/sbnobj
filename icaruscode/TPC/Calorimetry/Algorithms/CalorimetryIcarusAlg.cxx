////////////////////////////////////////////////////////////////////////
//  \file CalorimetryIcarusAlg.cxx
//
//  \brief Functions to calculate dE/dx. Based on code in Calorimetry.cxx
//
// andrzej.szelc@yale.edu
//
////////////////////////////////////////////////////////////////////////

// LArSoft includes
#include "lardataobj/RecoBase/Hit.h"
#include "icaruscode/TPC/Calorimetry/Algorithms/CalorimetryIcarusAlg.h"
#include "larevt/CalibrationDBI/Interface/ElectronLifetimeService.h"
#include "larevt/CalibrationDBI/Interface/ElectronLifetimeProvider.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "lardataalg/DetectorInfo/DetectorProperties.h"

namespace calo{

  //--------------------------------------------------------------------
  CalorimetryIcarusAlg::CalorimetryIcarusAlg(const Config& config)
  {
     this->reconfigure(config);

     detprop = art::ServiceHandle<detinfo::DetectorPropertiesService const>()->provider();
  }

  //--------------------------------------------------------------------
  CalorimetryIcarusAlg::~CalorimetryIcarusAlg()
  {

  }

  //--------------------------------------------------------------------
  void   CalorimetryIcarusAlg::reconfigure(const Config& config)
  {

    fCalAmpConstants 	= config.CalAmpConstants();
    fCalAreaConstants   = config.CalAreaConstants();
    fUseModBox          = config.CaloUseModBox();
    fLifeTimeForm       = config.CaloLifeTimeForm();
    fDoLifeTimeCorrection = config.CaloDoLifeTimeCorrection();

    return;
  }

  //------------------------------------------------------------------------------------//
  // Functions to calculate the dEdX based on the AMPLITUDE of the pulse
  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_AMP(art::Ptr< recob::Hit >  hit, double pitch, double T0) const
  {
    return dEdx_AMP(hit->PeakAmplitude()/pitch, hit->PeakTime(), hit->WireID().Plane, T0);
  }

  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_AMP(recob::Hit const&  hit, double pitch, double T0) const
  {
    return dEdx_AMP(hit.PeakAmplitude()/pitch, hit.PeakTime(), hit.WireID().Plane, T0);
  }

  ///\todo The plane argument should really be for a view instead
  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_AMP(double dQ, double time, double pitch, unsigned int plane, double T0) const
  {
    double dQdx   = dQ/pitch;           // in ADC/cm
    return dEdx_AMP(dQdx, time, plane, T0);
  }

  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_AMP(double dQdx,double time, unsigned int plane, double T0) const
  {
    double fADCtoEl=1.;

    fADCtoEl = fCalAmpConstants[plane];

    double dQdx_e = dQdx/fADCtoEl;  // Conversion from ADC/cm to e/cm
    return dEdx_from_dQdx_e(dQdx_e,time, T0);
  }

  //------------------------------------------------------------------------------------//
  // Functions to calculate the dEdX based on the AREA of the pulse
  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_AREA(art::Ptr< recob::Hit >  hit, double pitch, double T0) const
  {
    return dEdx_AREA(hit->Integral()/pitch, hit->PeakTime(), hit->WireID().Plane, T0);
  }

  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_AREA(recob::Hit const&  hit, double pitch, double T0) const
  {
    return dEdx_AREA(hit.Integral()/pitch, hit.PeakTime(), hit.WireID().Plane, T0);
  }
 //------------------------------------------------------------------------------------//
  // Functions to calculate the dEdX based on the AREA of the pulse
  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_SUMADC(art::Ptr< recob::Hit >  hit, double pitch, double T0) const
  {
    return dEdx_AREA(hit->SummedADC()/pitch, hit->PeakTime(), hit->WireID().Plane, T0);
  }

  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_SUMADC(recob::Hit const&  hit, double pitch, double T0) const
  {
    return dEdx_AREA(hit.SummedADC()/pitch, hit.PeakTime(), hit.WireID().Plane, T0);
  }

  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_AREA(double dQ,double time, double pitch, unsigned int plane, double T0) const
  {
    double dQdx   = dQ/pitch;           // in ADC/cm
    return dEdx_AREA(dQdx, time, plane, T0);
  }

  // ----------------------------------------------------------------------------------//
  double CalorimetryIcarusAlg::dEdx_AREA(double dQdx,double time, unsigned int plane, double T0) const
  {
    double fADCtoEl=1.;

    fADCtoEl = fCalAreaConstants[plane];

    double dQdx_e = dQdx/fADCtoEl;  // Conversion from ADC/cm to e/cm
    return dEdx_from_dQdx_e(dQdx_e, time, T0);
  }

  // ----------------- apply Lifetime and recombination correction.  -----------------//
  double CalorimetryIcarusAlg::dEdx_from_dQdx_e(double dQdx_e, double time, double T0) const
  {
    if (fDoLifeTimeCorrection)
      dQdx_e *= LifetimeCorrection(time, T0);   // Lifetime Correction (dQdx_e in e/cm)
    if(fUseModBox) {
      return detprop->ModBoxCorrection(dQdx_e);
    } else {
      return detprop->BirksCorrection(dQdx_e);
    }
  }


  //------------------------------------------------------------------------------------//
  // for the time being copying from Calorimetry.cxx - should be decided where to keep it.
  // ----------------------------------------------------------------------------------//
  double calo::CalorimetryIcarusAlg::LifetimeCorrection(double time, double T0) const
  {
    float t = time;

    double timetick = detprop->SamplingRate()*1.e-3;    //time sample in microsec
    double presamplings = detprop->TriggerOffset();

    t -= presamplings;
    time = t * timetick - T0*1e-3;  //  (in microsec)

    if (fLifeTimeForm==0){
      //Exponential form
      double tau = detprop->ElectronLifetime();

      double correction = exp(time/tau);
      return correction;
    }
    else if (fLifeTimeForm==1){
      //Exponential+constant form
      const lariov::ElectronLifetimeProvider& elifetime_provider = art::ServiceHandle<lariov::ElectronLifetimeService const>()->GetProvider();
      double correction = elifetime_provider.Lifetime(time);
      //std::cout<<correction<<std::endl;
      return correction;
    }
    else{
     std::cout << "Unknow CaloLifeTimeForm "<<fLifeTimeForm<<std::endl;
return 0;
    }
  }

} // namespace
