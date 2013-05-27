#ifndef SHELL_OSC1_HH
# define SHELL_OSC1_HH

# include "dsp.hh"

namespace shell
{
  template <typename float_type>
  class Osc1 : public Dsp<float_type>
  {
  public:
    Osc1(uint32_t sample_rate)
      : Dsp<float_type>(sample_rate),
        phi_(0),
        attack_(this->msToStep(300)),
        sustain_(this->msToStep(3000)),
        step_(0),
        phase_(0),
        phase_step_(0),
        sustain_step_(0)
    {
    }

    virtual Dsp<float_type> *clone(uint32_t sample_rate) const override
    {
      return new Osc1<float_type>(sample_rate);
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "Shell Osc1"; }
    virtual std::string desc() const override { return "Simple oscillator"; }

    virtual uint32_t inputCount() const override { return 0; }
    virtual uint32_t outputCount() const override { return 2; }

    inline float_type processStep()
    {
      float_type attack = 1 - exp(-5 * float_type(step_) / float_type(attack_));
      float_type wave   = attack * std::sin(phase_);

      if (sustain_step_ > 0) {
        float_type sustain = exp(-5 * float_type(step_ - sustain_step_) / float_type(sustain_));
        wave *= sustain;
      }

      /* step stuff */
      phase_ += phase_step_;
      ++step_;
      return wave;
    }

    virtual void process(float_type const * const * /*inputs*/,
                         float_type * const *       outputs,
                         uint32_t                   nframes,
                         Event const *              events,
                         uint32_t                   nevents) override
    {
      uint32_t ei = 0;
      for (uint32_t i = 0; i < nframes; ++i) {
        for (; ei < nevents && events[ei].offset == i; ++ei) {
          switch (events[ei].type) {
          case Event::kSynthEvent:
            if (events[ei].synth.gate) {
              step_         = 0;
              sustain_step_ = 0;
              phase_        = phi_;
              phase_step_   = events[ei].synth.freq * this->pi_sr_;
            } else
              sustain_step_ = step_;
            break;

          case Event::kParamEvent:
            this->paramSetValue(events[ei].param.index, events[ei].param.value);
            break;
          }
        }

        auto val = processStep();
        outputs[0][i] = val;
        outputs[1][i] = val;
      }
    }

    /* params */
    float_type phi_;
    float_type attack_;
    float_type sustain_;

    /* internal */
    int64_t    step_;
    float_type phase_;
    float_type phase_step_;
    float_type sustain_step_;
  };
}

#endif /* !SHELL_OSC1_HH */
