#ifndef MSIM_TAGGED_DATA_H
#define MSIM_TAGGED_DATA_H

#include "deliberate.h"
#include <boost/shared_ptr.hpp>

using namespace deliberate;

namespace msim
{

typedef long long int DataTagType;

class TaggedData
{
public:

  TaggedData ();

  Property <DataTagType>  tag;

  virtual bool         valid ();

  static DataTagType   genTag ();

  static const DataTagType   invalid;

  static bool validTag (DataTagType tag);

private:

  static DataTagType  currentTag;

};

typedef TaggedData*  TaggedDataPtr;

} // namespace

#endif
