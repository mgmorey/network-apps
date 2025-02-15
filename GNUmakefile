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

# Define variables for customization

ifeq "$(USING_CLANG)" "true"
	CC := clang$(LLVM_SUFFIX)
	CXX := clang++$(LLVM_SUFFIX)
	CLANG_TIDY := clang-tidy$(LLVM_SUFFIX)
else
	CLANG_TIDY := false
endif

GCOVR_HTML_THEME ?= green
TMPDIR ?= /tmp

# Define variables for project
BUILD_DIR ?= .
PREFIX ?= ~/.local
VERSION ?= 0.0.1

# Define variables for language and standard
language := c++
standard := $(language)20

# Define variable for cache directory
cache_dir := .cache

# Define variables for build directories
coverage_dir := coverage
cppcheck_dir := $(cache_dir)/cppcheck
depend_dir := $(cache_dir)/dependency
object_dir := $(BUILD_DIR)/object
output_dir := $(BUILD_DIR)

# Define variables for include and source directories
include_dir := include
script_dir := script
source_dir := src

# Define common functions and flag variables
include common.gmk
include flags.gmk
include funcs.gmk

# Define variables for API and OS
api = $(if $(is_windows),windows,unix)
install = $(if $(is_unix),ginstall,install)
is_posix = $(filter $(os_id_name),FreeBSD Linux)
is_unix = $(filter $(os_id_name),Darwin FreeBSD)
is_windows = $(filter $(os_id_name),MINGW64_NT)

# Define variables for version components
major = $(word 1,$(subst ., ,$(VERSION)))
minor = $(word 2,$(subst ., ,$(VERSION)))

# Define variable for temporary directory
temporary_dir ?= $(TMPDIR:/=)/$(library_file)

# Define variables for directory lists
include_dirs = $(include_dir)/$(api) $(include_dir)
source_dirs = $(source_dir)/$(api) $(source_dir)

