#ifndef SHELL_DSP_HH
# define SHELL_DSP_HH

# include <vector>
# include <string>
# include <stdexcept>

# include "context.hh"
# include "event.hh"
# include "non-copyable.hh"
# include "param.hh"

namespace shell
{
  template <typename float_type>
  class Dsp : public NonCopyable
  {
  public:
    Dsp(const Context<float_type> & ctx)
      : ctx_(ctx)
    {
    }

    virtual ~Dsp() {}

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const = 0;

    const Context<float_type> & ctx() const { return ctx_; }

    virtual std::string author() const { return "None"; }
    virtual std::string name() const { return "Unnamed"; }
    virtual std::string desc() const { return ""; }

    virtual uint32_t inputCount() const { return 0; }
    virtual uint32_t outputCount() const { return 0; }

    // Parameters stuff
    virtual Param & param(uint32_t /*index*/) { throw std::out_of_range(""); }
    virtual uint32_t paramCount() const { return 0; }

    virtual void noteOn(const SynthEvent &) {}
    virtual void noteOff(const SynthEvent &) {}

    // process one frame
    virtual void step(const float_type * inputs, float_type * outputs) = 0 ;

  protected:
    const Context<float_type> & ctx_;
  };
}

#endif /* !SHELL_DSP_HH */
