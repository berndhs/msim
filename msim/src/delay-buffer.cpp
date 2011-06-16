#include "delay-buffer.h"
#include "scheduler.h"
#include "tagged-data.h"

namespace msim
{
DelayBuffer::DelayBuffer (Scheduler *sched)
  :scheduler (sched)
{
}

}