# Define variables for file lists
include_files = $(addsuffix /network/*.h,$(include_dirs))

# Define variables for filenames, prefixes, and suffixes
library_file = $(library_prefix)network
library_prefix = lib

alias_suffix = .so.$(major)
alias_suffixes = $(alias_suffix)
binary_suffix = $(if $(is_windows),.exe,)
depend_suffix = .dep
include_suffix = .h
object_suffix = .o
shared_suffix = .so.$(VERSION)
source_suffix = .cpp

# Define enumerated file list variables

compile_commands = compile_commands.json
cppchecklog = cppcheck.log
gcovhtml = coverage/coverage.html
gcovtext = coverage.gcov

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

library_native_sources = create-socket.cpp format-ai-error.cpp		\
format-os-error.cpp get-last-context-error.cpp get-last-os-error.cpp	\
nativecontext.cpp read.cpp set-last-context-error.cpp			\
set-last-os-error.cpp start.cpp stop.cpp write.cpp

library_unix_sources = address-sun.cpp create-socketpair.cpp		\
create-socketpairresult.cpp get-path-length.cpp get-path-pointer.cpp	\
get-sun-length.cpp get-sun-pointer.cpp nativecontext.cpp		\
to-bytestring-null.cpp to-bytestring-path.cpp to-path.cpp		\
unixsocket.cpp validate-null.cpp validate-path.cpp validate-sun.cpp

test_common_sources = test-address.cpp test-bind.cpp test-connect.cpp	\
test-context.cpp test-host.cpp test-limits.cpp test-option.cpp		\
test-parse.cpp test-socket-inet.cpp

test_unix_sources = test-socket-unix.cpp

unix_sources = unix-server.cpp unix-client.cpp

sizes = sizes.txt sizes.txt~
tags = TAGS

# Define computed file list variables

sources = $(library_sources) $(test_sources) $(if	\
$(is_posix),$(unix_sources),)
library_sources = $(library_common_sources) $(library_native_sources)	\
$(if $(filter $(api),unix),$(library_unix_sources),)
program_sources = $(test_sources) $(if $(is_posix),$(unix_sources),)
test_sources = $(test_common_sources) $(if $(filter	\
$(api),unix),$(test_unix_sources),)

objects = $(call get-objects,$(sources))

library_objects = $(call get-objects,$(library_sources))

library_aliases = $(addprefix				\
$(output_dir)/$(library_file),$(alias_suffixes))
library_mapfile = $(output_dir)/$(library_file).map
shared_library = $(output_dir)/$(library_file)$(shared_suffix)
static_library = $(output_dir)/$(library_file).a

libraries = $(library_aliases) $(shared_library) $(static_library)

program_objects = $(call get-objects,$(program_sources))

dependencies = $(call get-dependencies,$(sources))
programs = $(call get-programs,$(program_sources))
test_programs = $(call get-programs,$(test_sources))
unix_programs = $(call get-programs,$(unix_sources))

coverage_files = $(datafiles) $(notefiles)
datafiles = $(objects:$(object_suffix)=.gcda)
listings = $(objects:$(object_suffix)=.lst)
logfiles = $(programs:$(binary_suffix)=.log)
mapfiles = $(programs:$(binary_suffix)=.map) $(library_mapfile)
notefiles = $(objects:$(object_suffix)=.gcno)
stackdumps = $(programs:$(binary_suffix)=.stackdump)

artifacts = $(binary_artifacts) $(text_artifacts)
binary_artifacts = $(coverage_files) $(libraries) $(objects)	\
$(programs) $(tags) $(tarfile)
build_artifacts = $(coverage_files) $(libraries) $(objects)	\
$(programs) $(timestamps)
text_artifacts = $(compile_commands) $(cppchecklog) $(dependencies)	\
$(gcovtext) $(listings) $(logfiles) $(mapfiles) $(stackdumps)		\
$(sizes) $(timestamps)

build_dirs = $(filter-out .,$(cache_dir) $(coverage_dir)	\
$(object_dir) $(output_dir))
dos2unix_files = $(filter-out %$(depend_suffix),$(wildcard	\
$(text_artifacts)))

tarfile = $(output_dir)/$(library_file).tar.gz

# Define target list variables

all_targets = $(build_targets) test $(if $(is_posix),unix,) $(if	\
$(is_windows),dos2unix,)

build_targets = assert dataclean objects libraries programs sizes

ifeq "$(ctags_is_universal)" "true"
ifeq "$(call compare-versions,$(ctags_version),5.8)" "greater"
	build_targets += tags
endif
endif

# Define variable for timestamp files
timestamps = .test-complete $(if $(is_posix),.unix-complete,)

# Define variable for run-program arguments
program_args = $(strip $(if $(filter .,$(output_dir)),,-d $(output_dir)) -v)

# Define compiler and linker command variables
COMPILE$(source_suffix) = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK$(object_suffix) = $(CXX) $(LDFLAGS)

# Set virtual paths
vpath %$(include_suffix) $(include_dirs)
vpath %$(source_suffix) $(source_dirs)

# Define pseudotargets

.PHONY: all
all: $(all_targets)

.PHONY: analyze
analyze: $(sources) | $(cppcheck_dir)
	printf '%s\n' $^ | xargs cppcheck $(CPPCHECK_FLAGS)

.PHONY: assert
assert:
ifneq "$(sort $(library_common_sources))" "$(library_common_sources)"
	$(warning Filenames in list library_common_sources are not sorted)
endif
ifneq "$(sort $(library_unix_sources))" "$(library_unix_sources)"
	$(warning Filenames in list library_unix_sources are not sorted)
endif
ifneq "$(sort $(test_common_sources))" "$(test_common_sources)"
	$(warning Filenames in list test_common_sources are not sorted)
endif
ifneq "$(sort $(test_unix_sources))" "$(test_unix_sources)"
	$(warning Filenames in list test_unix_sources are not sorted)
endif

.PHONY: check
check: test

.PHONY: clean
clean:
	$(call clean-build,$(build_artifacts))

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

.PHONY: dataclean
dataclean:
	$(call clean-files,$(datafiles))

.PHONY: distclean
distclean:
	$(call clean-artifacts,$(artifacts))

.PHONY: gcov
gcov: $(gcovtext)

.PHONY: gcovr
gcovr: $(gcovhtml)

.PHONY: dos2unix
dos2unix:
	printf '%s\n' $(sort $(dos2unix_files)) | xargs dos2unix -q

.PHONY: install
install: $(libraries) $(programs)
	$(call install-files,$(PREFIX))

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
	if [ -e $<~ ]; then diff -b $<~ $< || true; fi

.PHONY: tags
tags: $(tags)

.PHONY: tarfile
tarfile: $(tarfile)

.PHONY: test
test: $(test_programs)
	$(call run-test-programs,$(^F),$(program_args))

ifdef CLANG_TIDY
.PHONY: tidy
tidy: $(sources)
	$(CLANG_TIDY) $(sort $^) $(CLANG_TIDY_FLAGS)
endif

ifneq "$(is_posix)" ""
.PHONY: unix
unix: $(unix_programs)
	$(call run-unix-programs,$(^F),$(program_args))
endif

.SECONDARY: $(objects)

# Define targets

$(gcovhtml): $(gcovtext)
	gcovr --html-details --html-theme $(GCOVR_HTML_THEME) --output $@

$(gcovtext): $(sources) $(timestamps)
	gcov -mo $(object_dir) -rt $(filter-out .%,$^) >$@

$(library_aliases): $(shared_library)
	$(call install-aliases,$(output_dir))

$(shared_library): $(library_objects)
	$(strip $(LINK$(object_suffix)) $^ $(LOADLIBES) $(LDLIBS) -o $@)

$(static_library): $(library_objects)
	$(strip rm -f $@ && $(AR) $(ARFLAGS) $@ $^)

$(programs): $(firstword $(libraries))

sizes.txt: $(sort $(shared_library) $(objects) $(programs))
	if [ -e $@ ]; then mv -f $@ $@~; fi
	size $^ >$@

.test-complete :$(test_programs)
	$(call run-test-programs,$(^F),$(program_args))

ifneq "$(is_posix)" ""
.unix-complete: $(unix_programs)
	$(call run-unix-programs,$(^F),$(program_args))
endif

$(tags):
	ctags -e $(filter -D%,$(CPPFLAGS)) -R $(include_dir) $(source_dir)

$(tarfile): $(libraries) $(programs)
	$(call create-tarfile,$(temporary_dir),$@)

$(gcovhtml): | $(coverage_dir)

$(dependencies): | $(depend_dir)

$(libraries): | $(output_dir)

$(objects): | $(object_dir)

$(programs): | $(output_dir)

$(coverage_dir):
	mkdir -p $@

$(cppcheck_dir):
	mkdir -p $@

$(depend_dir):
	mkdir -p $@

$(object_dir):
	mkdir -p $@

$(output_dir):
	mkdir -p $@

# Define suffix rules

$(output_dir)/%$(binary_suffix): $(object_dir)/%$(object_suffix)
	$(strip $(LINK$(object_suffix)) $^ $(LOADLIBES) $(LDLIBS) -o $@)

$(object_dir)/%$(object_suffix): %$(source_suffix)
	$(strip $(COMPILE$(source_suffix)) $(OUTPUT_OPTION) $<)

$(depend_dir)/%$(depend_suffix): %$(source_suffix)
	$(make-rule)

# Include dependency files
ifeq "$(filter %clean,$(MAKECMDGOALS))" "$(filter-out %clean,$(MAKECMDGOALS))"
include $(sort $(dependencies))
endif
