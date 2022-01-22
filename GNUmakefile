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

so_name_filter = cut -d. -f 1-3
so_version = 0.0.1

prefix = /usr/local
tmpdir = tmp

include_suffix = .h
source_suffix = .cpp

include flags.gmk

libnetwork_sources = address.cpp address-sa.cpp address-sin.cpp		\
address-sin6.cpp address-sun.cpp addrinfo.cpp bind-endpoint.cpp		\
bind-fd.cpp buffer.cpp close.cpp connect-endpoint.cpp connect-fd.cpp	\
context.cpp dispatch-os-error.cpp family.cpp fd.cpp flags.cpp		\
format.cpp format-os-error.cpp get-endpoint.cpp get-hostname.cpp	\
get-hosts.cpp get-last-os-error.cpp get-length.cpp get-name.cpp		\
get-peername.cpp get-sa-data.cpp get-sa-family.cpp get-sa-length.cpp	\
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
posix_sources = test-socket.cpp unix-client.cpp unix-server.cpp

sources = $(libnetwork_sources) $(common_sources) $(posix_sources)

objects = $(addprefix $(tmpdir)/,$(addsuffix .o,$(basename	\
$(sources))))

libnetwork_objects = $(addprefix $(tmpdir)/,$(addsuffix .o,$(basename	\
$(libnetwork_sources))))

libnetwork_members = $(patsubst %.o,$(libnetwork_archive)(%.o),	\
$(libnetwork_objects))

libnetwork = libnetwork.so.$(so_version)
libnetwork_alias = $(shell echo $(libnetwork) | $(so_name_filter))
libnetwork_archive = libnetwork.a

libraries = $(libnetwork_alias) $(libnetwork) $(libnetwork_archive)

ifeq "$(os_name)" "MINGW64_NT"
	program_sources = $(common_sources)
else
	program_sources = $(common_sources) $(posix_sources)
endif

program_objects = $(addprefix $(tmpdir)/,$(addsuffix .o,$(basename	\
$(program_sources))))

programs = $(basename $(program_sources))

depends = $(subst .o,.dep,$(objects))
listings = $(subst .o,.lst,$(objects))
logfiles = $(addsuffix .log,$(programs))
mapfiles = $(addsuffix .map,$(programs) libnetwork)
sizes = sizes.txt
tags = TAGS

binary_artifacts = $(libraries) $(objects) $(programs)
text_artifacts = $(depends) $(listings) $(logfiles) $(mapfiles)	\
$(sizes) $(tags)

artifacts = $(binary_artifacts) $(text_artifacts)

COMPILE.cc = $(strip $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c)
LINK.o = $(strip $(CXX) $(LDFLAGS))

.PHONY: all
all: $(libraries) $(programs) sizes

.PHONY: analyze
analyze:
	cppcheck $(CPPCHECK_FLAGS) $(CPPFLAGS) .

.PHONY: check
check: test

.PHONY: clean
clean:
	rm -f $(artifacts)

.PHONY: distclean
distclean:
	rm -rf $(sort $(tmpdir) $(filter-out $(tmpdir)/%,$(artifacts)))

.PHONY: dos2unix

dos2unix:
	dos2unix $(sort $(wildcard $(text_artifacts)))

.PHONY: install
install: $(libraries)
	install $(libraries) $(prefix)/lib
	install include/network/*$(include_suffix) $(prefix)/include

.PHONY: realclean
realclean: distclean

.PHONY: sizes
sizes: $(sizes)
	test -e $<~ && diff -b $<~ $< || true

.PHONY: test
test: $(sort $(filter test-%,$(programs)))
	for f in $^; do ./$$f >$$f.log; done

.PHONY: tidy
tidy: $(sort $(sources))
	clang-tidy$(LLVM_SUFFIX) $^ $(TIDY_FLAGS)

.PHONY: unix
unix: $(sort $(filter unix-%,$(programs)))
	./unix-server & (sleep 1; ./unix-client 2 2; ./unix-client DOWN)

.SECONDARY: $(objects)

$(libnetwork): $(sort $(libnetwork_objects))
	$(LINK.o) -o $@ $^ $(LDLIBS)

$(libnetwork_alias): $(libnetwork)
	ln -sf $< $@

ifeq "$(USING_ARCHIVE_MEMBER_RULE)" "true"
$(libnetwork_archive): $(sort $(libnetwork_members))
else
$(libnetwork_archive): $(sort $(libnetwork_objects))
	rm -f $@ && $(AR) $(ARFLAGS) $@ $^
endif

$(programs): $(libnetwork_alias) $(libnetwork_archive)

(%): %
	$(AR) $(ARFLAGS) $@ $<

%: $(tmpdir)/%.o
	$(LINK.o) -o $@ $^ $(LDLIBS)

$(tmpdir)/%.o: %$(source_suffix)
	$(COMPILE$(source_suffix)) $(OUTPUT_OPTION) $<

$(tmpdir)/%.dep: %$(source_suffix)
	$(CXX) $(CPPFLAGS) -MM $< | bin/make-makefile -f TAGS -o $@

TAGS:
	printf '%s\n' $(sort $^) | etags --declarations --language=$(language) -

sizes.txt: $(sort $(objects))
	if [ -e $@ ]; then mv -f $@ $@~; fi
	size $^ >$@

$(objects) $(depends): | $(tmpdir)

$(tmpdir):
	mkdir -p $(tmpdir)

ifneq "$(MAKECMDGOALS)" "distclean"
include $(depends)
endif

vpath %$(include_suffix) include/network
vpath %$(source_suffix) src
