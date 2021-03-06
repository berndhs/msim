#include "tagged-data.h"


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


namespace msim
{

DataTagType   SimpleTaggedData::currentTag (SimpleTag::MinDynamic);

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
  return tag() >= SimpleTag::Invalid;
}

bool
SimpleTaggedData::validTag (DataTagType tagValue)
{
  return tagValue >= SimpleTag::Invalid;
}

DataTagType
SimpleTaggedData::genTag ()
{
  currentTag++;
  if (currentTag <= SimpleTag::Invalid) {
    currentTag = SimpleTag::MinDynamic;
  }
  return currentTag;
}

} // namespace
