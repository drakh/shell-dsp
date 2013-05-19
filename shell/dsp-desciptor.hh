#ifndef SHELL_DSP_DESCIPTOR_HH
# define SHELL_DSP_DESCIPTOR_HH

namespace shell
{
  class Dsp;

  class DspDescriptor
  {
  public:
    const std::string & name() const { return name_; }
    const std::string & author() const { return author_; }
    const std::vector<Param> & params() const { return params_; };

    uint32_t nInputs() const { return 0; }
    uint32_t nOutputs() const { return 0; }

    virtual Dsp * create() const;

  private:
    uint32_t ninputs_;
    uint32_t noutputs_;
    std::string name_;
    std::string author_;
    std::vector<Param> params_;
  };
}

#endif /* !SHELL_DSP_DESCIPTOR_HH */
