#ifndef MSIM_DEBUG_LOG_H
#define MSIM_DEBUG_LOG_H

#include <iostream>

#define MS_LOG msim::DebugLog::Debug() 

#define MS_DEBUG(X) msim::DebugLog::Debug() << X << std::endl;

#define MS_TRACE msim::DebugLog::Debug() << __PRETTY_FUNCTION__ << "  "

namespace msim
{
class DebugLog
{
public:
 
static ostream & Debug() { return std::cerr; }

};

} // namespace

#endif

