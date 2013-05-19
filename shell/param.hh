#ifndef SHELL_PARAM_HH
# define SHELL_PARAM_HH

# include <functional>

namespace shell
{
  template <typename float_type>
  class Param
  {
  public:
    typedef std::function<float_type ()>     get_type;
    typedef std::function<void (float_type)> set_type;

    Param(const char *name,
          const char *desc,
          const get_type & get,
          const set_type & set,
          float_type min,
          float_type max)
      : name_(name),
        desc_(desc),
        get_(get),
        set_(set),
        min_(min),
        max_(max)
    {
    }

    virtual ~Param() {}

    const std::string & name() const { return name_; }
    const std::string & desc() const { return desc_; }

    float_type get() const { return get_(); }
    void set(float_type val) const { return set_(val); }

    float_type min() const { return min_; }
    float_type max() const { return max_; }

  private:
    std::string name_;
    std::string desc_;
    get_type    get_;
    set_type    set_;
    float       min_;
    float       max_;
  };
}

#endif /* !SHELL_PARAM_HH */
