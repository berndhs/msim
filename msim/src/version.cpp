#include "version.h"

const int msim::Version::verMajor (MSIM_VERSION_MAJOR);
const int msim::Version::verMinor (MSIM_VERSION_MINOR);
const int msim::Version::verPatch (MSIM_VERSION_PATCH);

#define msim_xstr(s) msim_str(s)
#define msim_str(s) #s

const std::string msim::Version::version 
           (msim_xstr(MSIM_VERSION_MAJOR)
            "." msim_xstr(MSIM_VERSION_MINOR)
            "." msim_xstr(MSIM_VERSION_PATCH)
           );


