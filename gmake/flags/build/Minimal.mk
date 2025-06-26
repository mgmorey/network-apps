# -*- Mode: Makefile-GMake -*-

# Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

ifneq "$(filter 1 2 3,$(WITH_FORTIFY_SOURCE)" ""
ifeq "$(filter -D_FORTIFY_SOURCE=%,$(CPPFLAGS))" ""
CPPFLAGS += -D_FORTIFY_SOURCE=$(WITH_FORTIFY_SOURCE)
endif
endif

ifeq "$(filter -O%,$(CXXFLAGS))" ""
CXXFLAGS += -O
endif

ifeq "$(filter -fno-omit-frame-pointer,$(CXXFLAGS))" ""
CXXFLAGS += -fno-omit-frame-pointer
endif

# Local Variables:
# mode: makefile
# End:
