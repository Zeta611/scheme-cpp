#include "interpreter.h"
#ifndef NDEBUG
#include "test.h"
#endif

int main()
{
#ifndef NDEBUG
  // test();
#endif
  interpreter().run();
}
