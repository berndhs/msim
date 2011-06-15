#ifndef MSIM_EXPECT_BUFFER_H
#define MSIM_EXPECT_BUFFER_H

#include "deliberate.h"
#include "tagged-data.h"

using namespace deliberate;

namespace msim
{


class ExpectBuffer  // a set of data slots with tags of what to expect
{

public:

  ExpectBuffer (int nSlots=1);
  ~ExpectBuffer ();


  /// \brief false if not expecting data of this tag 
  bool  isExpectingData (DataTagType tag);

  /// \brief false if no empty slots, so will not be expecting
  bool  expectData (DataTagType tag);

  /// \brief false if not expecting data of this tag
  bool  dataToBuffer (SimpleTaggedDataPtr pData);

  /// \brief false if no matching data available
  bool  consumeData  (DataTagType tag, SimpleTaggedDataPtr pData);

  int   fullCount ();
  int   expectCount (DataTagType tagValue);
  int   capacity () { return numSlots; }

private:

  class DataSlot 
  {
  public:

    DataSlot ();

    SimpleTaggedDataPtr  data;
    bool                 empty;
  };

  DataSlot *    slots;
  int           numSlots;
};

} // namespace

#endif