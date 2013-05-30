#ifndef CONTEXT_HH
# define CONTEXT_HH

# include <cstdint>

namespace shell
{
  template <typename float_type>
  class Context
  {
  public:
    Context(uint32_t sr)
      : sr_(sr),
        pi_(3.141592653589793238462643383279502884197169399375105820974944592),
        pi_sr_(2 * pi_ / float_type(sr))
    {
    }

    inline uint32_t msToStep(uint32_t ms) const {
      return (ms * sr_) / 1000;
    }

    inline uint32_t stepToMs(uint32_t step) const {
      return (step * 1000) / sr_;
    }

    const uint32_t   sr_;       // sampling rate
    const float_type pi_;       // 3.14...
    const float_type pi_sr_;    // 2 * pi / sr
  };
}

#endif /* !CONTEXT_HH */
