#ifndef SHELL_PARAM_HH
# define SHELL_PARAM_HH

# include <string>
# include <functional>

namespace shell
{
  struct Param
  {
    enum Type {
      kBoolean,
      kInteger,
      kFloat,
    };

    enum Scale {
      kLinear,
      kLog,
    };

    Param()
      : type_(kFloat),
        scale_(kLinear),
        min_(0),
        max_(0),
        name_("no name"),
        desc_("no desc")
    {
    }

    Type                          type_;
    Scale                         scale_;
    float                         min_;
    float                         max_;
    std::string                   name_;
    std::string                   desc_;
    std::function<std::string ()> label_;
    std::function<float ()>       get_;
    std::function<void (float)>   set_;
  };
}

#endif /* !SHELL_PARAM_HH */
