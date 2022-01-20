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

language = c++
standard = $(language)20

os_release = $(shell bin/get-os-release -iko)
os_distro = $(word 1,$(os_release))
os_name = $(word 2,$(os_release))
os_type = $(word 3,$(os_release))

include flags.gmk
LINK.o = $(CXX) $(LDFLAGS)
LINK.so = $(CXX) $(LDFLAGS) -shared

prefix = /usr/local
tmp_dir = tmp

libraries = libnetwork.so libnetwork.a

libnetwork_sources = address.cpp address-sa.cpp address-sin.cpp		\
address-sin6.cpp address-sun.cpp addrinfo.cpp bind-endpoint.cpp		\
bind-fd.cpp buffer.cpp close.cpp connect-endpoint.cpp connect-fd.cpp	\
context.cpp family.cpp fd.cpp flags.cpp format.cpp			\
format-os-error.cpp get-endpoint.cpp get-hostname.cpp get-hosts.cpp	\
get-last-os-error.cpp get-length.cpp get-name.cpp get-peername.cpp	\
get-sa-data.cpp get-sa-family.cpp get-sa-length.cpp			\
get-sa-pointer.cpp get-sin-addr.cpp get-sin-pointer.cpp			\
get-sin-port.cpp get-sin6-addr.cpp get-sin6-pointer.cpp			\
get-sin6-port.cpp get-size.cpp get-socket.cpp get-socketpair.cpp	\
get-sockets-endpoint.cpp get-sockets-hostname.cpp get-sockname.cpp	\
get-sun-length.cpp get-sun-path.cpp get-sun-path-length.cpp		\
get-sun-pointer.cpp hints.cpp host.cpp is-valid.cpp open-endpoint.cpp	\
open-fd.cpp protocol.cpp reset-last-os-error.cpp			\
set-last-os-error.cpp socket.cpp socktype.cpp stream-address.cpp	\
stream-addrinfo.cpp stream-bytestring.cpp stream-hints.cpp		\
stream-socket.cpp to-byte-span-sa.cpp to-byte-span-sun.cpp		\
to-byte-span-void.cpp to-byte-string-bs.cpp to-byte-string-path.cpp	\
to-byte-string-sa.cpp to-byte-string-sun.cpp to-string-bs.cpp		\
to-string-sin.cpp to-string-sin6.cpp version.cpp

common_sources = test-address.cpp test-bind.cpp test-connect.cpp	\
test-context.cpp test-host.cpp test-hostname.cpp
posix_sources = test-socket.cpp
unix_sources = unix-client.cpp unix-server.cpp

ifeq "$(os_type)" "gnu-linux"
	program_sources = $(common_sources) $(posix_sources) $(unix_sources)
else ifeq "$(os_type)" "unix"

ifneq "$(os_distro)" "macos"
	program_sources = $(common_sources) $(posix_sources) $(unix_sources)
else
	program_sources = $(common_sources) $(posix_sources)
endif

else ifeq "$(os_name)" "CYGWIN_NT"
	program_sources = $(common_sources) $(posix_sources)
else
	program_sources = $(common_sources)
endif

sources = $(libnetwork_sources) $(program_sources)

programs = $(subst .cpp,,$(program_sources))
test_programs = $(filter test-%,$(programs))
unix_programs = $(filter unix-%,$(programs))

libnetwork_objs = $(addprefix $(tmp_dir)/,$(subst	\
.cpp,.o,$(libnetwork_sources)))
program_objs = $(addprefix $(tmp_dir)/,$(subst		\
.cpp,.o,$(program_sources)))

objects = $(program_objs) $(libnetwork_objs)

listings = $(addprefix $(tmp_dir)/,$(subst .o,.lst,$(objects)))
dependencies = $(addprefix $(tmp_dir)/,$(subst .o,.dep,$(objects)))

artifacts = $(programs) $(objects) $(libraries) $(listings) *.map

.PHONY: all
all: $(libraries) $(programs) sizes.txt TAGS

.PHONY: clean
clean:
	rm -f $(artifacts)

.PHONY: cppcheck
cppcheck:
	cppcheck $(CPPCHECK_FLAGS) $(CPPFLAGS) .

.PHONY: dos2unix
dos2unix:
	dos2unix *.log *.map $(tmp_dir)/*.lst $(tmp_dir)/*.txt

.PHONY: install
install: $(libraries)
	install $(libraries) $(prefix)/lib
	install include/network/*.h $(prefix)/include

.PHONY: realclean
realclean:
	rm -rf $(tmp_dir) $(artifacts) *.core *.map *.stackdump

.PHONY: report
report: sizes.txt
	test -e $^~ && diff -Z $^~ $^ || true

.PHONY: test
test: $(test_programs)
	for f in $^; do ./$$f >$$f.log; done

.PHONY: tidy
tidy: $(sources)
	clang-tidy$(LLVM_SUFFIX) $^ $(TIDY_FLAGS)

.PHONY: unix
unix: $(unix_programs)
	./unix-server & (sleep 1; ./unix-client 2 2; ./unix-client DOWN)

.SECONDARY: $(objects)

TAGS:
	printf '%s\n' $^ | etags --declarations --language=$(language) -

libnetwork.so: $(libnetwork_objs)
	$(LINK.so) -o $@ $^ $(LDLIBS)

ifeq "$(USING_ARCHIVE_MEMBER_RULE)" "true"
libnetwork.a: $(patsubst %.o,libnetwork.a(%.o),$(libnetwork_objs))
else
libnetwork.a: $(libnetwork_objs)
	rm -f $@
	$(AR) q $@ $^
endif

$(programs): $(libraries)

(%): %
	$(AR) $(ARFLAGS) $@ $<

%: $(tmp_dir)/%.o
	$(LINK.o) -o $@ $^ $(LDLIBS)

$(tmp_dir)/%.o: %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(tmp_dir)/%.dep: %.cpp
	$(CXX) $(CPPFLAGS) -MM $< | bin/make-makefile -f TAGS -o $@

sizes.txt: $(objects)
	if [ -e $@ ]; then mv -f $@ $@~; fi
	size $^ >$@

$(objects) $(dependencies) sizes.txt: | $(tmp_dir)

$(tmp_dir):
	mkdir -p $(tmp_dir)

ifneq "$(MAKECMDGOALS)" "realclean"
include $(dependencies)
endif

vpath %.h include/network
vpath %.cpp src
