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

# Set build flags common to all build types
include gmake/flags/common-flags.mk

# Set build flags specific to build type
ifneq "$(filter Debug Minimal Release Small,$(BUILD_TYPE))" ""
include gmake/flags/build/$(BUILD_TYPE).mk
else
include gmake/flags/build/Default.mk
endif

# Set build flags for coverage
ifneq "$(filter Debug Default,$(BUILD_TYPE))" ""
include gmake/flags/coverage-flags.mk
endif

# Set build flags for GDB DWARF format
ifneq "$(filter Debug Default,$(BUILD_TYPE))" ""
include gmake/flags/dwarf-flags.mk
endif

# Set build flags for Glib C++
ifneq "$(filter Debug Default,$(BUILD_TYPE))" ""
include gmake/flags/glibcxx-flags.mk
endif

# Set build flags for LTO
ifneq "$(filter Minimal Small,$(BUILD_TYPE))" ""
include gmake/flags/lto-flags.mk
endif

# Set build flags for sanitizers
ifneq "$(filter Default,$(BUILD_TYPE))" ""
include gmake/flags/sanitize-flags.mk
endif
