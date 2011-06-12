#include "version.h"

namespace msim
{

const int Version::major (MSIM_VERSION_MAJOR);
const int Version::minor (MSIM_VERSION_MINOR);
const int Version::patch (MSIM_VERSION_PATCH);

#define msim_xstr(s) msim_str(s)
#define msim_str(s) #s

const std::string Version::version 
           (msim_xstr(MSIM_VERSION_MAJOR)
            "." msim_xstr(MSIM_VERSION_MINOR)
            "." msim_xstr(MSIM_VERSION_PATCH)
           );

}
