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

MAJOR_VERSION := 0
MINOR_VERSION := 0
MICRO_VERSION := 1

PREFIX ?= /usr/local

# Define language
language := c++
standard := $(language)20

# Define version
major = $(MAJOR_VERSION)
minor = $(MINOR_VERSION)
micro = $(MICRO_VERSION)
version = $(major).$(minor).$(micro)

# Define directories
binary_dir := bin
cache_dir := .cache
cppbuild_dir := $(cache_dir)/cppcheck
dependency_dir := $(cache_dir)/dependency
include_dir := include
library_dir := lib
object_dir := object
script_dir := script
source_dir := src

# File prefixes
library_prefix := lib

# File suffixes
alias_suffix := .so.$(major)

ifeq "$(os_id_type)" "ms-windows"
	binary_suffix = .exe
else
	binary_suffix =
endif

dependency_suffix := .dep
include_suffix := .h
object_suffix := .o
shared_suffix := .so.$(version)
source_suffix := .cpp

# Include common functions and flag variables
include common.gmk
include flags.gmk

# Set virtual paths
vpath %$(source_suffix) $(source_dir)/$(platform) $(source_dir)

# Define enumerated file list variables

library_common_sources = accept.cpp address-sa.cpp address-sin.cpp	\
address-sin6.cpp address.cpp addresserror.cpp addresslist.cpp		\
addressstring.cpp argumentdata.cpp bind-endpoint.cpp bind-handle.cpp	\
close.cpp commonsocket.cpp connect-endpoint.cpp connect-handle.cpp	\
create-socket-hints.cpp create-socketresult.cpp error.cpp		\
familyerror.cpp format-bytestring.cpp format.cpp get-endpoint.cpp	\
get-endpointresult.cpp get-hostname.cpp get-hostnameresult.cpp		\
get-length-maximum.cpp get-length-minimum.cpp get-name.cpp		\
get-nameresult.cpp get-operands.cpp get-option.cpp get-options.cpp	\
get-peername.cpp get-peernameresult.cpp get-sa-family.cpp		\
get-sa-length.cpp get-sa-pointer.cpp get-sa-span.cpp get-sin-addr.cpp	\
get-sin-pointer.cpp get-sin-port.cpp get-sin6-addr.cpp			\
get-sin6-pointer.cpp get-sin6-port.cpp get-sockname.cpp			\
get-socknameresult.cpp listen.cpp logicerror.cpp open-endpoint.cpp	\
open-handle.cpp open.cpp oserrorresult.cpp parse-argumentspan.cpp	\
parse.cpp rangeerror.cpp reset-last-context-error.cpp			\
reset-last-os-error.cpp runtimeerror.cpp shutdown.cpp			\
socketfamily.cpp socketflags.cpp sockethost.cpp socketprotocol.cpp	\
sockettype.cpp start-context.cpp stream-address.cpp			\
stream-addrinfo.cpp stream-context.cpp stream-socket.cpp		\
stream-version.cpp template.cpp to-bytestring-void.cpp			\
to-string-in-addr.cpp to-string-in6-addr.cpp to-string-span-byte.cpp	\
to-string-vector-byte.cpp to-string-vector-char.cpp			\
to-string-void.cpp validate-bs.cpp validate-sa.cpp validate-sin.cpp	\
validate-sin6.cpp

library_native_sources = create-socket-handle.cpp			\
format-ai-error.cpp format-os-error.cpp get-last-context-error.cpp	\
get-last-os-error.cpp nativecontext.cpp read.cpp			\
set-last-context-error.cpp set-last-os-error.cpp start.cpp stop.cpp	\
write.cpp

library_unix_sources = address-sun.cpp create-socketpair.cpp		\
create-socketpairresult.cpp get-path-length.cpp get-path-pointer.cpp	\
get-sun-length.cpp get-sun-pointer.cpp nativecontext.cpp		\
to-bytestring-null.cpp to-bytestring-path.cpp to-path.cpp		\
unixsocket.cpp validate-null.cpp validate-path.cpp validate-sun.cpp

test_common_sources = test-address.cpp test-bind.cpp test-connect.cpp	\
test-context.cpp test-host.cpp test-limits.cpp test-option.cpp		\
test-parse.cpp test-socket-inet.cpp

test_unix_sources = test-socket-unix.cpp

unix_sources = unix-client.cpp unix-server.cpp

commands = compile_commands.json

# Define functions for computing lists of objects and programs

