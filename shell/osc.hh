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
      kSin,
      kSinc,
      kAsin,
      kPoly1,
      kPoly2,
      kSquare,
      kSawUp,
      kSawDown,
      kTri,
    };

    Osc(const Context<float_type> & ctx)
      : ctx_(ctx),
        waveform_(kSin),
        phase_(0),
        freq_(0),
        phi_(0),
        pwm_(0.5)
    {
      // waveform
      params_[0].type_ = Param::kInteger;
      params_[0].scale_ = Param::kLinear;
      params_[0].min_ = 0;
      params_[0].max_ = kTri;
      params_[0].name_ = "waveform";
      params_[0].desc_ = "waveform (sine, square, saw, tri)";
      params_[0].get_ = [this] { return this->waveform_; };
      params_[0].set_ = [this] (float v) { this->waveform_ = (Waveform)(v); };

      // phase
      params_[1].type_ = Param::kFloat;
      params_[1].scale_ = Param::kLinear;
      params_[1].min_ = -1;
      params_[1].max_ = 1;
      params_[1].name_ = "phase";
      params_[1].desc_ = "phase";
      params_[1].get_ = [this] { return this->phi_ / this->ctx_.pi_x2_; };
      params_[1].set_ = [this] (float v) { this->phi_ = this->ctx_.pi_ * v; };

      // pwm
      params_[2].type_ = Param::kFloat;
      params_[2].scale_ = Param::kLinear;
      params_[2].min_ = 0;
      params_[2].max_ = 1;
      params_[2].name_ = "pwm";
      params_[2].desc_ = "pulse width modulation";
      params_[2].get_ = [this] { return this->pwm_; };
      params_[2].set_ = [this] (float v) { this->pwm_ = v; };
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
      case kSin:
        value = std::sin(phase_);
        break;

      case kSinc: {
        auto phase2 = (phase_ - ctx_.pi_) / (pwm_ > 0 ? pwm_ : 0.001);
        value = phase2 == 0 ? 0 : std::sin(phase2) / phase2;
        break;
      }

      case kAsin:
        value = std::asin(phase_ / ctx_.pi_ - 1) / ctx_.pi_d2_;
        break;

      case kSquare:
        value = phase_ < ctx_.pi_x2_ * pwm_ ? 1 : 0;
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

      case kSawUp:
        value = phase_ / ctx_.pi_ - 1;
        break;

      case kSawDown:
        value = 1 - phase_ / ctx_.pi_;
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
    float_type                  phi_;
    float_type                  pwm_;
    std::array<Param, 3>        params_;
  };
}

#endif /* !SHELL_OSC_HH */
