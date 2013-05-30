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

    Kicker(const Context<float_type> & ctx)
      : Dsp<float_type>(),
        ctx_(ctx),
        decay_(ctx_.sr_ / 4),
        step_(decay_ + 1),
        f1_(700),
        f2_(0),
        wave_type_(0)
    {
      // f1
      params_[0].type_ = Param::kInteger;
      params_[0].scale_ = Param::kLinear;
      params_[0].min_ = 0;
      params_[0].max_ = 2000;
      params_[0].name_ = "f1";
      params_[0].desc_ = "frequency (hz)";
      params_[0].get_ = [this] { return this->ctx_.stepToMs(this->f1_); };
      params_[0].set_ = [this] (float v) { this->f1_ = v; };

      // f2
      params_[1].type_ = Param::kInteger;
      params_[1].scale_ = Param::kLinear;
      params_[1].min_ = 0;
      params_[1].max_ = 2000;
      params_[1].name_ = "f2";
      params_[1].desc_ = "frequency (hz)";
      params_[1].get_ = [this] { return this->ctx_.stepToMs(this->f2_); };
      params_[1].set_ = [this] (float v) { this->f2_ = v; };

      // decay
      params_[2].type_ = Param::kInteger;
      params_[2].scale_ = Param::kLinear;
      params_[2].min_ = 0;
      params_[2].max_ = 2000;
      params_[2].name_ = "decay";
      params_[2].desc_ = "duration (ms)";
      params_[2].get_ = [this] { return this->ctx_.stepToMs(this->decay_); };
      params_[2].set_ = [this] (float v) { this->decay_ = this->ctx_.msToStep(v); };
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new Kicker<float_type>(ctx);
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "Shell Kicker"; }
    virtual std::string desc() const override { return "Kicker"; }

    virtual uint32_t inputCount() const override { return 0; }
    virtual uint32_t outputCount() const override { return 2; }

    virtual Param & param(uint32_t index) override
    {
      if (index >= paramCount())
        throw std::out_of_range("");
      return params_[index];
    }

    virtual uint32_t paramCount() const override
    {
      return sizeof (params_) / sizeof (*params_);
    }

    void noteOn(const SynthEvent & se) override
    {
      step_  = 0;
      phase_ = 0;
    }

    inline void step(const float_type * /*input*/,
                     float_type * output) override
    {
      if (step_ > decay_) {
        output[0] = 0;
        output[1] = 0;
        return;
      }

      float_type freq = f1_ + (f2_ - f1_) *
        (1 - std::exp((-5 * float_type(step_)) / float_type(decay_)));
      float_type wave = std::sin(phase_);
      float_type shape = exp(-5 * float_type(step_) / float_type(decay_));
      float_type out =  wave * shape;

      output[0] = out;
      output[1] = out;
      ++step_;
      phase_ += freq * ctx_.pi_sr_;
    }

  private:
    const Context<float_type> & ctx_;
    uint32_t                    decay_; // in step
    uint32_t                    step_; // the step since start();
    float_type                  f1_;
    float_type                  f2_;
    float_type                  phase_;
    uint32_t                    wave_type_;
    Param                       params_[3];
  };
}

#endif /* !KICKER_HH */