get-dependencies = $(addprefix $(dependency_dir)/,$(addsuffix	\
$(dependency_suffix),$(basename $1)))
get-objects = $(addprefix $(object_dir)/,$(addsuffix	\
$(object_suffix),$(basename $1)))
get-programs = $(addprefix $(binary_dir)/,$(addsuffix	\
$(binary_suffix),$(basename $1)))

# Define variables
# Define variables for computed file lists

library_sources = $(library_common_sources) $(library_native_sources)

ifneq "$(os_id_name)" "MINGW64_NT"
	library_sources += $(library_unix_sources)
endif

sources = $(library_sources) $(test_sources)

ifneq "$(os_id_name)" "MINGW64_NT"
	sources += $(unix_sources)
endif

test_sources = $(test_common_sources)

ifneq "$(os_id_name)" "MINGW64_NT"
	test_sources += $(test_unix_sources)
endif

objects = $(call get-objects,$(sources))

library_objects = $(call get-objects,$(library_sources))

library_members = $(patsubst				\
%$(object_suffix),$(library_static)(%$(object_suffix)),	\
$(library_objects))

library_alias = $(library_dir)/libnetwork$(alias_suffix)
library_mapfile = $(library_dir)/libnetwork.map
library_shared = $(library_dir)/libnetwork$(shared_suffix)
library_static = $(library_dir)/libnetwork.a

ifeq "$(os_id_type)" "ms-windows"
	libraries = $(library_static)
else
	libraries = $(library_alias) $(library_shared)
endif

program_sources = $(test_sources)

ifneq "$(os_id_type)" "ms-windows"
ifneq "$(os_id_dist)" "macos"
	program_sources += $(unix_sources)
endif
endif

program_objects = $(call get-objects,$(program_sources))

programs = $(call get-programs,$(program_sources))
test_programs = $(call get-programs,$(test_sources))
unix_programs = $(call get-programs,$(unix_sources))

dependencies = $(call get-dependencies,$(sources))
listings = $(objects:$(object_suffix)=.lst)
logfiles = $(notdir $(programs:$(binary_suffix)=.log))
mapfiles = $(programs:$(binary_suffix)=.map)
stackdumps = $(programs:$(binary_suffix)=.stackdump)

artifacts = $(binary_artifacts) $(text_artifacts)
binary_artifacts = $(libraries) $(objects) $(programs) TAGS
build_artifacts = $(libraries) $(listings) $(mapfiles) $(objects)	\
$(programs) sizes.txt sizes.txt~
text_artifacts = $(commands) $(dependencies) $(listings) $(logfiles)	\
$(mapfiles) sizes.txt sizes.txt~ $(stackdumps)

dos2unix_args = $(sort $(filter-out %$(dependency_suffix),$(wildcard	\
$(text_artifacts))))

rm_args = $(sort $(filter-out $(binary_dir)/%,$(filter-out		\
$(cache_dir)/%,$(filter-out $(library_dir)/%,$(filter-out		\
$(object_dir)/%,$(wildcard $(binary_dir) $(cache_dir) $(library_dir)	\
$(object_dir) $(artifacts)))))))

# Define variables for build target lists

build_targets = assert objects libraries programs sizes

ifeq "$(ctags_is_universal)" "true"
ifeq "$(call compare-versions,$(ctags_version),5.8)" "greater"
	build_targets += tags
endif
endif

all_targets = $(build_targets) sizes test

ifneq "$(os_id_type)" "ms-windows"
ifneq "$(os_id_dist)" "macos"
	all_targets += unix
endif
endif

ifeq "$(os_id_name)" "MINGW64_NT"
	all_targets += dos2unix
endif

# Define variables for compiler, linker, and script commands
COMPILE.cc = $(strip $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c)
LINK$(object_suffix) = $(strip $(CXX) $(LDFLAGS))
make_makefile = $(script_dir)/make-makefile -d $(object_dir)

# Define variable for install program
ifeq "$(os_id_name)" "Darwin"
	install = ginstall
else
	install = install
endif

# Define pseudotargets

.PHONY: all
all: $(all_targets)

.PHONY: analyze
analyze: $(sources) | $(cppbuild_dir)
	printf '%s\n' $^ | xargs cppcheck $(CPPCHECK_FLAGS)

.PHONY: assert
assert:
ifneq "$(sort $(library_common_sources))" "$(library_common_sources)"
	$(warning File names in variable library_common_sources are not sorted)
endif
ifneq "$(sort $(library_unix_sources))" "$(library_unix_sources)"
	$(warning File names in variable library_unix_sources are not sorted)
endif
ifneq "$(sort $(test_common_sources))" "$(test_common_sources)"
	$(warning File names in variable test_common_sources are not sorted)
