#ifndef SHELL_AUTO_VOICE_HH
# define SHELL_AUTO_VOICE_HH

# include "poly-synth.hh"

namespace shell
{
  template <typename float_type>
  class AutoVoice : public Dsp<float_type>
  {
  public:
    AutoVoice(Dsp<float_type> * synth)
      : Dsp<float_type> (synth->ctx()),
        poly_(synth, 16)
    {
      evs_.resize(poly_.voiceCount());
      for (auto it = evs_.begin(); it != evs_.end(); ++it)
        it->freq = 0;
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new AutoVoice<float_type>(poly_.synth(0)->clone(ctx));
    }

    virtual std::string author() const override { return poly_.author(); }
    virtual std::string name() const override { return poly_.name(); }
    virtual std::string desc() const override { return poly_.desc(); }

    virtual uint32_t inputCount() const override { return poly_.inputCount(); }
    virtual uint32_t outputCount() const override { return poly_.outputCount(); }

    virtual Param & param(uint32_t index) override { return poly_.param(index); }
    virtual uint32_t paramCount() const override { return poly_.paramCount(); }

    virtual void noteOn(const SynthEvent & se) override
    {
      std::cout << "auto on: " << se.freq << std::endl;
      for (auto it = evs_.begin(); it != evs_.end(); ++it) {
        if (it->freq != 0)
          continue;
        *it = se;
        it->channel = it - evs_.begin();
        poly_.noteOn(*it);
        break;
      }
    }

    virtual void noteOff(const SynthEvent & se) override
    {
      std::cout << "auto off: " << se.freq << std::endl;
      for (auto it = evs_.begin(); it != evs_.end(); ++it) {
        if (it->freq != se.freq)
          continue;
        *it = se;
        it->channel = it - evs_.begin();
        poly_.noteOff(*it);
        it->freq = 0;
        break;
      }
    }

    virtual void step(const float_type * inputs, float_type * outputs) override
    {
      poly_.step(inputs, outputs);
    }

  private:
    PolySynth<float_type>   poly_;
    std::vector<SynthEvent> evs_;
  };
}

#endif /* !SHELL_AUTO_VOICE_HH */
