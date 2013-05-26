#ifndef SHELL_NOTE_HH
# define SHELL_NOTE_HH

# include <cmath>

namespace shell
{
  template <typename float_type>
  float_type noteToFreq(uint8_t note)
  {
    return 440 * std::pow(2, float_type(note - 69) / float_type(12));
  }
}

#endif /* !SHELL_NOTE_HH */
