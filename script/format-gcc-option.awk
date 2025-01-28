#!/usr/bin/awk -f

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

BEGIN {
    if (ARGC <= 2) {
	exit(1);
    }

    tool = ARGV[1];

    if (tool != "as" && tool != "ld") {
	exit(1);
    }

    printf("-W%.1s", tool);
    i = 2;

    while (i < ARGC) {
	arg = ARGV[i++];
	gsub(/\$/, "\\$", arg);
	printf(",%s", arg);
    }

    printf("\n");
}
