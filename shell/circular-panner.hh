#ifndef SHELL_CIRCULAR_PANNER_HH
# define SHELL_CIRCULAR_PANNER_HH

# include "dsp.hh"

namespace shell
{
  template <typename float_type>
  class CircularPanner : public Dsp<float_type>
  {
  public:
    CircularPanner(const Context<float_type> & ctx)
      : Dsp<float_type>(ctx),
        osc_(ctx)
    {
      // freq
      params_[0].type_ = Param::kFloat;
      params_[0].scale_ = Param::kLinear;
      params_[0].min_ = 0;
      params_[0].max_ = 8;
      params_[0].name_ = "freq";
      params_[0].desc_ = "frequency";
      params_[0].get_ = [this] { return osc_.freq(); };
      params_[0].set_ = [this] (float v) { this->osc_.setFreq(v); };

      osc_.setFreq(4);
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new CircularPanner<float_type>(ctx);
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "Circular Panner"; }
    virtual std::string desc() const override { return "circular Panner"; }

    virtual uint32_t inputCount() const override { return 2; }
    virtual uint32_t outputCount() const override { return 2; }

    virtual Param & param(uint32_t index) override
    {
      if (index < params_.size())
        return params_.at(index);
      index -= params_.size();
      return osc_.param(index);
    }

    virtual uint32_t paramCount() const override
    {
      return params_.size() + osc_.paramCount();
    }

    virtual void step(const float_type * inputs,
                      float_type * outputs) override
    {
      float_type value = std::abs(osc_.step());
      outputs[0] = inputs[0] * (1 - value) + inputs[1] * value;
      outputs[1] = inputs[0] * value + inputs[1] * (1 - value);
    }

  private:
    Osc<float_type>      osc_;
    std::array<Param, 1> params_;
  };
}

#endif /* !SHELL_CIRCULAR_PANNER_HH */
