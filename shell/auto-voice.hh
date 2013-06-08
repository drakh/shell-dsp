#ifndef SHELL_AUTO_VOICE_HH
# define SHELL_AUTO_VOICE_HH

# include <queue>
# include <deque>

# include "poly-synth.hh"

namespace shell
{
  template <typename float_type>
  class AutoVoice : public Dsp<float_type>
  {
  public:
    AutoVoice(Dsp<float_type> * synth)
      : Dsp<float_type> (synth->ctx()),
        poly_(synth, 16)
    {
      for (int i = 0; i < poly_.voiceCount(); ++i)
        ready_.push(i);
    }

    virtual Dsp<float_type> *clone(const Context<float_type> & ctx) const override
    {
      return new AutoVoice<float_type>(poly_.synth(0)->clone(ctx));
    }

    virtual std::string author() const override { return poly_.author(); }
    virtual std::string name() const override { return poly_.name(); }
    virtual std::string desc() const override { return poly_.desc(); }

    virtual uint32_t inputCount() const override { return poly_.inputCount(); }
    virtual uint32_t outputCount() const override { return poly_.outputCount(); }

    virtual Param & param(uint32_t index) override { return poly_.param(index); }
    virtual uint32_t paramCount() const override { return poly_.paramCount(); }

    virtual void noteOn(const SynthEvent & se) override
    {
      // do we have available voice?
      if (ready_.empty())
        return;

      SynthEvent se2(se);
      se2.channel = ready_.front();
      ready_.pop();
      busy_.push_back(se2);
      poly_.noteOn(se2);
    }

    virtual void noteOff(const SynthEvent & se) override
    {
      for (auto it = busy_.begin(); it != busy_.end(); ++it) {
        if (it->freq != se.freq)
          continue;
        poly_.noteOff(*it);
        ready_.push(it->channel);
        busy_.erase(it);
        break;
      }
    }

    virtual void step(const float_type * inputs, float_type * outputs) override
    {
      poly_.step(inputs, outputs);
    }

  private:
    PolySynth<float_type>  poly_;
    std::queue<int>        ready_;
    std::deque<SynthEvent> busy_;
  };
}

#endif /* !SHELL_AUTO_VOICE_HH */
