#ifndef SHELL_EVENT_HH
# define SHELL_EVENT_HH

namespace shell
{
  struct SynthEvent
  {
    uint32_t channel; // starts at 0.
    float    freq;
    float    velocity;
  };
}

#endif /* !SHELL_EVENT_HH */
