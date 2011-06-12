#ifndef MSIM_VERSION_H
#define MSIM_VERSION_H

#include <string>

namespace msim
{

#define MSIM_VERSION_MAJOR 0
#define MSIM_VERSION_MINOR 3
#define MSIM_VERSION_PATCH 1

#define MSIM_VERSION (((MSIM_VERSION_MAJOR)*10000) + ((MSIM_VERSION_MINOR)*100) + (MSIM_VERSION_PATCH))

class Version
{
public:

  static const int                 major;
  static const int                 minor;
  static const int                 patch;

  static const std::string         version;

};

}  // namespace


#endif
