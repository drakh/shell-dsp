#ifndef SHELL_NON_COPYABLE_HH
# define SHELL_NON_COPYABLE_HH

namespace shell
{
  class NonCopyable
  {
  public:
    inline NonCopyable() {}

    // allow move stuff
    inline NonCopyable(NonCopyable &&) {}
    inline NonCopyable & operator=(NonCopyable&&) { return *this; }

  private:
    NonCopyable(const NonCopyable &);
    NonCopyable & operator=(const NonCopyable &);
  };
}

#endif /* !SHELL_NON_COPYABLE_HH */
