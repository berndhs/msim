#include "tagged-data.h"

namespace msim
{

DataTagType   SimpleTaggedData::currentTag (256);

SimpleTaggedData::SimpleTaggedData (DataTagType tagValue)
  :tag (tagValue)
{
}

SimpleTaggedData::SimpleTaggedData (const SimpleTaggedData & other)
  :tag (other.tag)
{}

bool
SimpleTaggedData::valid ()
{
  return tag() > 0;
}

bool
SimpleTaggedData::validTag (DataTagType tagValue)
{
  return tagValue > 0;
}

DataTagType
SimpleTaggedData::genTag ()
{
  currentTag++;
  if (currentTag <= 0) {
    currentTag = 256;
  }
  return currentTag;
}

} // namespace
