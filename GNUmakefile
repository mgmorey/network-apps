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

# Define variables for project defaults
PROJECT_NAME = network
VERSION ?= 0.0.1

# Define variables for build defaults
BUILD_DIR ?= .
BUILD_TYPE ?= Debug
INSTALL_PREFIX ?= ~/.local
HTML_THEME ?= green
TMPDIR ?= /tmp

# Define variables for include and script directories
include_dir = include
script_dir = script

# Include function and variable definitions
include $(include_dir)/commands.gmk
include $(include_dir)/features.gmk
include $(include_dir)/flags.gmk
include $(include_dir)/funcs.gmk

# Define variable for cache directory
cache_dir = .cache

# Define variables for directories
coverage_dir = coverage
cppcheck_dir = $(cache_dir)/cppcheck
depend_dir = $(cache_dir)/dependency

# Define variables for output directory/prefix
output_dir = $(BUILD_DIR)
output_prefix = $(call get-prefix,$(output_dir))

# Define variable for object directory
object_dir = $(output_prefix)object

# Define variables for source directory
source_dir = src

# Define variables for version components
major = $(call get-version-number,1,$(VERSION))
minor = $(call get-version-number,2,$(VERSION))
patch = $(call get-version-number,3,$(VERSION))

# Define variables for directory lists
include_dirs = $(addprefix $(include_dir)/,$(api) .)
source_dirs = $(addprefix $(source_dir)/,$(api) .)

