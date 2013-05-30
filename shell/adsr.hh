#ifndef SHELL_ADSR_HH
# define SHELL_ADSR_HH

# include <cmath>
# include <cstdint>
# include <stdexcept>

# include "context.hh"
# include "param.hh"

namespace shell
{
  template <typename float_type>
  class Adsr
  {
  public:

    enum State {
      kIdle,
      kAttack,
      kDecay,
      kSustain,
      kRelease,
    };

    Adsr(const Context<float_type> & ctx)
      : ctx_(ctx),
        state_(kIdle),
        a_(ctx_.sr_ / 10),
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

      // decay
      params_[1].type_ = Param::kInteger;
      params_[1].scale_ = Param::kLinear;
      params_[1].min_ = 0;
      params_[1].max_ = 10000;
      params_[1].name_ = "decay";
      params_[1].desc_ = "duration (ms)";
      params_[1].get_ = [this] { return this->ctx_.stepToMs(this->d_); };
      params_[1].set_ = [this] (float v) { this->d_ = this->ctx_.msToStep(v); };

      // decay
      params_[2].type_ = Param::kFloat;
      params_[2].scale_ = Param::kLinear;
      params_[2].min_ = 0;
      params_[2].max_ = 1;
      params_[2].name_ = "sustain";
      params_[2].desc_ = "gain";
      params_[2].get_ = [this] { return s_; };
      params_[2].set_ = [this] (float v) { this->s_ = v; };

      // decay
      params_[3].type_ = Param::kInteger;
      params_[3].scale_ = Param::kLinear;
      params_[3].min_ = 0;
      params_[3].max_ = 10000;
      params_[3].name_ = "release";
      params_[3].desc_ = "duration (ms)";
      params_[3].get_ = [this] { return this->ctx_.stepToMs(this->r_); };
      params_[3].set_ = [this] (float v) { this->r_ = this->ctx_.msToStep(v); };
    }

    Param & param(uint32_t index)
    {
      if (index >= paramCount())
        throw std::out_of_range("");
      return params_[index];
    }

    uint32_t paramCount() const
    {
      return sizeof (params_) / sizeof (*params_);
    }

    void noteOn()
    {
      state_ = kAttack;
      step_  = 0;
    }

    void noteOff()
    {
      state_ = kRelease;
      step_  = 0;
    }

    State state() const { return state_; }

    float_type step()
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

      case kDecay:
        if (step_ >= d_) {
          state_ = kSustain;
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
        value = s_ * std::exp((-4 * float_type(step_)) / float_type(r_));
        ++step_;
        return value;

      default:
        return 0;
      }
    }

  private:
    const Context<float_type> & ctx_;
    State                       state_;
    uint32_t                    step_;
    uint32_t                    a_; // duration in steps
    uint32_t                    d_; // duration in steps
    float_type                  s_; // sustain level
    uint32_t                    r_; // duration in steps
    Param                       params_[4];
  };
}

#endif /* !SHELL_ADSR_HH */
