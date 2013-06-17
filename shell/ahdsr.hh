#ifndef SHELL_AHDSR_HH
# define SHELL_AHDSR_HH

# include <cmath>
# include <cstdint>
# include <stdexcept>
# include <array>

# include "context.hh"
# include "param.hh"

namespace shell
{
  template <typename float_type>
  class Ahdsr
  {
  public:

    enum CurveType {
      kLinear,
      kExp,
    };

    enum State {
      kIdle,
      kAttack,
      kHold,
      kDecay,
      kSustain,
      kRelease,
    };

    Ahdsr(const Context<float_type> & ctx)
      : ctx_(ctx),
        state_(kIdle),
        curve_type_(kExp),
        a_(ctx_.sr_ / 10),
        h_(ctx_.sr_ / 10),
        d_(ctx_.sr_ / 10),
        s_(0.9),
        r_(ctx_.sr_ / 4)
    {
      // attack
      params_[0].type_ = Param::kInteger;
      params_[0].scale_ = Param::kLinear;
      params_[0].min_ = 0;
      params_[0].max_ = 10000;
      params_[0].name_ = "attack";
      params_[0].desc_ = "duration (ms)";
      params_[0].get_ = [this] { return this->ctx_.stepToMs(this->a_); };
      params_[0].set_ = [this] (float v) { this->a_ = this->ctx_.msToStep(v); };

      // hold
      params_[1].type_ = Param::kInteger;
      params_[1].scale_ = Param::kLinear;
      params_[1].min_ = 0;
      params_[1].max_ = 10000;
      params_[1].name_ = "hold";
      params_[1].desc_ = "duration (ms)";
      params_[1].get_ = [this] { return this->ctx_.stepToMs(this->h_); };
      params_[1].set_ = [this] (float v) { this->h_ = this->ctx_.msToStep(v); };

      // decay
      params_[2].type_ = Param::kInteger;
      params_[2].scale_ = Param::kLinear;
      params_[2].min_ = 0;
      params_[2].max_ = 10000;
      params_[2].name_ = "decay";
      params_[2].desc_ = "duration (ms)";
      params_[2].get_ = [this] { return this->ctx_.stepToMs(this->d_); };
      params_[2].set_ = [this] (float v) { this->d_ = this->ctx_.msToStep(v); };

      // decay
      params_[3].type_ = Param::kFloat;
      params_[3].scale_ = Param::kLinear;
      params_[3].min_ = 0;
      params_[3].max_ = 1;
      params_[3].name_ = "sustain";
      params_[3].desc_ = "gain";
      params_[3].get_ = [this] { return s_; };
      params_[3].set_ = [this] (float v) { this->s_ = v; };

      // decay
      params_[4].type_ = Param::kInteger;
      params_[4].scale_ = Param::kLinear;
      params_[4].min_ = 0;
      params_[4].max_ = 10000;
      params_[4].name_ = "release";
      params_[4].desc_ = "duration (ms)";
      params_[4].get_ = [this] { return this->ctx_.stepToMs(this->r_); };
      params_[4].set_ = [this] (float v) { this->r_ = this->ctx_.msToStep(v); };

      // curve type
      params_[5].type_ = Param::kInteger;
      params_[5].scale_ = Param::kLinear;
      params_[5].min_ = 0;
      params_[5].max_ = kExp;
      params_[5].name_ = "type";
      params_[5].desc_ = "curve type";
      params_[5].get_ = [this] { return this->curve_type_; };
      params_[5].set_ = [this] (float v) { this->curve_type_ = (CurveType)(v); };
    }

    Param & param(uint32_t index) { return params_.at(index); }
    uint32_t paramCount() const { return params_.size(); }

    void noteOn()
    {
      state_ = kAttack;
      step_  = 0;
    }

    void noteOff()
    {
      r0_    = step();
      state_ = kRelease;
      step_  = 0;
    }

    State state() const { return state_; }

    float_type stepLinear()
    {
      float_type value;

      switch (state_) {
      case kIdle:
        return 0;

      case kAttack:
        if (step_ >= a_) {
          state_ = kHold;
          step_  = 0;
          return step();
        }

        value = float_type(step_) / float_type(a_);
        ++step_;
        return value;

      case kHold:
        if (step_ >= h_) {
          state_ = kDecay;
          step_  = 0;
          return step();
        }
        ++step_;
        return 1;

      case kDecay:
        if (step_ >= d_) {
          state_ = kSustain;
          step_  = 0;
          return step();
        }

        value = 1 - (1 - s_) * float_type(step_) / float_type(d_);
        ++step_;
        return value;

      case kSustain:
        return s_;

      case kRelease:
        if (step_ >= r_) {
          state_ = kIdle;
          return 0;
        }
        value = r0_ * float_type(r_ - step_) / float_type(r_);
        ++step_;
        return value;

      default:
        return 0;
      }
    }

    float_type stepExp()
    {
      float_type value;

      switch (state_) {
      case kIdle:
        return 0;

      case kAttack:
        if (step_ >= a_) {
          state_ = kDecay;
          step_  = 0;
          return step();
        }

        value = 1 - std::exp((-4 * float_type(step_)) / float_type(a_));
        ++step_;
        return value;

      case kHold:
        if (step_ >= h_) {
          state_ = kDecay;
          step_  = 0;
          return step();
        }
        ++step_;
        return 1;

      case kDecay:
        if (step_ >= d_) {
          state_ = kSustain;
          step_  = 0;
          return step();
        }

        value = 1 - (1 - s_) * (1 - std::exp((-4 * float_type(step_)) / float_type(d_)));
        ++step_;
        return value;

      case kSustain:
        return s_;

      case kRelease:
        if (step_ >= r_) {
          state_ = kIdle;
          return 0;
        }
        value = r0_ * std::exp((-4 * float_type(step_)) / float_type(r_));
        ++step_;
        return value;

      default:
        return 0;
      }
    }

    float_type step()
    {
      switch (curve_type_) {
      case kLinear:
        return stepLinear();

      case kExp:
        return stepExp();

      default:
        return 0;
      }
    }

  private:
    const Context<float_type> & ctx_;
    State                       state_;
    uint32_t                    step_;
    CurveType                   curve_type_;
    uint32_t                    a_; // duration in steps
    uint32_t                    h_; // duration in steps
    uint32_t                    d_; // duration in steps
    float_type                  s_; // sustain level
    uint32_t                    r_; // duration in steps
    float_type                  r0_; // initial value at release
    std::array<Param, 6>        params_;
  };
}

#endif /* !SHELL_AHDSR_HH */
