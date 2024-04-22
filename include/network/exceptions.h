// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef NETWORK_EXCEPTIONS_H
#define NETWORK_EXCEPTIONS_H

#include "network/addresserror.h"       // AddressError
#include "network/error.h"              // Error
#include "network/lengtherror.h"        // LengthError
#include "network/logicerror.h"         // LogicError
#include "network/namelengtherror.h"    // NameLengthError
#ifndef WIN32
#include "network/pathlengtherror.h"    // PathLengthError
#endif
#include "network/rangeerror.h"         // RangeError
#include "network/runtimeerror.h"       // RuntimeError
#include "network/salengtherror.h"      // SaLengthError
#ifndef WIN32
#include "network/sunlengtherror.h"     // SunLengthError
#endif
#include "network/valueerror.h"         // ValueError

#endif
