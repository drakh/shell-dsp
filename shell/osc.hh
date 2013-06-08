#ifndef SHELL_OSC_HH
# define SHELL_OSC_HH

# include <cmath>
# include <cassert>

# include "context.hh"
# include "param.hh"

namespace shell
{
  template <typename float_type>
  class Osc
  {
  public:

    enum Waveform {
      kSine,
      kSine2,
      kPoly1,
      kPoly2,
      kSquare,
      kSaw,
      kTri,
    };

    Osc(const Context<float_type> & ctx)
      : ctx_(ctx),
        waveform_(kSine),
        phase_(0),
        freq_(0)
    {
      // f1
      params_[0].type_ = Param::kInteger;
      params_[0].scale_ = Param::kLinear;
      params_[0].min_ = 0;
      params_[0].max_ = kTri;
      params_[0].name_ = "waveform";
      params_[0].desc_ = "waveform (sine, square, saw, tri)";
      params_[0].get_ = [this] { return this->ctx_.stepToMs(this->waveform_); };
      params_[0].set_ = [this] (float v) { this->waveform_ = (Waveform)(v); };
    }

    Param & param(uint32_t index)
    {
      return params_.at(index);
    }

    uint32_t paramCount() const
    {
      return params_.size();
    }

    float_type freq() const
    {
      return freq_;
    }

    void setFreq(float_type freq)
    {
      freq_ = freq;
    }

    void setPhase(float_type phase)
    {
      phase_ = phase;
    }

    float_type step()
    {
      float_type value;

      // ensure phase is in [0..2*pi[
      phase_ = std::fmod(phase_, ctx_.pi_x2_);

      switch (waveform_) {
      case kSine:
        value = std::sin(phase_);
        break;

      case kSine2:
        value = std::sin(phase_);
        value *= std::abs(value);
        break;

      case kSquare:
        value = phase_ < ctx_.pi_ ? 1 : 0;
        break;

      case kPoly1:
        value = phase_ / ctx_.pi_x2_;
        value = 1 - value * value;
        value = 2 * value - 1;
        break;

      case kPoly2:
        value = phase_ / ctx_.pi_ - 1;
        value = 1 - value * value;
        value = 2 * value - 1;
        break;

      case kSaw:
        value = phase_ / ctx_.pi_ - 1;
        break;

      case kTri:
        value = std::abs(phase_ / ctx_.pi_d2_ - 2) - 1;
        break;

      default:
        value = 0;
        break;
      }

      phase_ += freq_ * ctx_.pi_sr_;
      return value;
    }

  private:
    const Context<float_type> & ctx_;
    Waveform                    waveform_;
    float_type                  phase_;
    float_type                  freq_;
    std::array<Param, 1>        params_;
  };
}

#endif /* !SHELL_OSC_HH */
