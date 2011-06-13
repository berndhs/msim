#include "expect-buffer.h"


namespace msim
{

ExpectBuffer::ExpectBuffer (int nSlots)
  :numSlots (nSlots)
{
  slots.reserve (numSlots); 
}

DataSlot &
ExpectBuffer::slot (int index)
{
  return slots[index];
}


} // namespace