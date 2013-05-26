#ifndef SHELL_EVENT_HH
# define SHELL_EVENT_HH

namespace shell
{
  struct ParamEvent
  {
    uint32_t index;
    float    value;
  };

  struct SynthEvent
  {
    float freq;
    float velocity;
    bool  gate;
  };

  struct Event
  {
    enum Type
    {
      kSynthEvent,
      kParamEvent
    };

    Type     type;   // Type of event
    uint32_t offset; // offset in samples from the beggining of the frame
    union
    {
      ParamEvent param;
      SynthEvent synth;
    };
  };
}

#endif /* !SHELL_EVENT_HH */
