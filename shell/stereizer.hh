#ifndef SHELL_STEREIZER_HH
# define SHELL_STEREIZER_HH

# include "dsp.hh"

namespace shell
{
  template <typename float_type>
  class Stereizer : public Dsp<float_type>
  {
  public:
    Stereizer(const Context<float_type> & ctx,
              Dsp<float_type> * dsp)
      : Dsp<float_type> (ctx),
        dsp_(dsp)
    {
      assert(dsp->outputCount() == 1);
    }

    ~Stereizer()
    {
      delete dsp_;
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new Stereizer(ctx, dsp_->clone(ctx));
    }

    virtual std::string author() const override { return dsp_->author(); }
    virtual std::string name() const override { return dsp_->name(); }
    virtual std::string desc() const override { return dsp_->desc(); }

    virtual uint32_t inputCount() const override { return dsp_->inputCount(); }
    virtual uint32_t outputCount() const override { return 2; }

    virtual Param & param(uint32_t index) override
    {
      return dsp_->param(index);
    }

    virtual uint32_t paramCount() const override
    {
      return dsp_->paramCount();
    }

    virtual void noteOn(const SynthEvent & se) override
    {
      dsp_->noteOn(se);
    }

    virtual void noteOff(const SynthEvent & se) override
    {
      dsp_->noteOff(se);
    }

    virtual void step(const float_type * inputs, float_type * outputs) override
    {
      dsp_->step(inputs, outputs);
      outputs[1] = outputs[0];
    }

  private:
    Dsp<float_type> * dsp_;
  };
}

#endif /* !SHELL_STEREIZER_HH */
