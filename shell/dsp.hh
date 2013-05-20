#ifndef SHELL_DSP_HH
# define SHELL_DSP_HH

# include <vector>

# include "context.hh"
# include "param.hh"

namespace shell
{
  template <typename float_type>
  class Dsp : public Context<float_type>
  {
  public:
    Dsp(uint32_t sample_rate)
      : Context<float_type>(sample_rate),
        ninputs_(0),
        noutputs_(0)
    {
    }

    inline virtual ~Dsp() {}

    virtual Dsp<float_type> *clone(uint32_t sample_rate) const = 0;

    virtual void process(float_type **inputs,
                         float_type **outputs,
                         uint32_t     nframes) = 0;

    uint32_t nInputs() const { return ninputs_; }
    uint32_t nOutputs() const { return noutputs_; }
    const std::vector<Param<float_type> > params() const { return params_; };
    const std::string & name() const { return name_; }
    const std::string & author() const { return author_; }

  protected:
    uint32_t    ninputs_;
    uint32_t    noutputs_;
    std::string name_;
    std::string author_;
    std::vector<Param<float_type> > params_;
  };
}

#endif /* !SHELL_DSP_HH */
