#ifndef SHELL_DSP_HH
# define SHELL_DSP_HH

# include <vector>
# include <string>

# include "context.hh"
# include "event.hh"
# include "non-copyable.hh"

namespace shell
{
  template <typename float_type>
  class Dsp : public Context<float_type>,
              public NonCopyable
  {
  public:
    Dsp(uint32_t sample_rate)
      : Context<float_type>(sample_rate)
    {
    }

    inline virtual ~Dsp() {}

    virtual Dsp<float_type> *clone(uint32_t sample_rate) const = 0;

    virtual std::string author() const { return "None"; }
    virtual std::string name() const { return "Unnamed"; }
    virtual std::string desc() const { return ""; }

    virtual uint32_t inputCount() const = 0;
    virtual uint32_t outputCount() const = 0;

    enum ScaleType {
      kScaleLinear,
      kScaleLog,
    };

    // Parameters stuff
    virtual float paramMin(int index) const { return 0; }
    virtual float paramMax(int index) const { return 0; }
    virtual float paramScale(int index) const { return kScaleLinear; }
    virtual float paramValue(int index) const { return 0; }
    virtual void paramSetValue(int index, float value) {}
    virtual std::string paramName(int index) const { return "(no name)"; }
    virtual std::string paramDesc(int index) const { return "(no desc)"; }
    virtual std::string paramLabel(int index) const { return "(no label)"; }
    virtual uint32_t paramCount() const { return 0; }

    // DSP
    virtual void process(float_type const * const * inputs,
                         float_type * const *       outputs,
                         uint32_t                   nframes,
                         Event const *              events,
                         uint32_t                   nevents) = 0;
  };
}

#endif /* !SHELL_DSP_HH */
