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

# Define project-specific variables

PREFIX ?= /usr/local
VERSION ?= 0.0.1

# Language
language = c++
standard = $(language)20

# File suffixes
dependency_suffix = .dep
include_suffix = .h
object_suffix = .o
source_suffix = .cpp

# Directories
cache_dir = .cache
cppbuild_dir = $(cache_dir)/cppcheck
dependency_dir = $(cache_dir)/dependency
include_dir = include
library_dir = lib
object_dir = object
script_dir = script
source_dir = src

# Set virtual paths
vpath %$(include_suffix) $(include_dir)/network
vpath %$(source_suffix) $(source_dir)

# Include common functions and flag variables
include common.gmk
include flags.gmk

# Define enumerated file list variables

libnetwork_sources = address-sa.cpp address-sin.cpp address-sin6.cpp	\
address-sun.cpp address.cpp addresserror.cpp addrinfo.cpp		\
bind-endpoint.cpp bind-fd.cpp buffer.cpp bytestring.cpp cleanup.cpp	\
close.cpp connect-endpoint.cpp connect-fd.cpp context.cpp		\
dispatch-os-error.cpp error.cpp fd.cpp fdpair.cpp format-ai-error.cpp	\
format-os-error.cpp format.cpp get-endpoint.cpp				\
get-endpointresult.cpp get-hostname.cpp get-hostnameresult.cpp		\
get-hosts.cpp get-last-os-error.cpp get-length.cpp get-name.cpp		\
get-nameresult.cpp get-peername.cpp get-peernameresult.cpp		\
get-sa-data.cpp get-sa-family.cpp get-sa-length.cpp			\
get-sa-pointer.cpp get-sin-addr.cpp get-sin-pointer.cpp			\
get-sin-port.cpp get-sin6-addr.cpp get-sin6-pointer.cpp			\
get-sin6-port.cpp get-size.cpp get-socket.cpp get-socketpair.cpp	\
get-socketpairresult.cpp get-socketresult.cpp				\
get-sockets-endpoint.cpp get-sockets-hostname.cpp get-sockname.cpp	\
get-socknameresult.cpp get-sun-length.cpp get-sun-path-length.cpp	\
get-sun-path.cpp get-sun-pointer.cpp is-valid.cpp logicerror.cpp	\
open-endpoint.cpp open-fd.cpp oserrorresult.cpp				\
reset-last-os-error.cpp runtimeerror.cpp set-last-os-error.cpp		\
socket.cpp socketfamily.cpp socketflags.cpp sockethints.cpp		\
sockethost.cpp socketprotocol.cpp sockettype.cpp startup.cpp		\
stream-address.cpp stream-addrinfo.cpp stream-socket.cpp		\
to-byte-span-sa.cpp to-byte-span-sun.cpp to-byte-span-void.cpp		\
to-byte-string-bs.cpp to-byte-string-path.cpp to-byte-string-sa.cpp	\
to-byte-string-sun.cpp to-string-bs.cpp to-string-sin.cpp		\
to-string-sin6.cpp version.cpp

test_sources = test-address.cpp test-bind.cpp test-connect.cpp	\
test-context.cpp test-host.cpp test-hostname.cpp

ifneq "$(os_name)" "MINGW64_NT"
	test_sources += test-socket.cpp
	unix_sources = unix-client.cpp unix-server.cpp
endif

commands = compile_commands.json
sizes = sizes.txt
tags = TAGS

# Define computed file list variables

sources = $(libnetwork_sources) $(test_sources) $(unix_sources)

objects = $(addprefix $(object_dir)/,$(addsuffix	\
$(object_suffix),$(basename $(sources))))

libnetwork_objects = $(addprefix $(object_dir)/,$(addsuffix	\
$(object_suffix),$(basename $(libnetwork_sources))))

libnetwork_members = $(patsubst					\
%$(object_suffix),$(libnetwork_static)(%$(object_suffix)),	\
$(libnetwork_objects))

