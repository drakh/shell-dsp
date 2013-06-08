#ifndef SHELL_OSC1_HH
# define SHELL_OSC1_HH

# include "dsp.hh"
# include "adsr.hh"
# include "osc.hh"
# include "lfo.hh"

namespace shell
{
  template <typename float_type>
  class Osc1 : public Dsp<float_type>
  {
  public:

    Osc1(const Context<float_type> & ctx)
      : Dsp<float_type>(ctx),
        adsr_(ctx),
        osc_(ctx),
        lfo_(ctx)
    {
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new Osc1<float_type>(ctx);
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "Shell Osc1"; }
    virtual std::string desc() const override { return "Simple oscillator"; }

    virtual uint32_t inputCount() const override { return 0; }
    virtual uint32_t outputCount() const override { return 2; }

    virtual Param & param(uint32_t index) override
    {
      if (index < osc_.paramCount())
        return osc_.param(index);
      index -= osc_.paramCount();
      if (index < adsr_.paramCount())
        return adsr_.param(index);
      index -= adsr_.paramCount();
      return lfo_.param(index);
    }

    virtual uint32_t paramCount() const override
    {
      return osc_.paramCount() + adsr_.paramCount() + lfo_.paramCount();
    }

    virtual void noteOn(const SynthEvent & se) override
    {
      // should I reset phase? osc_.setPhase(0);
      freq_ = se.freq;
      osc_.setFreq(freq_);
      adsr_.noteOn();
    }

    virtual void noteOff(const SynthEvent &) override
    {
      adsr_.noteOff();
    }

    virtual void step(const float_type * /*inputs*/,
                      float_type * outputs) override
    {
      if (adsr_.state() == Adsr<float_type>::kIdle) {
        outputs[0] = 0;
        outputs[1] = 0;
        return;
      }

      osc_.setFreq(freq_ * lfo_.step());
      float_type wave = adsr_.step() * osc_.step();
      outputs[0] = wave;
      outputs[1] = wave;
    }

    Adsr<float_type> adsr_;     // envelope
    Osc<float_type>  osc_;      // oscillator
    Lfo<float_type>  lfo_;      // lfo
    float_type       freq_;
  };
}

#endif /* !SHELL_OSC1_HH */
