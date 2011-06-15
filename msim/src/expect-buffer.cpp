#include "expect-buffer.h"


namespace msim
{

ExpectBuffer::DataSlot::DataSlot ()
  :data (new SimpleTaggedData()),
   empty (true)
{
}

ExpectBuffer::ExpectBuffer (int nSlots)
  :slots (nSlots, DataSlot()),
   numSlots (nSlots)
{
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
    if (slots[s].empty && !slots[s].data->valid()) {
      slots[s].data->tag.set(tag);
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

} // namespace
