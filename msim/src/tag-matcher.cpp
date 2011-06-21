#include "tag-matcher.h"


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
TagMatcher::TagMatcher ()
{}

TagMatcher::TagMatcher (const TagMatcher & other)
  :clientMap (other.clientMap)
{
}

void
TagMatcher::registerClient (DataDestination * client,  
                      DataTagType        tag, 
                      TagDuration           duration)
{
  ClientMap::iterator it = clientMap.lower_bound (tag);
  while (it != clientMap.end() && it->first == tag) {
    ClientRecord & rec (it->second);
    if (rec.client == client) {
      rec.duration = duration;
      return;
    }
    it++;
  }
  // if we get here, its a new key
  clientMap.insert (ClientMapPair (tag, ClientRecord (tag, duration, client)));
}

int
TagMatcher::unregisterClient (DataDestination * client)
{
  ClientMap::iterator it = clientMap.begin();
  int count(0);
  while (it != clientMap.end()) {
    if (it->second.client == client) {
      clientMap.erase (it);
    }
    count++;
  }
  return count;
}

int
TagMatcher::unregisterClient (DataDestination * client, DataTagType tag)
{
  ClientMap::iterator it = clientMap.lower_bound (tag);
  while (it != clientMap.end() && it->first == tag) {
    if (it->second.client == client) {
      clientMap.erase (it);
      return 1;
    }
    it++;
  }
  return 0;
}

bool
TagMatcher::dataArrived (SimpleTaggedDataPtr pData)
{
  if (pData == 0) {
    return false;
  }
  DataTagType tag = pData->tag();
  ClientMap::iterator it = clientMap.lower_bound (tag);
  bool delivered (false);
  while (it != clientMap.end() && it->first == tag) {
    DataDestination * client (it->second.client);
    if (client) {
      delivered |= client->dataArrived (pData);
      if (delivered && it->second.duration == TagDuration::Once) {
        clientMap.erase (it);
      }
    }
    it++;
  }
  return delivered;
}

} // namespace
