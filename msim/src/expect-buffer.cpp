#include "expect-buffer.h"
#include "debug-log.h"

namespace msim
{

ExpectBuffer::DataSlot::DataSlot ()
  :data (new SimpleTaggedData()),
   empty (true)
{
  MS_TRACE << " allocated slot at " << this << std::endl;
}

ExpectBuffer::ExpectBuffer (int nSlots)
  :slots (0),
   numSlots (nSlots)
{
  slots = new DataSlot[nSlots];
}

ExpectBuffer::~ExpectBuffer ()
{
  delete[] slots;
}

bool
ExpectBuffer::isExpectingData (DataTagType tag)
{
  for (int s=0; s<numSlots; s++) {
    if (slots[s].empty && slots[s].data->tag() == tag) {
      return true;
    }
  }
  return false;
}

bool
ExpectBuffer::expectData (DataTagType tag)
{
  if (!SimpleTaggedData::validTag (tag)) {
    return false;   // won't expect invalid data
  }
  for (int s=0; s<numSlots; s++) {
MS_TRACE << " slot " << s << " " 
         << slots[s].data << " empty " << slots[s].empty
         << " tag " << slots[s].data->tag() 
         << std::endl;
    if (slots[s].empty && !slots[s].data->valid()) {
      slots[s].data->tag.set(tag);
MS_TRACE << " using slot " << s << std::endl;
      return true;
    }
  }
  return false;
}

bool
ExpectBuffer::dataToBuffer (SimpleTaggedDataPtr pData)
{
  DataTagType tag = pData->tag();
  for (int s=0; s<numSlots; s++) {
    if (slots[s].empty && slots[s].data->tag() == tag) {
      slots[s].data  = pData;
      slots[s].empty = false;
      return true;
    }
  }
  return false;
}

bool
ExpectBuffer::consumeData (DataTagType tag, SimpleTaggedDataPtr pData)
{
  for (int s=0; s<numSlots; s++) {
    if (!slots[s].empty && slots[s].data->tag() == tag) {
      pData = slots[s].data;
      slots[s].empty = true;
      slots[s].data->tag.set(SimpleTaggedData::Invalid);
      return true;
    }
  }
  return false;
}

int
ExpectBuffer::fullCount ()
{
  int count (0);
  for (int s=0; s<numSlots; s++) {
    if (!slots[s].empty) {
      count++;
    }
  }
  return count;
}

int
ExpectBuffer::expectCount (DataTagType tagValue)
{
  int count (0);
  for (int s=0; s<numSlots; s++) {
MS_TRACE << " slot " << s << " " 
         << slots[s].data << " empty " << slots[s].empty
         << " tag " << slots[s].data->tag() 
         << std::endl;
    if (slots[s].empty && slots[s].data->tag() == tagValue) {
MS_TRACE << " count slot " << s << std::endl;
      count++;
    }
  }
  return count;
}

} // namespace
