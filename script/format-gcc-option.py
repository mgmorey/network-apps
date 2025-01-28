#!/usr/bin/env python3

# format-gcc-option: format compiler option for assembler or linker
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

import sys

if len(sys.argv) < 3:
    sys.exit(1)

tool = sys.argv[1]

if tool not in ('as', 'ld'):
    sys.exit(1)

option = '-W%.1s,%s' % (tool, ','.join(sys.argv[2:]))
print(option.replace('$', '\\$'))
