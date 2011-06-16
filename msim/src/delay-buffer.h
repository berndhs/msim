#ifndef MSIM_DELAY_BUFFER_H
#define MSIM_DELAY_BUFFER_H

#include "property.h"

using namespace deliberate;

namespace msim
{

class Scheduler;

class DelayBuffer
{
public:

  DelayBuffer (Scheduler *sched);

  Property <Scheduler*> scheduler;
};

}

#endif
