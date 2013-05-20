#include <vector>

#include <ladspa.h>
#include <dssi.h>

#include "kicker.hh"

namespace shell
{
  static uint32_t g_unique_id = 19042;

  template <typename float_type>
  class DssiDescriptor
  {
  public:
    DssiDescriptor(Dsp<float_type> * dsp)
    {
      ladspa_desc_.UniqueID = g_unique_id++;
      ladspa_desc_.Label = strdup(dsp_->name().c_str());
      ladspa_desc_.Properties = 0;
      ladspa_desc_.Name = strdup(dsp_->name().c_str());
      ladspa_desc_.Maker = strdup(dsp_->author().c_str());
      ladspa_desc_.Maker = "None";
      ladspa_desc_.PortCount = dsp_->nInputs() + dsp_->nOutputs();// + dsp_->params().size();
      port_descriptors_.resize(ladspa_desc_.PortCount);
      port_names_.resize(ladspa_desc_.PortCount);
      port_range_hint_.resize(ladspa_desc_.PortCount);
      for (int i = 0; i < dsp_->nInputs(); ++i) {
      }
      for (int i = 0; i < dsp_->nOutputs(); ++i) {
      }
      // for (int i = 0; i < dsp_->params().size(); ++i) {
      // }
      ladspa_desc_.PortDescriptors = &port_descriptors_[0];
      ladspa_desc_.instantiate = &DssiDescriptor::clone;
      ladspa_desc_.connect_port = nullptr;
      ladspa_desc_.run = nullptr;
      ladspa_desc_.run_adding = nullptr;

      dssi_desc_.DSSI_API_Version = 1;
      dssi_desc_.LADSPA_Plugin = &ladspa_desc_;
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
    const DSSI_Descriptor *dssiDescriptor() const
    {
      return &dssi_desc_;
    }

    static LADSPA_Handle clone(LADSPA_Descriptor *ptr, unsigned long sample_rate)
    {
      DssiDescriptor<float_type> *desc =
        reinterpret_cast<DssiDescriptor<float_type> *>(ptr->ImplementationData);
      Dsp<float_type> *dsp = desc->dsp_->clone(sample_rate);
      if (!dsp)
        return nullptr;
      return new DssiDescriptor<float_type>(dsp);
    }

  private:
    Dsp<float_type>                   *dsp_;
    std::vector<LADSPA_PortDescriptor> port_descriptors_;
    std::vector<const char *>          port_names_;
    std::vector<LADSPA_PortRangeHint>  port_range_hint_;
    LADSPA_Descriptor                  ladspa_desc_;
    DSSI_Descriptor                    dssi_desc_;
  };
}

const DSSI_Descriptor *dssi_descriptor(unsigned long index)
{
  static std::vector<shell::DssiDescriptor<long double> > plugins;

  if (index < plugins.size())
    return plugins[index].dssiDescriptor();
  return nullptr;
}
