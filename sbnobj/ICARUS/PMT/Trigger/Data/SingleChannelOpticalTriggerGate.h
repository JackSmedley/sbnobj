/**
 * @file   sbnobj/ICARUS/PMT/Trigger/Data/SingleChannelOpticalTriggerGate.h
 * @brief  Logical multi-level gate associated to a optical detector channel.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   April 1, 2019
 * @see    `sbnobj/ICARUS/PMT/Trigger/Data/SingleChannelOpticalTriggerGate.cxx`
 * 
 */

#ifndef SBNOBJ_ICARUS_PMT_TRIGGER_DATA_SINGLECHANNELOPTICALTRIGGERGATE_H
#define SBNOBJ_ICARUS_PMT_TRIGGER_DATA_SINGLECHANNELOPTICALTRIGGERGATE_H


// ICARUS libraries
#include "sbnobj/ICARUS/PMT/Trigger/Data/OpticalTriggerGate.h"

// LArSoft libraries
#include "lardataobj/RawData/OpDetWaveform.h"

// C/C++ standard libraries
#include <iosfwd> // std::ostream


//------------------------------------------------------------------------------
// TODO move this into `lardataobj/RawData/OpDetWaveform.h`
namespace raw {
  
  /// Mnemonics for an invalid channel number.
  constexpr Channel_t InvalidChannel = std::numeric_limits<Channel_t>::max();
  
  /// Returns whether the specified one is a valid `channel` number.
  constexpr bool isValidChannel(Channel_t channel)
    { return channel != InvalidChannel; }
  
} // namespace raw


//------------------------------------------------------------------------------
namespace icarus::trigger { class SingleChannelOpticalTriggerGate; }
/**
 * @brief Logical multi-level gate associated to a optical detector channel.
 * 
 * This object is a trigger gate associated with one readout channel.
 */
class icarus::trigger::SingleChannelOpticalTriggerGate
  : public icarus::trigger::OpticalTriggerGate
{
  //
  // NOTE while there is some infrastructure allowing for the presence of more
  //      than one waveform, this class is designed with a single channel in
  //      mind.
  //
  
  using Base_t = icarus::trigger::OpticalTriggerGate;
  
    public:
  
  /// Constructor: a closed gate for the channel in `waveform`.
  SingleChannelOpticalTriggerGate(raw::OpDetWaveform const& waveform)
    : Base_t(waveform)
    {}
  
  SingleChannelOpticalTriggerGate
    (SingleChannelOpticalTriggerGate const&) = default;
  SingleChannelOpticalTriggerGate
    (SingleChannelOpticalTriggerGate&&) = default;
  SingleChannelOpticalTriggerGate& operator=
    (SingleChannelOpticalTriggerGate const&) = default;
  SingleChannelOpticalTriggerGate& operator=
    (SingleChannelOpticalTriggerGate&&) = default;
  
  /// Adds another waveform to the channel (unless it has just been added).
  bool add(raw::OpDetWaveform const& waveform);
  
  //@{
  /// Copies/steals all the levels from the specified data.
  SingleChannelOpticalTriggerGate& operator= (GateData_t const& data)
    { Base_t::operator=(data); return *this; }
  SingleChannelOpticalTriggerGate& operator= (GateData_t&& data)
    { Base_t::operator=(std::move(data)); return *this; }
  //@}
  
  
  /// Do not support multi-channel interface.
  decltype(auto) channels() const = delete;
  
  /// Comparison operator: sorts by increasing channel number.
  bool operator< (SingleChannelOpticalTriggerGate const& other) const
    { return channel() < other.channel(); }
  
    private:
  
  /// Returns the "reference" waveform, used when a single waveform is needed.
  bool hasRefWaveform() const { return !waveforms().empty(); }
  
  /// Returns the "reference" waveform, used when a single waveform is needed.
  raw::OpDetWaveform const& refWaveform() const
    { return *(waveforms().front()); }
  
  /// Returns the "reference" waveform, used when a single waveform is needed.
  raw::OpDetWaveform const* refWaveformPtr() const
    { return hasRefWaveform()? nullptr: &(refWaveform()); }
  

}; // class icarus::trigger::SingleChannelOpticalTriggerGate


#endif // SBNOBJ_ICARUS_PMT_TRIGGER_DATA_SINGLECHANNELOPTICALTRIGGERGATE_H
