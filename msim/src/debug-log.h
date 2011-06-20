#ifndef MSIM_DEBUG_LOG_H
#define MSIM_DEBUG_LOG_H


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


#include <iostream>
#include <fstream>

#define MS_DEBUG_OFF msim::DebugLog::setQuiet(true);
#define MS_DEBUG_ON msim::DebugLog::setQuiet(false);

#define MS_LOG msim::DebugLog::Debug() 

#define MS_DEBUG(X) msim::DebugLog::Debug() << X << std::endl;

#define MS_TRACE msim::DebugLog::Debug() << __PRETTY_FUNCTION__ << "  "

using namespace std;

namespace msim
{
class DebugLog
{
public:

static void setQuiet (bool q) { quiet = q;}

 
static std::ostream & Debug() 
  { 
    if (quiet) {
      return *noStream;
    }
    return cerr; 
  }

static bool isQuiet () { return quiet; }

private:

  static bool     quiet;

  static ofstream * noStream;
  static ofstream * initNada();


};

} // namespace

#endif

