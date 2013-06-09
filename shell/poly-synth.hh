#ifndef SHELL_POLY_SYNTH_HH
# define SHELL_POLY_SYNTH_HH

# include <cstdint>
# include <vector>
# include <memory>

# include "dsp.hh"

namespace shell
{
  /**
   * This class helps to transform a monophonic synth,
   * into a polyphonic synth.
   */
  template <typename float_type>
  class PolySynth : public Dsp<float_type>
  {
  public:
    PolySynth(Dsp<float_type> * synth,
              uint32_t voice_count = 1)
      : Dsp<float_type>(synth->ctx())
    {
      if (voice_count < 1)
        voice_count = 1;

      synths_.resize(voice_count);
      synths_[0].reset(synth);
      for (int i = 1; i < voice_count; ++i)
        synths_[i].reset(synth->clone(this->ctx_));

      /* init params */
      params_.resize(synth->paramCount());
      for (int i = 0; i < synth->paramCount(); ++i) {
        const Param & in  = synth->param(i);
        Param &       out = params_[i];

        out = in;
        out.set_ = [this, i] (float value) {
          for (auto it = synths_.begin(); it != synths_.end(); ++it)
            (*it)->param(i).set_(value);
        };
      }
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new PolySynth<float_type>(synths_[0]->clone(ctx), synths_.size());
    }

    uint32_t voiceCount() const { return synths_.size(); }

    void setVoiceCount(uint32_t nb)
    {
      if (nb < 1)
        nb = 1;

      if (nb < synths_.size())
        synths_.resize(nb);
      else {
        synths_.resize(nb);
        for (--nb; nb < synths_.size(); ++nb)
          synths_[nb].reset(synths_[0]->clone());
      }
    }

    Dsp<float_type> * synth(uint32_t index) const { return synths_.at(index).get(); }

    virtual std::string author() const override { return synths_[0]->author(); }
    virtual std::string name() const override { return synths_[0]->name(); }
    virtual std::string desc() const override { return synths_[0]->desc(); }

    virtual uint32_t inputCount() const override { return synths_[0]->inputCount(); }
    virtual uint32_t outputCount() const override { return synths_[0]->outputCount(); }

    virtual Param & param(uint32_t index) override { return params_.at(index); }
    virtual uint32_t paramCount() const override { return params_.size(); }

    virtual void noteOn(const SynthEvent & se) override
    {
      if (se.channel >= synths_.size())
        return;
      synths_[se.channel]->noteOn(se);
    }

    virtual void noteOff(const SynthEvent & se) override
    {
      if (se.channel >= synths_.size())
        return;
      synths_[se.channel]->noteOff(se);
    }

    virtual void step(const float_type * inputs, float_type * outputs) override
    {
      float_type out[outputCount()];

      for (int i = 0; i < outputCount(); ++i)
        outputs[i] = 0;

      for (auto it = synths_.begin(); it != synths_.end(); ++it) {
        (*it)->step(inputs, out);
        for (int i = 0; i < outputCount(); ++i)
          outputs[i] += out[i];
      }
    }

  private:
    std::vector<Param> params_;
    std::vector<std::unique_ptr<Dsp<float_type> > > synths_;
  };
}

#endif /* !SHELL_POLY_SYNTH_HH */
