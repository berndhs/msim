#ifndef MSIM_EXPECT_BUFFER_H
#define MSIM_EXPECT_BUFFER_H

#include "deliberate.h"
#include "tagged-data.h"
#include <vector>

using namespace deliberate;

namespace msim
{

class DataSlot 
{
public:

  Property <TaggedData*>    data;
  Property <bool>           empty;
};

class ExpectBuffer  // a set of data slots with tags of what to expect
{

public:

  ExpectBuffer (int nSlots);

  DataSlot & slot (int index);

private:

  std::vector<DataSlot>    slots;
  int                      numSlots;
};

} // namespace

#endif