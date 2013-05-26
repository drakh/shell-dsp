#include <cstdlib>
#include <cstring>

#include <iostream>
#include <vector>
#include <memory>

#include <ladspa.h>
#include <dssi.h>

#include "dsp.hh"
#include "kicker.hh"
#include "note.hh"

#define LOG_HERE std::cerr << __PRETTY_FUNCTION__ << std::endl

namespace shell
{
  static uint32_t g_unique_id = 19042;

  template <typename float_type>
  class LadspaHandle : public NonCopyable
  {
  public:
    LadspaHandle(Dsp<float_type> * dsp)
      : dsp_(dsp)
    {
      ports_.resize(dsp->inputCount() + dsp->outputCount()
                    + 2 * dsp->paramCount());
    }

    ~LadspaHandle()
    {
      delete dsp_;
    }

    static void activate(LADSPA_Handle instance)
    {
    }

    static void deactivate(LADSPA_Handle instance)
    {
    }

    static void run(LADSPA_Handle    instance,
                    unsigned long    sample_count)
    {
      runSynth(instance, sample_count, nullptr, 0);
    }

    static void runSynth(LADSPA_Handle    instance,
                         unsigned long    sample_count,
                         snd_seq_event_t *events,
                         unsigned long    event_count)
    {
      auto thiz = reinterpret_cast<LadspaHandle<float_type> *>(instance);
      auto dsp  = thiz->dsp_;

      // copy params
      for (int i = 0; i < dsp->paramCount(); ++i) {
        int offset = dsp->inputCount() + dsp->outputCount();
        dsp->paramSetValue(i, *thiz->ports_[i * 2 + offset]);
        *thiz->ports_[i * 2 + offset + 1] = *thiz->ports_[i * 2 + offset];
      }

      float_type  data[dsp->inputCount() + dsp->outputCount()][sample_count];
      float_type *inputs[dsp->inputCount()];
      float_type *outputs[dsp->inputCount()];
      for (int i = 0; i < dsp->inputCount(); ++i) {
        inputs[i] = data[i];
        for (int j = 0; j < sample_count; ++j)
          data[i][j] = thiz->ports_[i][j];
      }
      for (int i = 0; i < dsp->outputCount(); ++i)
        outputs[i] = data[i + dsp->inputCount()];

      std::vector<Event> evs;
      Event e;
      for (int i = 0; i < event_count; ++i) {
        switch (events[i].type) {
        case SND_SEQ_EVENT_NOTE:
          break;

        case SND_SEQ_EVENT_NOTEON:
          e.type = Event::kSynthEvent;
          e.offset = events[i].time.tick;
          e.synth.freq = noteToFreq<float_type>(events[i].data.note.note);
          e.synth.velocity = events[i].data.note.velocity;
          e.synth.gate = true;
          evs.push_back(e);
          break;

        case SND_SEQ_EVENT_NOTEOFF:
          e.type           = Event::kSynthEvent;
          e.offset         = events[i].time.tick;
          e.synth.freq     = 0;
          e.synth.velocity = 0;
          e.synth.gate     = false;
          evs.push_back(e);
          break;

        case SND_SEQ_EVENT_KEYPRESS:
          break;

        default:
          std::cerr << "[SHELL] unhandled midi event: " << events[i].type
                    << std::endl;
          break;
        }
      }

      dsp->process(inputs, outputs, sample_count, &evs[0], evs.size());

      for (int i = 0; i < dsp->outputCount(); ++i)
        for (int j = 0; j < sample_count; ++j)
          thiz->ports_[i + dsp->inputCount()][j] = outputs[i][j];
    }

    static void cleanup(LADSPA_Handle instance)
    {
      delete reinterpret_cast<LadspaHandle<float_type> *>(instance);
    }

    static void connectPort(LADSPA_Handle instance,
                            unsigned long port,
                            LADSPA_Data * data_location)
    {
      auto thiz = reinterpret_cast<LadspaHandle<float_type> *>(instance);
      thiz->ports_.at(port) = data_location;
      if (!data_location ||
          port < thiz->dsp_->inputCount() + thiz->dsp_->outputCount())
        return;
      port -= thiz->dsp_->inputCount() + thiz->dsp_->outputCount();
      *data_location = thiz->dsp_->paramValue(port / 2);
    }

  private:
    Dsp<float_type> *          dsp_;
    std::vector<LADSPA_Data *> ports_;
  };