ifneq "$(WITH_SHARED_OBJS)" "false"
	libnetwork_shared = libnetwork.so.$(VERSION)
	libnetwork = $(call get-alias,$(libnetwork_shared))
endif

libnetwork_static = $(library_dir)/libnetwork.a

libraries = $(libnetwork) $(libnetwork_shared) $(libnetwork_static)

program_sources = $(test_sources) $(unix_sources)

program_objects = $(addprefix $(object_dir)/,$(addsuffix	\
$(object_suffix),$(basename $(program_sources))))

ifeq "$(os_type)" "ms-windows"
	program_suffix = .exe
endif

test_programs = $(addsuffix $(program_suffix),$(basename $(test_sources)))
unix_programs = $(addsuffix $(program_suffix),$(basename $(unix_sources)))
programs = $(addsuffix $(program_suffix),$(basename $(program_sources)))

dependencies = $(addprefix $(dependency_dir)/,$(subst	\
$(source_suffix),$(dependency_suffix),$(sources)))
listings = $(subst $(object_suffix),.lst,$(objects))
logfiles = $(addsuffix .log,$(basename $(programs)))
mapfiles = $(addsuffix .map,$(basename $(programs))) $(call	\
subst-suffix,.map,$(libnetwork))

dumps = $(addsuffix .stackdump,$(programs))

binary_artifacts = $(libraries) $(objects) $(programs) $(tags)
build_artifacts = $(libraries) $(listings) $(mapfiles) $(objects)	\
$(programs) $(sizes) $(sizes)~
text_artifacts = $(commands) $(dependencies) $(dumps) $(listings)	\
$(logfiles) $(mapfiles) $(sizes) $(sizes)~

rm_args = $(sort $(filter-out $(cache_dir)/%,$(filter-out	\
$(library_dir)/%,$(filter-out $(object_dir)/%,$(wildcard $(cache_dir)	\
$(library_dir) $(object_dir) $(artifacts))))))

dos2unix_args = $(sort $(filter-out %$(dependency_suffix),$(wildcard	\
$(text_artifacts))))

artifacts = $(binary_artifacts) $(text_artifacts)

build_targets = assert objects libraries programs sizes

ifeq "$(shell $(script_dir)/compare-versions $(cppcheck_version) 2.6)" "greater"
	build_targets += analyze
endif

ifeq "$(is_ctags_universal)" "true"
ifeq "$(shell $(script_dir)/compare-versions $(ctags_version) 5)" "greater"
	build_targets += tags
endif
endif

all_targets = $(build_targets) test

ifeq "$(os_name)" "MINGW64_NT"
	all_targets += dos2unix
endif

# Define compiler and linker variables

COMPILE.cc = $(strip $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c)
LINK$(object_suffix) = $(strip $(CXX) $(LDFLAGS))

# Define install program variable
ifeq "$(os_name)" "Darwin"
	install = ginstall
else
	install = install
endif

# Define script variables
make_depfile = $(script_dir)/make-makefile -d $(object_dir)
run_tests = $(script_dir)/run-test-programs

# Define pseudotargets

.PHONY: all
all: $(all_targets)

.PHONY: analyze
analyze: $(sources) | $(cppbuild_dir)
	printf '%s\n' $^ | xargs cppcheck $(CPPCHECK_FLAGS) $(CPPFLAGS)

.PHONY: assert
assert:
ifneq "$(sort $(libnetwork_sources))" "$(libnetwork_sources)"
	$(error File names in variable libnetwork_sources are not sorted)
endif
ifneq "$(sort $(test_sources))" "$(test_sources)"
	$(error File names in variable test_sources are not sorted)
endif
ifneq "$(sort $(unix_sources))" "$(unix_sources)"
	$(error File names in variable unix_sources are not sorted)
endif

.PHONY: build
build: $(build_targets)

