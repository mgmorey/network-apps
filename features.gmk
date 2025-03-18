# -*- Mode: Makefile-GMake -*-

# Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

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

# Define variables for toolset family and features
as_family := $(shell $(script_dir)/get-as-family $(AS))
as_features := $(shell $(script_dir)/get-features $(AS))
cxx_family := $(shell $(script_dir)/get-cc-family $(CXX))
is_uctags := $(filter true,$(shell $(script_dir)/is-uctags ctags))
ld_family := $(shell $(script_dir)/get-ld-family $(LD))
ld_features := $(shell $(script_dir)/get-features $(LD))

# Define variables for operating system identifiers
os_labels := $(shell $(script_dir)/get-os-release -ik)
os_macros := $(shell $(script_dir)/uppercase $(os_labels))

# Local Variables:
# mode: makefile
# End:
