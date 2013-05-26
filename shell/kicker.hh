#ifndef SHELL_KICKER_HH
# define SHELL_KICKER_HH

# include <cstdint>
# include <cmath>

# include "dsp.hh"

namespace shell
{
  template <typename float_type>
  class Kicker : public Dsp<float_type>
  {
  public:
    enum Params {
      kParamF1,
      kParamF2,
      kParamDuration,
      kParamsCount,
    };

    Kicker(uint32_t sample_rate)
      : Dsp<float_type>(sample_rate),
        duration_(sample_rate / 4),
        step_(duration_ + 1),
        phi_(0),
        f1_(700),
        f2_(0),
        wave_type_(0)
    {
    }

    virtual Dsp<float_type> *clone(uint32_t sample_rate) const override
    {
      return new Kicker<float_type>(sample_rate);
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "Shell Kicker"; }
    virtual std::string desc() const override { return "Kicker"; }

    virtual uint32_t inputCount() const override { return 0; }
    virtual uint32_t outputCount() const override { return 2; }

    virtual float paramMin(int index) const override
    {
      switch (index) {
      case kParamF1:
        return 0;

      case kParamF2:
        return 0;

      case kParamDuration:
        return 25;

      default:
        return 0;
      }
    }

    virtual float paramMax(int index) const override
    {
      switch (index) {
      case kParamF1:
        return 12000;

      case kParamF2:
        return 12000;

      case kParamDuration:
        return 2500;

      default:
        return 0;
      }
    }

    virtual float paramScale(int index) const override
    {
      switch (index) {
      case kParamF1:
        return Dsp<float_type>::kScaleLog;

      case kParamF2:
        return Dsp<float_type>::kScaleLog;

      case kParamDuration:
        return Dsp<float_type>::kScaleLinear;

      default:
        return Dsp<float_type>::kScaleLinear;
      }
    }

    virtual float paramValue(int index) const override
    {
      switch (index) {
      case kParamF1:
        return f1_;

      case kParamF2:
        return f2_;

      case kParamDuration:
        return this->stepToMs(duration_);

      default:
        return 0;
      }
    }

    virtual void paramSetValue(int index, float value) override
    {
      switch (index) {
      case kParamF1:
        f1_ = value;

      case kParamF2:
        f2_ = value;

      case kParamDuration:
        duration_ = this->msToStep(value);
      }
    }

    virtual std::string paramName(int index) const override
    {
      switch (index) {
      case kParamF1:
        return "f1";

      case kParamF2:
        return "f2";

      case kParamDuration:
        return "duration";

      default:
        return "(no parameter)";
      }
    }

    virtual std::string paramDesc(int index) const { return paramName(index); }
    virtual uint32_t paramCount() const override { return kParamsCount; }

    void start()
    {
      step_ = 0;
    }

    inline float_type processStep()
    {
      if (step_ > duration_)
        return 0;

      float_type freq = f1_ + (f2_ - f1_) *
        (1 - std::exp(-((float_type(5 * step_)) / float_type(duration_))));
      float_type phase = phi_ + step_ * freq * this->pi_sr_;
      float_type wave = std::sin(phase);
      float_type shape = exp(-5 * float_type(step_) / float_type(duration_));
      ++step_;

      return wave * shape;
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
            if (events[ei].synth.gate)
              start();
            break;

          case Event::kParamEvent:
            paramSetValue(events[ei].param.index, events[ei].param.value);
            break;
          }
        }

        auto val = processStep();
        outputs[0][i] = val;
        outputs[1][i] = val;
      }
    }

  private:
    uint32_t   duration_;        // in step
    uint32_t   step_;            // the step since start();
    float_type phi_;
    float_type f1_;
    float_type f2_;
    uint32_t   wave_type_;
  };
}

#endif /* !KICKER_HH */
