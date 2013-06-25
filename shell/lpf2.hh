#ifndef SHELL_LPF2_HH
# define SHELL_LPF2_HH

# include "dsp.hh"

namespace shell
{
  template <typename float_type>
  class Lpf2 : public Dsp<float_type>
  {
  public:
    Lpf2(const Context<float_type> & ctx)
      : Dsp<float_type>(ctx),
        prev_(0),
        phase_(0),
        phase_step_(0),
        cut_(0),
        res_(0),
        grow_(0)
    {
      // cutoff
      params_[0].type_  = Param::kInteger;
      params_[0].scale_ = Param::kLog;
      params_[0].min_   = 1;
      params_[0].max_   = 22500;
      params_[0].name_  = "cutoff";
      params_[0].desc_  = "Cutoff (Hz)";
      params_[0].get_   = [this] { return this->cut_; };
      params_[0].set_   = [this] (float v) {
        this->cut_ = v;
        updateGrow();
      };
      params_[0].set_(22500);

      // resonance
      params_[1].type_  = Param::kFloat;
      params_[1].scale_ = Param::kLinear;
      params_[1].min_   = 0;
      params_[1].max_   = 1;
      params_[1].name_  = "resonance";
      params_[1].desc_  = "Resonance";
      params_[1].get_   = [this] { return this->res_; };
      params_[1].set_   = [this] (float v) {
        this->res_ = v;
        updateGrow();
      };
      params_[1].set_(0.1);
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new Lpf2<float_type>(ctx);
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "LPF2"; }
    virtual std::string desc() const override { return "Low Pass Resonant Filter"; }

    virtual uint32_t inputCount() const override { return 1; }
    virtual uint32_t outputCount() const override { return 1; }

    virtual Param & param(uint32_t index) override
    {
      return params_.at(index);
    }

    virtual uint32_t paramCount() const override
    {
      return params_.size();
    }

    virtual void step(const float_type * inputs,
                      float_type * outputs) override
    {
      outputs[0] = inputs[0] - (inputs[0] - prev_) * grow_ * std::sin(phase_);
      phase_ = std::fmod(phase_ + phase_step_, this->ctx().pi_x2_);
      prev_ = outputs[0];
      std::cout << "out: " << prev_ << std::endl;
    }

  private:
    void updateGrow()
    {
      grow_ = std::exp(-this->ctx().pi_sr_ * cut_ * res_) / std::sqrt(1 - res_ * res_);
      phase_step_ = this->ctx().pi_sr_ * cut_ * std::sqrt(1 - res_ * res_);
      std::cout << "grow: " << grow_ << ", step: " << phase_step_ << std::endl;
    }

    std::array<Param, 2> params_;
    float_type prev_;
    float_type phase_;
    float_type phase_step_;
    float_type cut_;
    float_type res_;
    float_type grow_;
  };
}

#endif /* !SHELL_LPF2_HH */
