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

ifneq "$(WITH_SANITIZE)" ""

ifneq "$(filter -O -O1 -O2 -Ofast -Os -Oz,$(CXXFLAGS))" ""

ifeq "$(filter -fno-omit-frame-pointer,$(CXXFLAGS))" ""
CXXFLAGS += -fno-omit-frame-pointer
endif

ifneq "$(filter -O2 -Ofast -Os -Oz,$(CXXFLAGS))" ""

ifeq "$(filter -fno-optimize-sibling-calls,$(CXXFLAGS))" ""
CXXFLAGS += -fno-optimize-sibling-calls
endif

endif

endif

ifeq "$(filter -fsanitize=%,$(CXXFLAGS))" ""
ifneq "$(findstring undefined,$(WITH_SANITIZE))" ""
CXXFLAGS += -fno-sanitize-recover=null
endif
ifneq "$(findstring memory,$(WITH_SANITIZE))" ""
CXXFLAGS += -fsanitize-memory-track-origins=1
endif
CXXFLAGS += -fsanitize=$(WITH_SANITIZE)
endif

ifeq "$(filter -fsanitize=%,$(LDFLAGS))" ""
LDFLAGS += -fsanitize=$(WITH_SANITIZE)
endif

endif
