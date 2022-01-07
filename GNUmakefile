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

LANGUAGE := c++20

include flags.gmk

LINK.o = $(CXX) $(LDFLAGS)

prefix = /usr/local

sources = $(exec_sources) $(lib_sources)

test_sources = test-address.cpp test-bind.cpp test-connect.cpp	\
test-host.cpp test-hostname.cpp
unix_sources =

lib_sources = address.cpp address-sa.cpp address-sin.cpp		\
address-sin6.cpp address-sun.cpp addrinfo.cpp bind.cpp buffer.cpp	\
close.cpp connect.cpp context.cpp error.cpp family.cpp fd.cpp		\
flags.cpp format.cpp get-bytespan.cpp get-endpoint.cpp			\
get-hostname.cpp get-hosts.cpp get-length.cpp get-name.cpp		\
get-sa-data.cpp get-sa-family.cpp get-sa-length.cpp			\
get-sa-pointer.cpp get-sin-addr.cpp get-sin-pointer.cpp			\
get-sin-port.cpp get-sin6-addr.cpp get-sin6-pointer.cpp			\
get-sin6-port.cpp get-sockaddr-any.cpp get-sockaddr-byte.cpp		\
get-sockaddr-path.cpp get-sockaddr-sa.cpp get-sockaddr-size.cpp		\
get-sockaddr-sun.cpp get-socket.cpp get-socketpair.cpp			\
get-sockets.cpp get-peername.cpp get-sockname.cpp get-sun-length.cpp	\
get-sun-path.cpp get-sun-pointer.cpp hints.cpp host.cpp is-valid.cpp	\
open-endpoint.cpp open-fd.cpp optionalstring.cpp protocol.cpp		\
result.cpp socket.cpp socktype.cpp stream-address.cpp			\
stream-addrinfo.cpp stream-bytestring.cpp stream-hints.cpp		\
stream-optionalstring.cpp stream-socket.cpp to-string.cpp		\
to-string-sin.cpp to-string-sin6.cpp

ifneq "$(SYSTEM)" "MINGW64_NT"
	test_sources += test-socket.cpp
	unix_sources += unix-client.cpp	unix-server.cpp
endif

executables = $(test_execs) $(unix_execs)
test_execs = $(subst .cpp,,$(test_sources))
unix_execs = $(subst .cpp,,$(unix_sources))

exec_sources = $(test_sources) $(unix_sources)

libraries = libnetwork.a

objects = $(lib_objs) $(exec_objs)

exec_objs = $(addprefix $(tmp_dir)/,$(subst .cpp,.o,$(exec_sources)))
lib_objs = $(addprefix $(tmp_dir)/,$(subst .cpp,.o,$(lib_sources)))

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
-std=$(LANGUAGE) $(CPPFLAGS)

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
test: $(test_execs)
	for f in $(test_execs); do ./$$f; done

.PHONY:	unix
unix: $(unix_execs)
	./unix-server & (sleep 1; ./unix-client 2 2; ./unix-client DOWN)

.PHONY:	install
install: $(libraries)
	install libnetwork.a $(prefix)/lib
	install include/network/*.h $(prefix)/include

TAGS:
	printf '%s\n' $^ | etags --declarations --language=c++ -

$(executables): libnetwork.a

libnetwork.a: $(patsubst %.o,libnetwork.a(%.o),$(lib_objs))

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
