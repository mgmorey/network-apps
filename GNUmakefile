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

PREFIX ?= /tmp/$(library_file)
VERSION ?= 0.0.1

# Define language
language := c++
standard := $(language)20

# Define cache directory
cache_dir := .cache

# Define build directories
cppcheck_dir := $(cache_dir)/cppcheck
depend_dir := $(cache_dir)/dependency
object_dir := object
output_dir := output

# Define include and source directories
include_dir := include
script_dir := script
source_dir := src

# Define common functions and flag variables
include common.gmk
include flags.gmk

# Define function install-aliases
install-aliases = cd $1 $(foreach suffix,$(alias_suffixes),&& ln -sf	\
$(addprefix $(library_file),$(shared_suffix) $(suffix)))

# Define installation macro
define install-files
	$(install) -d $(install_dirs)
	$(install) -m 644 $(includes) $(PREFIX)/include/network
	$(install) $(shared_library) $(static_library) $(PREFIX)/lib
	$(call install-aliases,$(PREFIX)/lib)
endef

# Define install dirs
includes = $(addprefix $(include_dir)/,$(addsuffix	\
/*.h,$(ostype)/network network))
install_dirs = $(addprefix $(PREFIX)/,bin include/network lib)

# Define filenames, prefixes, and suffixes
library_file = $(library_prefix)network
library_prefix = lib

ifeq "$(os_id_name)" "MINGW64_NT"
	binary_suffix = .exe
else
	binary_suffix =
endif

alias_suffix = .so.$(major)
alias_suffixes = $(alias_suffix)
depend_suffix = .dep
include_suffix = .h
object_suffix = .o
shared_suffix = .so.$(VERSION)
source_suffix = .cpp

# Define linker options rpaths and soname
ifeq "$(ld_origin)" "gnu"
rpaths = '$$ORIGIN' '$$ORIGIN/../lib'
soname = $(library_file)$(alias_suffix)
endif

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

get-dependencies = $(addprefix $(depend_dir)/,$(addsuffix	\
$(depend_suffix),$(basename $1)))
get-objects = $(addprefix $(object_dir)/,$(addsuffix	\
$(object_suffix),$(basename $1)))
get-programs = $(addprefix $(output_dir)/,$(addsuffix	\
$(binary_suffix),$(basename $1)))

# Define variables for computed file lists

sources = $(library_sources) $(test_sources)
library_sources = $(library_common_sources) $(library_native_sources)
test_sources = $(test_common_sources)

ifneq "$(os_id_name)" "MINGW64_NT"
	sources += $(unix_sources)
	library_sources += $(library_unix_sources)
	test_sources += $(test_unix_sources)
endif

objects = $(call get-objects,$(sources))

library_objects = $(call get-objects,$(library_sources))

library_aliases = $(addprefix				\
$(output_dir)/$(library_file),$(alias_suffixes))
library_mapfile = $(output_dir)/$(library_file).map
shared_library = $(output_dir)/$(library_file)$(shared_suffix)
static_library = $(output_dir)/$(library_file).a

libraries = $(library_aliases) $(shared_library) $(static_library)

program_sources = $(test_sources)

ifneq "$(os_id_dist)" "macos"
ifneq "$(os_id_type)" "ms-windows"
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
mapfiles = $(programs:$(binary_suffix)=.map) $(library_mapfile)
stackdumps = $(programs:$(binary_suffix)=.stackdump)

artifacts = $(binary_artifacts) $(text_artifacts)
binary_artifacts = $(libraries) $(objects) $(programs) TAGS
build_artifacts = $(libraries) $(listings) $(mapfiles) $(objects)	\
$(programs) sizes.txt*
text_artifacts = $(commands) $(dependencies) $(listings) $(logfiles)	\
$(mapfiles) $(stackdumps) sizes.txt*

build_dirs = $(filter-out .,$(output_dir) $(cache_dir) $(output_dir)	\
$(object_dir))
dos2unix_files = $(filter-out %$(depend_suffix),$(wildcard	\
$(text_artifacts)))

tarfile = /tmp/$(library_file).tar.gz

# Define variables for build target lists

build_targets = assert objects libraries programs sizes

ifeq "$(ctags_is_universal)" "true"
ifeq "$(call compare-versions,$(ctags_version),5.8)" "greater"
	build_targets += tags
endif
endif

all_targets = $(build_targets) sizes test

ifneq "$(os_id_dist)" "macos"
ifneq "$(os_id_type)" "ms-windows"
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

# Set virtual paths
vpath %$(source_suffix) $(source_dir)/$(ostype) $(source_dir)

# Define pseudotargets

.PHONY: all
all: $(all_targets)

.PHONY: analyze
analyze: $(sources) | $(cppcheck_dir)
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
check: test

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
	printf '%s\n' $(sort $(artifacts) $(build_dirs)) | xargs rm -rf

.PHONY: dos2unix
dos2unix:
	printf '%s\n' $(sort $(dos2unix_files)) | xargs dos2unix -q

.PHONY: install
install: $(libraries) $(programs)
	$(install-files)

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

.PHONY: tarfile
tarfile: $(tarfile)

.PHONY: test
test: $(test_programs)
	$(script_dir)/run-test-programs $^

ifeq "$(cxx_family)" "clang"
.PHONY: tidy
tidy: $(sources)
	$(CLANG_TIDY) $(sort $^) $(CLANG_TIDY_FLAGS)
endif

.PHONY: unix
unix: $(unix_programs)
	$(script_dir)/run-unix-programs $^

.SECONDARY: $(objects)

# Define targets

$(library_aliases): $(shared_library)
	$(call install-aliases,$(output_dir))

$(shared_library): $(library_objects)
	$(LINK$(object_suffix)) -o $@ $^ $(LDLIBS)

$(static_library): $(library_objects)
	rm -f $@ && $(AR) $(ARFLAGS) $@ $^

$(programs): $(firstword $(libraries))

# Define suffix rules

(%): %
	$(AR) $(ARFLAGS) $@ $<

$(output_dir)/%$(binary_suffix): $(object_dir)/%$(object_suffix)
	$(LINK$(object_suffix)) -o $@ $^ $(LDLIBS)

$(object_dir)/%$(object_suffix): %$(source_suffix)
	$(COMPILE$(source_suffix)) $(OUTPUT_OPTION) $<

$(depend_dir)/%$(depend_suffix): %$(source_suffix)
	$(CXX) $(CPPFLAGS) -MM $< | $(make_makefile) -o $@ TAGS

sizes.txt: $(sort $(shared_library) $(objects) $(programs))
	if [ -e $@ ]; then mv -f $@ $@~; fi
	size $^ >$@

TAGS:
	ctags -e $(filter -D%,$(CPPFLAGS)) -R $(include_dir) $(source_dir)

$(tarfile): $(libraries) $(programs)
	$(install-files)
	tar -C /tmp/$(library_file) -cf $(@:.gz=) .
	gzip -9f $(@:.gz=)

$(dependencies): | $(depend_dir)

$(libraries): | $(output_dir)

$(objects): | $(object_dir)

$(programs): | $(output_dir)

$(cppcheck_dir):
	mkdir -p $(cppcheck_dir)

$(depend_dir):
	mkdir -p $(depend_dir)

$(object_dir):
	mkdir -p $(object_dir)

$(output_dir):
	mkdir -p $(output_dir)

# Include dependency files
ifeq "$(filter %clean,$(MAKECMDGOALS))" "$(filter-out %clean,$(MAKECMDGOALS))"
include $(sort $(dependencies))
endif
