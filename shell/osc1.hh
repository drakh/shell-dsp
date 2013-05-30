#ifndef SHELL_OSC1_HH
# define SHELL_OSC1_HH

# include "dsp.hh"
# include "adsr.hh"

namespace shell
{
  template <typename float_type>
  class Osc1 : public Dsp<float_type>
  {
  public:

    Osc1(const Context<float_type> & ctx)
      : Dsp<float_type>(),
        ctx_(ctx),
        adsr_(ctx_),
        phi_(0),
        step_(0),
        phase_(0),
        phase_step_(0)
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
      return adsr_.param(index);
    }

    virtual uint32_t paramCount() const override
    {
      return adsr_.paramCount();
    }

    virtual void noteOn(const SynthEvent & se) override
    {
      std::cout << "note on" << std::endl;
      phase_step_ = se.freq * ctx_.pi_sr_;
      adsr_.noteOn();
    }

    virtual void noteOff(const SynthEvent &) override
    {
      std::cout << "note off" << std::endl;
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

      float_type wave = adsr_.step() * std::sin(phase_ + phi_);
      outputs[0] = wave;
      outputs[1] = wave;

      /* step stuff */
      phase_ += phase_step_;
      ++step_;
    }

    const Context<float_type> & ctx_;
    Adsr<float_type>            adsr_; // envelope

    /* params */
    float_type phi_;

    /* internal */
    int32_t    step_;
    float_type phase_;
    float_type phase_step_; // the amount to add to phase for each step
  };
}

#endif /* !SHELL_OSC1_HH */
