#ifndef CONTEXT_HH
# define CONTEXT_HH

# include <cstdint>

namespace shell
{
  template <typename float_type>
  class Context
  {
  public:
    Context(uint32_t _sr)
      : sr(_sr),
        pi(3.141592653589793238462643383279502884197169399375105820974944592),
        pi_sr(2 * pi / float_type(sr)),
        step(0)
    {
    }

    inline uint32_t msToStep(uint32_t ms) const {
      return 1000 * ms / sr;
    }

  protected:
    const uint32_t   sr;        // sampling rate
    const float_type pi;        // 3.14...
    const float_type pi_sr;     // 2 * pi / sr
    uint64_t         step;      // the current step
  };
}

#endif /* !CONTEXT_HH */
