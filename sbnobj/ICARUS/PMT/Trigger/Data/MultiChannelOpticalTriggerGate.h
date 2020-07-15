/**
 * @file   sbnobj/ICARUS/PMT/Trigger/Data/MultiChannelOpticalTriggerGate.h
 * @brief  Logical multi-level gate associated to one or more channels.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   April 1, 2019
 * @see    `sbnobj/ICARUS/PMT/Trigger/Data/MultiChannelOpticalTriggerGate.cxx`
 * 
 */

#ifndef SBNOBJ_ICARUS_PMT_TRIGGER_DATA_MULTICHANNELOPTICALTRIGGERGATE_H
#define SBNOBJ_ICARUS_PMT_TRIGGER_DATA_MULTICHANNELOPTICALTRIGGERGATE_H


// ICARUS libraries
#include "sbnobj/ICARUS/PMT/Trigger/Data/OpticalTriggerGate.h"
#include "sbnobj/ICARUS/PMT/Trigger/Data/SingleChannelOpticalTriggerGate.h" // raw::Channel_t TODO should be moved to OpDetWaveforms

// LArSoft libraries
#include "lardataobj/RawData/OpDetWaveform.h"

// C/C++ standard libraries
#include <utility> // std::move()
#include <iosfwd> // std::ostream


namespace icarus::trigger {
  
  class SingleChannelOpticalTriggerGate; // external
  
  // ---------------------------------------------------------------------------
  //
  // declarations
  //
  
  class MultiChannelOpticalTriggerGate;
  
  MultiChannelOpticalTriggerGate sumTriggerGates
    (std::vector<SingleChannelOpticalTriggerGate> const& gates);
  
  
  // ---------------------------------------------------------------------------
  
} // namespace icarus::trigger


//------------------------------------------------------------------------------
/**
 * @brief Logical multi-level gate associated to one or more channels.
 * 
 * This object is a trigger gate associated with one or more readout channels.
 * The object contains a single trigger gate, representing the combination of
 * all involved channels.
 */
class icarus::trigger::MultiChannelOpticalTriggerGate
  : public icarus::trigger::OpticalTriggerGate
{
  
    public:
  
  using OpticalTriggerGate::OpticalTriggerGate;

  MultiChannelOpticalTriggerGate(OpticalTriggerGate const& gate)
    : OpticalTriggerGate(gate) {}
  MultiChannelOpticalTriggerGate(OpticalTriggerGate&& gate)
    : OpticalTriggerGate(std::move(gate)) {}

  using OpticalTriggerGate::operator=;
  MultiChannelOpticalTriggerGate& operator= (OpticalTriggerGate const& gate)
    { OpticalTriggerGate::operator=(gate); return *this; }
  MultiChannelOpticalTriggerGate& operator= (OpticalTriggerGate&& gate)
    { OpticalTriggerGate::operator=(std::move(gate)); return *this; }

  /// Do not support single-channel interface.
  ChannelID_t channel() const = delete;
  
}; // class icarus::trigger::MultiChannelOpticalTriggerGate


#endif // SBNOBJ_ICARUS_PMT_TRIGGER_DATA_MULTICHANNELOPTICALTRIGGERGATE_H
