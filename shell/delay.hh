#ifndef SHELL_DELAY_HH
# define SHELL_DELAY_HH

# include "dsp.hh"

namespace shell
{
  template <typename float_type>
  class Delay : public Dsp<float_type>
  {
  public:
    Delay(const Context<float_type> & ctx)
      : Dsp<float_type> (ctx)
    {
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new Delay<float_type>(ctx);
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "Delay"; }
    virtual std::string desc() const override { return "Delay"; }

    virtual uint32_t inputCount() const override { return 1; }
    virtual uint32_t outputCount() const override { return 1; }

    // Parameters stuff
    virtual Param & param(uint32_t index) override
    {
      return params_.at(index);
    }

    virtual uint32_t paramCount() const override
    {
      return params_.size();
    }

    // process one frame
    virtual void step(const float_type * inputs, float_type * outputs)
    {
      
    }

  private:
    uint32_t delay_;
    std::vector<float_type> buffer_; // XXX rotate buffer
    std::array<Param, 2> params_;
  };
}

#endif /* !SHELL_DELAY_HH */
