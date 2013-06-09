#ifndef SHELL_PREFIX_PARAMS_HH
# define SHELL_PREFIX_PARAMS_HH

namespace shell
{
  class PrefixParams
  {
  public:
    template <typename T>
    PrefixParams(T & obj,
                 const std::string & prefix)
    {
      params_.resize(obj.paramCount());
      for (int i = 0; i < obj.paramCount(); ++i) {
        params_[i] = obj.param(i);
        params_[i].name_ = prefix + params_[i].name_;
      }
    }

    Param & param(uint32_t index) { return params_.at(index); }
    uint32_t paramCount() const { return params_.size(); }

  private:
    std::vector<Param> params_;
  };
}

#endif /* !SHELL_PREFIX_PARAMS_HH */
