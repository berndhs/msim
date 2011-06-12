#include "msim/src/version.h"
#include <iostream>

int
main (int argc, char*cargv[])
{
  std::cout << msim::Version::version << std::endl;
}
