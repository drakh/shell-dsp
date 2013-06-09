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
      osc_.setFreq(1);
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
      return osc_.param(index);
    }

    virtual uint32_t paramCount() const override
    {
      return osc_.paramCount();
    }

    virtual void step(const float_type * inputs,
                      float_type * outputs) override
    {
      float_type value = std::abs(osc_.step());
      outputs[0] = inputs[0] * (1 - value) + inputs[1] * value;
      outputs[1] = inputs[0] * value + inputs[1] * (1 - value);
    }

  private:
    Osc<float_type> osc_;
  };
}

#endif /* !SHELL_CIRCULAR_PANNER_HH */
