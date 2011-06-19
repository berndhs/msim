#include "data-consumer.h"


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
DataConsumer::DataConsumer ()
  :numPorts (4),
   registerFile (0)
{
  registerFile = new Register[numPorts];
}

DataConsumer::Register::Register ()
  :ExpectBuffer (1),
   TagMatcher()
{
}

int
DataConsumer::portCount ()
{
  return numPorts;
}

void
DataConsumer::expectTag (int port, DataTagType tag)
{
  if (0 <= port && port < numPorts) {
    registerFile[port].expectData (tag);
  } 
}

bool
DataConsumer::Register::dataArrived (SimpleTaggedDataPtr pData)
{
  return writeToBuffer (pData);
}

}  // namespace
