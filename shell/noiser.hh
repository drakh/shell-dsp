#ifndef SHELL_NOISER_HH
# define SHELL_NOISER_HH

# include <random>

# include "dsp.hh"

namespace shell
{
  /**
   * Noise source, white, pink, red, grey.
   */
  template <typename float_type>
  class Noiser : public Dsp<float_type>
  {
  public:

    Noiser(const Context<float_type> & ctx)
      : Dsp<float_type> (ctx),
        rand_(),
        distrib_(-1, 1)
    {
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new Noiser<float_type>(ctx);
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "Shell Noiser"; }
    virtual std::string desc() const override { return "Noise generator"; }

    virtual uint32_t inputCount() const override { return 0; }
    virtual uint32_t outputCount() const override { return 1; }

    virtual void step(const float_type * inputs, float_type * outputs) override
    {
      outputs[0] = distrib_(rand_);
    }

  private:
    std::default_random_engine rand_;
    std::uniform_real_distribution<float_type> distrib_;
  };
}

#endif /* !SHELL_NOISER_HH */
