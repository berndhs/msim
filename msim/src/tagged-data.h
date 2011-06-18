#ifndef MSIM_TAGGED_DATA_H
#define MSIM_TAGGED_DATA_H


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

using namespace deliberate;

namespace msim
{

/** \brief this library is not intended for systems that have trouble 
  *   with 64 bit values
  */

typedef unsigned long long int DataTagType;  

class SimpleTaggedData
{
public:

  enum SimpleTag: DataTagType {
    Invalid = 0,
    MinStatic = 1,
    MinDynamic = (1 << 30)
  };

  SimpleTaggedData (DataTagType tagValue = Invalid);
  SimpleTaggedData (const SimpleTaggedData & other);

  Property <DataTagType>  tag;

  bool         valid ();

  static DataTagType   genTag ();  // generate a dynamic tag

  static bool validTag (DataTagType tag);

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
