#ifndef MSIM_TAGGED_DATA_H
#define MSIM_TAGGED_DATA_H

#include "deliberate.h"

using namespace deliberate;

namespace msim
{

typedef long long int DataTagType;

class SimpleTaggedData
{
public:

  enum SpecialTags {
    Invalid = -1
  };

  SimpleTaggedData (DataTagType tagValue = Invalid);
  SimpleTaggedData (const SimpleTaggedData & other);

  Property <DataTagType>  tag;

  virtual bool         valid ();

  static DataTagType   genTag ();

  static bool validTag (DataTagType tag);
  static SimpleTaggedData *  alloc ();

private:

  static DataTagType  currentTag;

};

typedef SimpleTaggedData*  SimpleTaggedDataPtr;

template <typename PayloadType>
class TaggedData : public SimpleTaggedData
{
public:

  TaggedData<PayloadType> (const TaggedData<PayloadType> & other)
    :SimpleTaggedData (other),
     payload (other.payload())
  {}

  TaggedData<PayloadType> (DataTagType tagValue = SimpleTaggedData::Invalid,
                           const PayloadType & data = PayloadType())
    :SimpleTaggedData (tagValue),
     payload (data)
  {}

  Property <PayloadType> payload;
};

} // namespace

#endif
