#ifndef SHELL_BINAURAL_HH
# define SHELL_BINAURAL_HH

# include "osc1.hh"

namespace shell
{
  template <typename float_type>
  class Binaural : public Dsp<float_type>
  {
  public:
    Binaural(const Context<float_type> & ctx,
             Dsp<float_type> * left,
             Dsp<float_type> * right)
      : Dsp<float_type>(ctx),
        left_(left),
        right_(right),
        left_params_(*left_, "left:"),
        right_params_(*right_, "right:")
    {
      assert(left->inputCount() == 0);
      assert(right->inputCount() == 0);
      assert(left->outputCount() == 1);
      assert(right->outputCount() == 1);
    }

    ~Binaural()
    {
      delete left_;
      delete right_;
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new Binaural<float_type>(ctx, left_->clone(ctx), right_->clone(ctx));
    }

    virtual std::string author() const override { return "Alexandre BIQUE"; }
    virtual std::string name() const override { return "Shell Binaural"; }
    virtual std::string desc() const override { return "Simple binaural"; }

    virtual uint32_t inputCount() const override { return 0; }
    virtual uint32_t outputCount() const override { return 2; }

    virtual Param & param(uint32_t index) override
    {
      if (index < left_params_.paramCount())
        return left_params_.param(index);
      index -= left_params_.paramCount();
      return right_params_.param(index);
    }

    virtual uint32_t paramCount() const override
    {
      return left_params_.paramCount() + right_params_.paramCount();
    }

    virtual void noteOn(const SynthEvent & se) override
    {
      // should I reset phase? osc_.setPhase(0);
      left_->noteOn(se);
      right_->noteOn(se);
    }

    virtual void noteOff(const SynthEvent & se) override
    {
      left_->noteOff(se);
      right_->noteOff(se);
    }

    virtual void step(const float_type * /*inputs*/,
                      float_type * outputs) override
    {
      left_->step(nullptr, outputs + 0);
      right_->step(nullptr, outputs + 1);
    }

  private:
    Dsp<float_type> * left_;
    Dsp<float_type> * right_;
    PrefixParams      left_params_;
    PrefixParams      right_params_;
  };
}

#endif /* !SHELL_BINAURAL_HH */
