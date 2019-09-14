#include <iostream>
#include <string>
#include "interpreter.h"
// #define NDEBUG
#ifndef NDEBUG
#include "test.h"
#endif

int main()
{
#ifndef NDEBUG
  test();
#endif
  interpreter().run();
}
