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

STANDARD := c++20

ifeq "$(STANDARD)" "c++20"
ifeq "$(shell bin/g++-less-than 10 $(CXX))" "true"
	STANDARD := c++2a
endif
endif

SYSTEM := $(shell uname -s | cut -d- -f 1)

CPPCHECK_FLAGS += --cppcheck-build-dir=tmp --enable=all	\
--inline-suppr --quiet --std=$(STANDARD)

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
ifneq "$(SYSTEM)" "FreeBSD"
	CPPFLAGS += -DUSING_STD_BYTE
endif
ifneq "$(SYSTEM)" "Darwin"
	CPPFLAGS += -D_GLIBCXX_DEBUG
endif
	CXXFLAGS += -g3 -O0
endif

ifeq "$(SYSTEM)" "Darwin"
	CPPFLAGS += -D_DARWIN_C_SOURCE -DHAVE_SOCKADDR_SA_LEN
else ifeq "$(SYSTEM)" "CYGWIN_NT"
	CPPFLAGS += -D_POSIX_C_SOURCE=200809L
else ifeq "$(SYSTEM)" "FreeBSD"
	CPPFLAGS += -DHAVE_SOCKADDR_SA_LEN
	CPPFLAGS += -I/usr/local/include
	LDLIBS += -L/usr/local/lib
else ifeq "$(SYSTEM)" "MINGW64_NT"
	CPPCHECK_FLAGS += -D_WIN32
	LDLIBS += -lws2_32
endif

CPPFLAGS += -Iinclude
CXXFLAGS += -std=$(STANDARD) -Wall -Werror -Wextra -Wpedantic -Wshadow

ifeq "$(SYSTEM)" "Darwin"
	LDFLAGS += -Wl,-map,$@.map
else
ifneq "$(SYSTEM)" "FreeBSD"
	CXXFLAGS += -Wa,-adghln=$(subst .o,.lst,$@)
endif
	LDFLAGS += -Wl,-Map=$@.map
endif

LINK.o = $(CXX) $(LDFLAGS)

prefix = /usr/local

sources = $(exec_sources) $(lib_sources)

exec_sources = test-address.cpp test-bind.cpp test-connect.cpp	\
test-hostname.cpp

lib_sources = address.cpp address-sa.cpp address-sin.cpp		\
address-sin6.cpp address-sun.cpp addrinfo.cpp bind.cpp buffer.cpp	\
close.cpp connect.cpp context.cpp error.cpp family.cpp flags.cpp	\
format.cpp get-bytespan.cpp get-endpoint.cpp get-hostname.cpp		\
get-hosts.cpp get-length.cpp get-name.cpp get-sa-family.cpp		\
get-sa-length.cpp get-sa-pointer.cpp get-sin-pointer.cpp		\
get-sin6-pointer.cpp get-sockaddr-any.cpp get-sockaddr-byte.cpp		\
get-sockaddr-path.cpp get-sockaddr-sa.cpp get-sockaddr-size.cpp		\
get-sockaddr-sun.cpp get-socket.cpp get-socketpair.cpp			\
get-sockets.cpp get-peername.cpp get-sockname.cpp get-sun-length.cpp	\
get-sun-path.cpp get-sun-pointer.cpp hints.cpp host.cpp is-valid.cpp	\
open-endpoint.cpp open-fd.cpp optionalstring.cpp protocol.cpp		\
result.cpp socket.cpp socktype.cpp stream-address.cpp			\
stream-addrinfo.cpp stream-bytestring.cpp stream-hints.cpp		\
stream-optionalstring.cpp stream-socket.cpp to-string.cpp

ifneq "$(SYSTEM)" "MINGW64_NT"
	exec_sources += test-socket.cpp unix-client.cpp	\
	unix-server.cpp
endif

executables = $(subst .cpp,,$(exec_sources))
libraries = libnetwork.a

objects = $(lib_objects) $(exec_objects)

exec_objects = $(addprefix $(tmp_dir)/,$(subst	\
.cpp,.o,$(exec_sources)))
lib_objects = $(addprefix $(tmp_dir)/,$(subst	\
.cpp,.o,$(lib_sources)))

dependencies = $(addprefix $(tmp_dir)/,$(subst .cpp,.dep,$(sources)))
listings = $(addprefix $(tmp_dir)/,$(subst .cpp,.lst,$(sources)))
maps = $(subst .cpp,.map,$(exec_sources))

tmp_dir = tmp

.PHONY:	all
all: $(executables) TAGS

.PHONY:	check
check: cppcheck

.PHONY:	clang-tidy
clang-tidy:	$(sources)
	clang-tidy $^ -config-file='.clang-tidy' -header-filter='.*' --	\
-std=$(STANDARD) $(CPPFLAGS)

.PHONY:	clean
clean:
	rm -f $(executables) $(libraries) $(objects) $(maps) $(listings)

.PHONY:	cppcheck
cppcheck:
	cppcheck $(CPPCHECK_FLAGS) $(CPPFLAGS) .

.PHONY:	dos2unix
dos2unix:
	dos2unix *.log *.map $(tmp_dir)/*.lst $(tmp_dir)/*.txt

.PHONY:	realclean
realclean: clean
	rm -rf TAGS *.core *.stackdump $(tmp_dir)

.PHONY:	test
test: $(executables)
	for f in test-*; do if [ -x $$f ]; then ./$$f; fi; done

.PHONY:	unix
unix: $(executables)
	./unix-server & (sleep 1; ./unix-client 2 2; ./unix-client DOWN)

.PHONY:	install
install: $(libraries)
	install libnetwork.a $(prefix)/lib
	install include/network/*.h $(prefix)/include

TAGS:	$(sources)
	etags $^

$(executables): libnetwork.a

libnetwork.a: $(patsubst %.o,libnetwork.a(%.o),$(lib_objects))

$(dependencies) $(objects): | $(tmp_dir)

$(tmp_dir):
	mkdir -p $(tmp_dir)

include $(dependencies)

.SECONDARY: $(objects)

%: $(tmp_dir)/%.o
	$(LINK.o) -o $@ $^ $(LDLIBS)

$(tmp_dir)/%.o: %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(tmp_dir)/%.dep: %.cpp
	$(CXX) $(CPPFLAGS) -MM $< | bin/make-makefile -f TAGS -o $@

vpath %.cpp src
vpath %.h include/network
