#ifndef MSIM_EXPECT_BUFFER_H
#define MSIM_EXPECT_BUFFER_H

#include "deliberate.h"
#include "tagged-data.h"
#include <vector>

using namespace deliberate;

namespace msim
{


class ExpectBuffer  // a set of data slots with tags of what to expect
{

public:

  ExpectBuffer (int nSlots);


  /// \brief false if not expecting data of this tag 
  bool  isExpectingData (DataTagType tag);

  /// \brief false if no empty slots, so will not be expecting
  bool  expectData (DataTagType tag);

  /// \brief false if not expecting data of this tag
  bool  dataToBuffer (TaggedDataPtr pData);

  /// \brief false if no matching data available
  bool  consumeData  (DataTagType tag, TaggedDataPtr pData);

private:

  class DataSlot 
  {
  public:

    DataSlot ();

    TaggedDataPtr  data;
    bool           empty;
  };

  std::vector<DataSlot>    slots;
  int                      numSlots;
};

} // namespace

#endif