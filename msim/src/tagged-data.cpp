#include "tagged-data.h"

namespace msim
{

DataTagType   TaggedData::currentTag (256);

const DataTagType   TaggedData::invalid (-1);

TaggedData::TaggedData ()
  :tag (-1)
{
}

bool
TaggedData::valid ()
{
  return tag() > 0;
}

bool
TaggedData::validTag (DataTagType tagValue)
{
  return tagValue > 0;
}

DataTagType
TaggedData::genTag ()
{
  currentTag++;
  if (currentTag <= 0) {
    currentTag = 256;
  }
  return currentTag;
}

} // namespace
