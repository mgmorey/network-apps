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

include language.mk
include system.mk

ifdef NDEBUG
	CPPFLAGS += -D_FORTIFY_SOURCE=2
	CXXFLAGS += -O2
else
ifeq "$(USING_DMALLOC)" "true"
	CPPFLAGS += -DDMALLOC -DMALLOC_FUNC_CHECK
	LDLIBS += -ldmalloc
else ifeq "$(USING_LIBASAN)" "true"
	CXXFLAGS += -fno-omit-frame-pointer -fsanitize=address
	CXXFLAGS += -fsanitize-address-use-after-scope
	LDFLAGS += -fsanitize=address
else
	CXXFLAGS += -fno-omit-frame-pointer
endif
ifneq "$(SYSTEM)" "Darwin"
	CPPFLAGS += -D_GLIBCXX_DEBUG
endif
	CXXFLAGS += -g3 -O0
endif

ifeq "$(SYSTEM)" "Darwin"
	CPPFLAGS += -D_DARWIN_C_SOURCE
else ifeq "$(SYSTEM)" "CYGWIN_NT"
	CPPFLAGS += -D_POSIX_C_SOURCE=200809L
else ifeq "$(SYSTEM)" "FreeBSD"
	CPPFLAGS += -I/usr/local/include
	LDLIBS += -L/usr/local/lib
else ifeq "$(SYSTEM)" "MINGW64_NT"
	CPPCHECK_FLAGS += -D_WIN32
	LDLIBS += -lws2_32
endif

ifeq "$(SYSTEM)" "Darwin"
	CPPFLAGS += -DHAVE_SOCKADDR_SA_LEN
else ifeq "$(SYSTEM)" "FreeBSD"
	CPPFLAGS += -DHAVE_SOCKADDR_SA_LEN
endif

ifneq "$(SYSTEM)" "FreeBSD"
	CPPFLAGS += -DUSING_STD_BYTE
endif

CPPFLAGS += -Iinclude
CXXFLAGS += -std=$(LANGUAGE) -Wall -Werror -Wextra -Wpedantic -Wshadow

ifeq "$(SYSTEM)" "Darwin"
	LDFLAGS += -Wl,-map,$@.map
else
ifneq "$(SYSTEM)" "FreeBSD"
	CXXFLAGS += -Wa,-adghln=$(subst .o,.lst,$@)
endif
	LDFLAGS += -Wl,-Map=$@.map
endif

CPPCHECK_FLAGS += --cppcheck-build-dir=tmp --enable=all	\
--inline-suppr --quiet --std=$(LANGUAGE)
