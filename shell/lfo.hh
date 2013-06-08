#ifndef SHELL_LFO_HH
# define SHELL_LFO_HH

# include "osc.hh"

namespace shell
{
  template <typename float_type>
  class Lfo
  {
  public:
    Lfo(const Context<float_type> & ctx)
      : depth_(0),
        osc_(ctx)
    {
      // freq
      params_[0].type_ = Param::kFloat;
      params_[0].scale_ = Param::kLinear;
      params_[0].min_ = 0;
      params_[0].max_ = 20;
      params_[0].name_ = "freq";
      params_[0].desc_ = "frequency";
      params_[0].get_ = [this] { return osc_.freq(); };
      params_[0].set_ = [this] (float v) { this->osc_.setFreq(v); };

      // depth
      params_[1].type_ = Param::kFloat;
      params_[1].scale_ = Param::kLinear;
      params_[1].min_ = 0;
      params_[1].max_ = 12 * 4;
      params_[1].name_ = "depth";
      params_[1].desc_ = "depth";
      params_[1].get_ = [this] { return depth_; };
      params_[1].set_ = [this] (float v) { this->depth_ = v; };
    }

    Param & param(uint32_t index)
    {
      if (index < params_.size())
        return params_.at(index);
      index -= params_.size();
      return osc_.param(index);
    }

    uint32_t paramCount() const
    {
      return params_.size() + osc_.paramCount();
    }

    void setPhase(float_type phase)
    {
      osc_.setPhase(phase);
    }

    float_type step()
    {
      return std::pow(2, depth_ * osc_.step() / float_type(12));
    }

  private:
    float_type depth_;
    Osc<float_type> osc_;
    std::array<Param, 2> params_;
  };
}

#endif /* !SHELL_LFO_HH */