  template <typename float_type>
  class DssiDescriptor : public NonCopyable
  {
  public:
    DssiDescriptor(Dsp<float_type> * dsp)
      : dsp_(dsp)
    {
      memset(&ladspa_desc_, 0, sizeof (ladspa_desc_));
      memset(&dssi_desc_, 0, sizeof (dssi_desc_));

      ladspa_desc_.UniqueID = g_unique_id++;
      ladspa_desc_.Label = ::strdup(dsp_->name().c_str());
      ladspa_desc_.Properties = 0;
      ladspa_desc_.Name = ::strdup(dsp_->name().c_str());
      ladspa_desc_.Maker = ::strdup(dsp_->author().c_str());
      ladspa_desc_.Copyright = "None";
      ladspa_desc_.PortCount = dsp_->inputCount() + dsp_->outputCount()
        + 2 * dsp_->paramCount();
      port_descriptors_.resize(ladspa_desc_.PortCount);
      port_names_.resize(ladspa_desc_.PortCount);
      port_range_hints_.resize(ladspa_desc_.PortCount);
      for (int i = 0; i < dsp_->inputCount(); ++i) {
        port_descriptors_[i] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
        port_names_[i] = "input";
      }
      for (int i = 0; i < dsp_->outputCount(); ++i) {
        int offset = i + dsp_->inputCount();
        port_descriptors_[offset] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
        port_names_[offset] = "output";
      }
      for (int i = 0; i < 2 * dsp_->paramCount(); i += 2) {
        int offset = i + dsp_->inputCount() + dsp_->outputCount();
        port_descriptors_[offset] = LADSPA_PORT_CONTROL | LADSPA_PORT_INPUT;
        port_names_[offset] = strdup(dsp_->paramName(i / 2).c_str());
        port_range_hints_[offset].HintDescriptor = LADSPA_HINT_BOUNDED_BELOW |
          LADSPA_HINT_BOUNDED_ABOVE |
          (dsp_->paramScale(i / 2) == Dsp<float_type>::kScaleLog ?
           LADSPA_HINT_LOGARITHMIC : 0);
        port_range_hints_[offset].LowerBound = dsp_->paramMin(i / 2);
        port_range_hints_[offset].UpperBound = dsp_->paramMax(i / 2);

        port_descriptors_[offset + 1] = LADSPA_PORT_CONTROL | LADSPA_PORT_OUTPUT;
        port_names_[offset + 1] = port_names_[offset];
        port_range_hints_[offset + 1] = port_range_hints_[offset];
      }
      ladspa_desc_.PortDescriptors = &port_descriptors_[0];
      ladspa_desc_.PortNames = &port_names_[0];
      ladspa_desc_.PortRangeHints = &port_range_hints_[0];
      ladspa_desc_.ImplementationData = this;
      ladspa_desc_.instantiate = &DssiDescriptor::clone;
      ladspa_desc_.connect_port = &LadspaHandle<float_type>::connectPort;
      ladspa_desc_.activate = &LadspaHandle<float_type>::activate;
      ladspa_desc_.run = &LadspaHandle<float_type>::run;
      ladspa_desc_.run_adding = nullptr;
      ladspa_desc_.set_run_adding_gain = nullptr;
      ladspa_desc_.deactivate = &LadspaHandle<float_type>::deactivate;
      ladspa_desc_.cleanup = &LadspaHandle<float_type>::cleanup;

      dssi_desc_.DSSI_API_Version = 1;
      dssi_desc_.LADSPA_Plugin = &ladspa_desc_;
      dssi_desc_.configure = nullptr;
      dssi_desc_.get_program = nullptr;
      dssi_desc_.select_program = nullptr;
      dssi_desc_.get_midi_controller_for_port = nullptr;
      dssi_desc_.run_synth = &LadspaHandle<float_type>::runSynth;
      dssi_desc_.run_synth_adding = nullptr;
      dssi_desc_.run_multiple_synths = nullptr;
      dssi_desc_.run_multiple_synths_adding = nullptr;
    }

    ~DssiDescriptor()
    {
      free((void*)ladspa_desc_.Label);
      free((void*)ladspa_desc_.Name);
      free((void*)ladspa_desc_.Maker);
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

    static LADSPA_Handle clone(const LADSPA_Descriptor *ptr, unsigned long sample_rate)
    {
      DssiDescriptor<float_type> *desc =
        reinterpret_cast<DssiDescriptor<float_type> *>(ptr->ImplementationData);
      Dsp<float_type> *dsp = desc->dsp_->clone(sample_rate);
      if (!dsp)
        return nullptr;
      return new LadspaHandle<float_type>(dsp);
    }

  private:
    Dsp<float_type>                   *dsp_;
    std::vector<LADSPA_PortDescriptor> port_descriptors_;
    std::vector<const char *>          port_names_;
    std::vector<LADSPA_PortRangeHint>  port_range_hints_;
    LADSPA_Descriptor                  ladspa_desc_;
    DSSI_Descriptor                    dssi_desc_;
  };
}

const DSSI_Descriptor *dssi_descriptor(unsigned long index)
{
  std::vector<shell::DssiDescriptor<long double> *> plugins = {
    new shell::DssiDescriptor<long double>(new shell::Kicker<long double> (44100)),
  };

  if (index < plugins.size())
    return plugins[index]->dssiDescriptor();
  return nullptr;
}