# Define variable for include file list
include_files = $(addsuffix /$(PROJECT_NAME)/*.h,$(include_dirs:/.=))

# Define variable for library prefix
library_prefix = lib

# Define variables for filename suffixes
alias_suffix = $(if $(is_windows_api),,.so.$(major))
alias_suffixes = $(alias_suffix)
binary_suffix = $(if $(is_windows_api),.exe,)
depend_suffix = .dep
include_suffix = .h
object_suffix = .o
shared_suffix = $(if $(is_windows_api),.dll,.so.$(VERSION))
source_suffix = .cpp

# Define variable for library file stem
library_stem = $(library_prefix)$(PROJECT_NAME)

# Define variable for package filename
package = $(output_prefix)$(library_stem).tar.gz

# Define variable for temporary directory
temporary_dir ?= $(TMPDIR:/=)/$(library_stem)

# Define variabled for enumerated file lists

compile_commands = compile_commands.json
coverage_html = coverage/coverage.html
cppchecklog = cppcheck.log

library_common_sources = accept.cpp address-sa.cpp address-sin.cpp	\
address-sin6.cpp address.cpp addresserror.cpp addresslist.cpp		\
argumentdata.cpp binarybuffer.cpp bind-endpoint.cpp close.cpp		\
commonsocket.cpp connect-endpoint.cpp create-socket-handle.cpp		\
create-socket-hints.cpp create-socketresult.cpp error.cpp		\
familyerror.cpp format.cpp get-endpoint.cpp get-endpointresult.cpp	\
get-hostname.cpp get-hostnameresult.cpp get-name.cpp			\
get-nameresult.cpp get-operands.cpp get-option.cpp get-options.cpp	\
get-sa-family.cpp get-sa-length.cpp get-sa-pointer.cpp			\
get-sa-span.cpp get-sin-addr.cpp get-sin-pointer.cpp get-sin-port.cpp	\
get-sin6-addr.cpp get-sin6-pointer.cpp get-sin6-port.cpp listen.cpp	\
logicerror.cpp open-endpoint.cpp open-handle.cpp open.cpp		\
oserrorresult.cpp parse-argumentspan.cpp parse.cpp quote.cpp		\
rangeerror.cpp reset-api-error.cpp reset-os-error.cpp			\
runtimeerror.cpp shutdown.cpp socketdata.cpp socketfamily.cpp		\
socketflags.cpp sockethost.cpp socketlimits.cpp socketprotocol.cpp	\
sockettype.cpp start-context.cpp stream-address.cpp			\
stream-addrinfo.cpp stream-context.cpp stream-socket.cpp		\
stream-version.cpp template.cpp textbuffer.cpp to-bytestring-void.cpp	\
to-string-in-addr.cpp to-string-in6-addr.cpp to-string-span-byte.cpp	\
to-string-void.cpp validate-bs.cpp validate-sa.cpp validate-sin.cpp	\
validate-sin6.cpp

library_native_sources = create-socket.cpp format-ai-error.cpp	\
format-os-error.cpp get-api-error.cpp get-os-error.cpp		\
nativecontext.cpp read.cpp set-api-error.cpp set-os-error.cpp	\
start.cpp stop.cpp write.cpp

library_unix_sources = address-sun.cpp create-socketpair.cpp		\
create-socketpairresult.cpp get-path-length.cpp get-path-pointer.cpp	\
get-sun-length.cpp get-sun-pointer.cpp nativecontext.cpp		\
to-bytestring-null.cpp to-bytestring-path.cpp to-path.cpp		\
unixsocket.cpp validate-path.cpp validate-sun.cpp

test_common_sources = test-address.cpp test-bind.cpp test-connect.cpp	\
test-context.cpp test-errors.cpp test-host.cpp test-hostname.cpp	\
test-limits.cpp test-option.cpp test-parse.cpp test-socket-inet.cpp	\
test-version.cpp

test_unix_sources = test-socket-unix.cpp

unix_sources = unix-server.cpp unix-client.cpp

sizes = sizes.txt sizes.txt~
tags = TAGS

# Define variables for computed file lists

sources = $(library_sources) $(test_sources) $(if	\
$(is_posix),$(unix_sources),)
library_sources = $(library_common_sources) $(library_native_sources)	\
$(if $(filter unix,$(api)),$(library_unix_sources),)
program_sources = $(test_sources) $(if $(is_posix),$(unix_sources),)
test_sources = $(test_common_sources) $(if $(filter	\
unix,$(api)),$(test_unix_sources),)

objects = $(call get-objects-from-sources,$(sources))

library_objects = $(call get-objects-from-sources,$(library_sources))

library_aliases = $(addprefix				\
$(output_prefix)$(library_stem),$(alias_suffixes))
library_mapfile = $(output_prefix)$(library_stem).map
shared_library = $(output_prefix)$(library_stem)$(shared_suffix)
static_library = $(output_prefix)$(library_stem).a

libraries = $(library_aliases) $(shared_library) $(static_library)

program_objects = $(call get-objects-from-sources,$(program_sources))

dependencies = $(call get-dependencies-from-sources,$(sources))

programs = $(test_programs) $(if $(is_posix),$(unix_programs),)
test_programs = $(call get-programs-from-sources,$(test_sources))
unix_programs = $(call get-programs-from-sources,$(unix_sources))

coverage_files = $(datafiles) $(notefiles)
datafiles = $(objects:$(object_suffix)=.gcda)
listings = $(objects:$(object_suffix)=.lst)
mapfiles = $(programs:$(binary_suffix)=.map) $(library_mapfile)
notefiles = $(objects:$(object_suffix)=.gcno)
stackdumps = $(programs:$(binary_suffix)=.stackdump)

logfiles = $(test_logfiles) $(if $(is_posix),$(unix_logfiles),)
test_logfiles = $(test_programs:$(binary_suffix)=.log)
unix_logfiles = $(unix_programs:$(binary_suffix)=.log)

artifacts = $(binary_artifacts) $(text_artifacts)
binary_artifacts = $(coverage_files) $(libraries) $(objects)	\
$(package) $(programs) $(tags)
text_artifacts = $(compile_commands) $(cppchecklog) $(dependencies)	\
$(coverage_gcov) $(listings) $(logfiles) $(mapfiles) $(stackdumps)	\
$(sizes)

build_artifacts = $(coverage_files) $(libraries) $(mapfiles)	\
$(objects) $(programs) $(sizes)

build_dirs = $(filter-out .,$(cache_dir) $(coverage_dir)	\
$(object_dir) $(output_dir))
dos2unix_files = $(filter-out %$(depend_suffix),$(wildcard	\
$(text_artifacts)))

# Define target list variables

all_targets = $(build_targets) test $(if $(filter	\
true,$(WITH_COVERAGE)),$(if $(GCOVR),gcovr,),) $(if	\
$(is_windows_api),dos2unix,)

build_targets = assert objects libraries programs sizes $(if $(is_uctags),tags)

# Define variable for run-program arguments
program_args = $(strip $(if $(filter .,$(output_dir)),,-d $(output_dir)) -v)

# Define variables for compiler and linker commands
COMPILE$(source_suffix) = $(if $(CXX),$(CXX),c++) $(sort $(CXXFLAGS)	\
$(CPPFLAGS) $(TARGET_ARCH)) -c
LINK$(object_suffix) = $(if $(CXX),$(CXX),c++) $(sort $(LDFLAGS))

# Set virtual paths
vpath %$(include_suffix) $(include_dirs:/.=)
vpath %$(source_suffix) $(source_dirs:/.=)

# Define pseudotargets

.PHONY: all
all: $(all_targets)

.PHONY: buildonly
buildonly: $(build_targets)

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

.PHONY: distclean
distclean:
	$(call clean-all,$(artifacts))

.PHONY: gcovr
gcovr: $(coverage_html)

.PHONY: dos2unix
dos2unix:
	$(call dos-to-unix,$(dos2unix_files))

.PHONY: install
install: $(libraries) $(programs)
	$(call install-package,$(INSTALL_PREFIX))

.PHONY: libraries
libraries: $(libraries)

.PHONY: objects
objects: $(sort $(objects))

.PHONY: package
package: $(package)

.PHONY: programs
programs: $(sort $(programs))

.PHONY: realclean
realclean: distclean

.PHONY: sizes
sizes: sizes.txt
	@if [ -e $<~ ]; then diff -b $<~ $< || true; fi

.PHONY: tags
tags: $(tags)

.PHONY: test
test: $(logfiles)

ifneq "$(TIDY)" ""
.PHONY: tidy
tidy: $(sources)
	$(TIDY) $(sort $^) $(TIDY_FLAGS)
endif

.SECONDARY: $(objects)

# Define targets

$(coverage_html): $(logfiles)
	$(strip $(GCOVR) $(GCOVRFLAGS) --output=$@)

$(library_aliases): $(shared_library)
	$(call install-aliases,$(output_dir),$(library_stem))

$(package): $(libraries) $(programs)
	$(call archive-package,$(temporary_dir),$@)

$(shared_library): $(library_objects)
	$(call link-objects,$^,$@)

$(static_library): $(library_objects)
	$(strip rm -f $@ && $(AR) $(ARFLAGS) $@ $^)

$(programs): $(firstword $(libraries))

$(logfiles): $(programs)
	$(call run-programs,$(^F),$(program_args))

sizes.txt: $(shared_library) $(objects) $(programs)
	if [ -e $@ ]; then mv -f $@ $@~; fi
	size $(sort $^) >$@

$(tags):
	ctags -e $(filter -D%,$(CPPFLAGS)) -R $(include_dir) $(source_dir)

$(coverage_html): | $(coverage_dir)

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

$(output_prefix)%$(binary_suffix): $(object_dir)/%$(object_suffix)
	$(call link-objects,$^,$@)

$(object_dir)/%$(object_suffix): %$(source_suffix)
	$(call compile-source,$<,$@)

$(depend_dir)/%$(depend_suffix): %$(source_suffix)
	$(call make-dependency-rule,$<,$@)

# Include dependency files
ifeq "$(filter %clean,$(MAKECMDGOALS))" "$(filter-out %clean,$(MAKECMDGOALS))"
include $(sort $(dependencies))
endif
