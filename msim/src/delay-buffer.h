#ifndef MSIM_DELAY_BUFFER_H
#define MSIM_DELAY_BUFFER_H

#include "property.h"
#include "simtime.h"

using namespace deliberate;

namespace msim
{

class Scheduler;
class SimpleTaggedData;

class DelayBuffer
{
public:

  DelayBuffer (Scheduler *sched);

  Property <Scheduler*> scheduler;

  void writeToBuffer (SimpleTaggedData * data, SimTickType delay);

};

}

#endif
