#ifndef SHELL_KICKER_HH
# define SHELL_KICKER_HH

# include <cstdint>
# include <cmath>

# include "dsp.hh"

namespace shell
{
  template <typename float_type>
  class Kicker : public Dsp<float_type>
  {
  public:
    Kicker(uint32_t sample_rate)
      : Dsp<float_type>(sample_rate),
        duration_(sample_rate / 4),
        step_(0),
        phi_(0),
        f1_(700),
        f2_(0),
        wave_type_(0)
    {
      this->name_     = "shell kicker";
      this->author_   = "Alexandre BIQUE";
      this->ninputs_  = 0;
      this->noutputs_ = 0;

      this->params_.push_back(
        Param<float_type>("f1", "initial freq",
                          [&] { return this->f1_; },
                          [&] (float_type v) { this->f1_ = v; },
                          0, 24000));
      // params_.push_back(Param("f2", "final freq", &f2_));
      // params_.push_back(Param("duration", "duration (ms)", &duration_));
    }

    virtual Dsp<float_type> *clone(uint32_t sample_rate) override
    {
      return new Kicker<float_type>(sample_rate);
    }

    void start()
    {
      step_ = 0;
    }

    inline float_type processStep()
    {
      if (step_ > duration_)
        return 0;

      float_type freq = f1_ + (f2_ - f1_) *
        (1 - std::exp(-((float_type(5 * step_)) / float_type(duration_))));
      float_type phase = phi_ + step_ * freq * 2 * this->pi_ / float_type(this->sr_);
      float_type wave = std::sin(phase);
      float_type shape = exp(-float_type(step_) / float_type(duration_ * 5));
      ++step_;

      return wave * shape;
    }

    virtual void process(float_type **inputs,
                         float_type **outputs,
                         uint32_t     nframes) override
    {
      for (uint32_t i = 0; i < nframes; ++i)
        outputs[0][i] = processStep();
    }

  private:
    uint32_t   duration_;        // in step
    uint32_t   step_;            // the step since start();
    float_type phi_;
    float_type f1_;
    float_type f2_;
    uint32_t   wave_type_;
  };
}

#endif /* !KICKER_HH */
