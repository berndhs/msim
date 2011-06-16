#ifndef MSIM_EXPECT_BUFFER_H
#define MSIM_EXPECT_BUFFER_H


/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/


#include "property.h"
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
  virtual bool  isExpectingData (DataTagType tag);

  /// \brief false if no empty slots, so will not be expecting
  virtual bool  expectData (DataTagType tag);

  /// \brief false if not expecting data of this tag
  virtual bool  writeToBuffer (SimpleTaggedDataPtr pData);

  /// \brief false if no matching data available
  virtual bool  consumeData  (DataTagType tag, SimpleTaggedDataPtr pData);

  virtual int   fullCount ();
  virtual int   expectCount (DataTagType tagValue);
  virtual int   capacity () { return numSlots; }

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