#ifndef SHELL_LPF_HH
# define SHELL_LPF_HH

# include "dsp.hh"

namespace shell
{
  template <typename float_type>
  class Lpf : public Dsp<float_type>
  {
  public:
    Lpf(const Context<float_type> & ctx)
      : Dsp<float_type>(ctx),
        prev_(0),
        cut_(0),
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
        this->grow_ = std::exp(-this->ctx().pi_sr_ * this->cut_);
      };
      params_[0].set_(22500);
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new Lpf<float_type>(ctx);
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "LPF"; }
    virtual std::string desc() const override { return "Low Pass Filter"; }

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
      outputs[0] = inputs[0] - (inputs[0] - prev_) * grow_;
      prev_ = outputs[0];
    }

  private:
    std::array<Param, 1> params_;
    float_type prev_;
    float_type cut_;
    float_type grow_;
  };
}

#endif /* !SHELL_LPF_HH */