endif
ifneq "$(sort $(test_unix_sources))" "$(test_unix_sources)"
	$(warning File names in variable test_unix_sources are not sorted)
endif
ifneq "$(sort $(unix_sources))" "$(unix_sources)"
	$(warning File names in variable unix_sources are not sorted)
endif

.PHONY: build
build: $(build_targets)

.PHONY: check
check: $(test_programs)
	$(script_dir)/run-test-programs $(sort $^)

.PHONY: check-syntax
check-syntax:
	$(CXX) -fsyntax-only $(CXXFLAGS) $(CPPFLAGS) $(CHK_SOURCES)

.PHONY: clean
clean:
	rm -f $(sort $(wildcard $(build_artifacts)))

.PHONY: count-library-common-source-files
count-library-common-source-files: $(library_common_sources)
	@printf '%s\n' $(words $^)

.PHONY: count-library-source-files
count-library-source-files: $(library_sources)
	@printf '%s\n' $(words $^)

.PHONY: count-source-files
count-source-files: $(sources)
	@printf '%s\n' $(words $^)

.PHONY: count-test-source-files
count-test-source-files: $(test_sources)
	@printf '%s\n' $(words $^)

.PHONY: count-unix-source-files
count-unix-source-files: $(unix_sources)
	@printf '%s\n' $(words $^)

.PHONY: distclean
distclean:
	printf '%s\n' $(rm_args) | xargs rm -rf

.PHONY: dos2unix
dos2unix:
	printf '%s\n' $(dos2unix_args) | xargs dos2unix -q

.PHONY: install
install: $(libraries) $(programs)
	$(install) -d $(PREFIX)/include/$(platform)/network \
	$(PREFIX)/include/network $(PREFIX)/bin $(PREFIX)/lib
	$(install) -m 644 $(include_dir)/$(platform)/network/* \
	$(PREFIX)/include/$(platform)/network
	$(install) -m 644 $(include_dir)/network/* \
	$(PREFIX)/include/network
	$(install) -s $(filter-out %$(alias_suffix),$(libraries)) $(PREFIX)/lib
	$(install) -s $(programs) $(PREFIX)/bin
	cd $(PREFIX)/lib && ln -sf $(notdir $(library_shared) $(library_alias))

.PHONY: libraries
libraries: $(libraries)

.PHONY: objects
objects: $(sort $(objects))

.PHONY: programs
programs: $(sort $(programs))

.PHONY: realclean
realclean: distclean

.PHONY: sizes
sizes: sizes.txt
	test -e $<~ && diff -b $<~ $< || true

.PHONY: tags
tags: TAGS

.PHONY: test
test: check

ifeq "$(cxx_family)" "clang"
.PHONY: tidy
tidy: $(sort $(sources))
	$(CLANG_TIDY) $^ $(CLANG_TIDY_FLAGS)
endif

.PHONY: unix
unix: $(sort $(unix_programs))
	$(script_dir)/run-unix-programs

.SECONDARY: $(objects)

# Define targets

$(library_alias): $(library_shared)
	cd $(library_dir) && ln -sf $(notdir $< $@)

$(library_shared): $(library_objects)
	$(LINK$(object_suffix)) -o $@ $^ $(LDLIBS)

$(library_static): $(library_objects)
	rm -f $@ && $(AR) $(ARFLAGS) $@ $^

$(programs): $(library_alias)

# Define suffix rules

(%): %
	$(AR) $(ARFLAGS) $@ $<

$(binary_dir)/%$(binary_suffix): $(object_dir)/%$(object_suffix)
	$(LINK$(object_suffix)) -o $@ $^ $(LDLIBS)

$(dependency_dir)/%$(dependency_suffix): %$(source_suffix)
	$(CXX) $(CPPFLAGS) -MM $< | $(make_makefile) -o $@ TAGS

$(object_dir)/%$(object_suffix): %$(source_suffix)
	$(COMPILE$(source_suffix)) $(OUTPUT_OPTION) $<

sizes.txt: $(sort $(library_shared) $(objects) $(programs))
	if [ -e $@ ]; then mv -f $@ $@~; fi
	size $^ >$@

TAGS:
	ctags -e $(filter -D%,$(CPPFLAGS)) -R $(include_dir) $(source_dir)

$(dependencies): | $(dependency_dir)

$(libraries): | $(library_dir)

$(objects): | $(object_dir)

$(programs): | $(binary_dir)

$(binary_dir):
	mkdir -p $(binary_dir)

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