.PHONY: check
check: $(test_programs)
	$(run_tests) $^

.PHONY: check-syntax
check-syntax:
	$(CXX) -fsyntax-only $(CXXFLAGS) $(CPPFLAGS) $(CHK_SOURCES)

.PHONY: clean
clean:
	rm -f $(sort $(wildcard $(build_artifacts)))

.PHONY: commands
commands: $(commands)

.PHONY: counts
counts:
	find include src -type f | sort | xargs wc

.PHONY: distclean
distclean:
	printf '%s\n' $(rm_args) | xargs rm -rf

.PHONY: dos2unix
dos2unix:
	printf '%s\n' $(dos2unix_args) | xargs dos2unix -q

.PHONY: install
install: $(libraries)
	$(install) -d $(PREFIX)/include/network $(PREFIX)/lib
	$(install) $(include_dir)/network/* $(PREFIX)/include/network
	$(install) $(libnetwork_static) $(libnetwork_shared) $(PREFIX)/lib
	cd $(PREFIX)/lib && ln -sf $(libnetwork_shared) $(libnetwork)

.PHONY: libraries
libraries: $(libraries)

.PHONY: objects
objects: $(objects)

.PHONY: programs
programs: $(programs)

.PHONY: realclean
realclean: distclean

.PHONY: sizes
sizes: $(sizes)
	test -e $<~ && diff -b $<~ $< || true

.PHONY: tags
tags: $(tags)

.PHONY: test
test: check

.PHONY: tidy
tidy: $(sort $(sources))
	$(clang_tidy) $^ $(CLANG_TIDY_FLAGS)

.PHONY: unix
unix:
	./unix-server & (sleep 1; ./unix-client 2 2; ./unix-client DOWN)

.SECONDARY: $(objects)

# Define targets

$(libnetwork_shared): $(libnetwork_objects)
	$(LINK$(object_suffix)) -o $@ $^ $(LDLIBS)

$(libnetwork): $(libnetwork_shared)
	ln -sf $< $@

ifeq "$(USING_ARCHIVE_MEMBER_RULE)" "true"
$(libnetwork_static): $(libnetwork_members)
else
$(libnetwork_static): $(libnetwork_objects)
	rm -f $@ && $(AR) $(ARFLAGS) $@ $^
endif

$(programs): $(libnetwork)

# Define suffix rules

(%): %
	$(AR) $(ARFLAGS) $@ $<

%$(program_suffix): $(object_dir)/%$(object_suffix)
	$(LINK$(object_suffix)) -o $@ $^ $(LDLIBS)

$(dependency_dir)/%$(dependency_suffix): %$(source_suffix)
	$(CXX) $(CPPFLAGS) -MM $< | $(make_depfile) -o $@ $(tags)

$(object_dir)/%$(object_suffix): %$(source_suffix)
	$(COMPILE$(source_suffix)) $(OUTPUT_OPTION) $<

$(commands): $(MAKEFILE_LIST)
	bear -- $(MAKE_COMMAND) $(MFLAGS) CXX=$(CXX) clean build

$(tags):
	ctags -e $(filter -D%,$(CPPFLAGS)) -R $(include_dir) $(source_dir)

sizes.txt: $(sort $(libnetwork_shared) $(objects) $(programs))
	if [ -e $@ ]; then mv -f $@ $@~; fi
	size $^ >$@

$(dependencies): | $(dependency_dir)

$(libraries): | $(library_dir)

$(objects): | $(object_dir)

$(cppbuild_dir):
	mkdir -p $(cppbuild_dir)

$(dependency_dir):
	mkdir -p $(dependency_dir)

$(library_dir):
	mkdir -p $(library_dir)

$(object_dir):
	mkdir -p $(object_dir)

# Include dependency files
ifeq "$(filter %clean,$(MAKECMDGOALS))" "$(filter-out %clean,$(MAKECMDGOALS))"
include $(sort $(dependencies))
endif
