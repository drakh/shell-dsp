#include <vector>

#include <ladspa.h>
#include <dssi.h>

#include "kicker.hh"

namespace shell
{
  class DssiDescriptor
  {
  public:
    DssiDescriptor(Dsp * dsp)
    {
      ladspa_desc_.UniqueId = unique_id_++;
      ladspa_desc_.Label = strdup(dsp_->name().c_str());
      ladspa_desc_.Properties = 0;
      ladspa_desc_.Name = strdup(dsp_->name().c_str());
      ladspa_desc_.Maker = strdup(dsp_->author().c_str());
      ladspa_desc_.Maker = "None";
      ladspa_desc_.PortCount = dsp_->nInputs() + dsp_->nOutputs();// + dsp_->params().size();
      port_descriptors_.resize(ladspa_desc_.PortCount);
      port_names_.resize(ladspa_desc_.PortCount);
      port_port_range_hint_.resize(ladspa_desc_.PortCount);
      for (int i = 0; i < dsp_->nInputs(); ++i) {
      }
      for (int i = 0; i < dsp_->nOutputs(); ++i) {
      }
      // for (int i = 0; i < dsp_->params().size(); ++i) {
      // }
      ladspa_desc_.PortDescriptors = &port_descritpors_[0];
      ladspa_desc_.instantiate = &DssiDescriptor::clone;
      ladspa_desc_.connect_port = nullptr;
      ladspa_desc_.run = nullptr;
      ladspa_desc_.run_adding = nullptr;

      dssi_desc_.DSSI_API_Version = 1;
      dssi_desc_.LADSPA_PLugin = &ladspa_desc_;
      dssi_desc_.configure = nullptr;
      dssi_desc_.get_program = nullptr;
      dssi_desc_.select_program = nullptr;
      dssi_desc_.get_midi_controller_for_port = nullptr;
      dssi_desc_.run_synth = nullptr;
      dssi_desc_.run_synth_adding = nullptr;
      dssi_desc_.run_multiple_synths = nullptr;
      dssi_desc_.run_multiple_synths_adding = nullptr;
    }

    /* ladspa stuff */
    LADSPA_Descriptor *ladspaDescriptor() const
    {
      return &ladspa_desc_;
    }

    /* dssi stuff */
    DSSI_Descriptor *dssiDescriptor() const
    {
      return &dssi_desc_;
    }

    static LADSPA_Handle *clone(LADSPA_Hanle handle, unsigned long sample_rate)
    {
      DssiDescriptor *desc = handle;
      Dsp *dsp = handle->dsp_->clone(sample_rate);
      if (!dsp)
        return nullptr;
      return new DssiDescriptor(dsp);
    }

  private:
    static uint32_t unique_id_ = 19042;

    Dsp                               *dsp_;
    std::vector<LADSPA_PortDescriptor> port_descriptors_;
    std::vector<const char *>          port_names_;
    std::vector<LADSPA_PortRangeHint>  port_range_hint_;
    LADSPA_Descriptor                  ladspa_desc_;
    DSSI_Descriptor                    dssi_desc_;
  };
}

// static char *dssi_configure(LADSPA_Handle  ctx,
//                             const char    *key,
//                             const char    *value)
// {
//   return nullptr;
// }

static 

static const LADSPA_Descriptor g_ladspa_descriptors[] = {
  {
    19042, // UniqueID
    "shell-kicker", // Label
    {}, // properties
    "Shell Kicker", // Name
    "Alexandre BIQUE", // Maker
    "None", // Copyright
    0, // PortCount
    {}, // PortDescriptors
    {}, // PortNames
    {}, // PortRangesHints
    NULL, // ImplementationData
    NULL, // instantiate
    
  },
};

// static const DSSI_Descriptor g_dssi_descriptors[] = {
//   {
//     1, // api version
//     g_ladspa_descriptors + 0,
    
//   },
// };

const DSSI_Descriptor *dssi_descriptor(unsigned long index)
{
  static std::vector<shell::Dssi> plugins;

  if (index < plugins.size())
    return plugins[i].dssiDescriptor();
  return nullptr;
}
