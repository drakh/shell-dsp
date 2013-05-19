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

    virtual void process(float **inputs,
                         float **outputs,
                         uint32_t nframes) = 0;

  protected:
    uint32_t           ninputs_;
    uint32_t           noutputs_;
    std::string        name_;
    std::string        author_;
    std::vector<Param<float_type> > params_;
  };
}

#endif /* !SHELL_DSP_HH */
