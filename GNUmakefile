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

# Define variables for project name and version
PROJECT_NAME := network
VERSION ?= 0.0.1

# Define variables for other project defaults
BUILD_DIR ?= .
BUILD_TYPE ?= Default
HTML_THEME ?= green
INSTALL_PREFIX ?= ~/.local
TMPDIR ?= /tmp
WITH_FORTIFY_SOURCE ?= 3
WITH_LIBRARY ?= shared

# Define variables for include, script, and source directories
include_dir := include
script_dir := script
source_dir := src

# Include variable definitions for toolchain commands/features and
# functions.
include gmake/common-commands.mk
include gmake/common-features.mk
include gmake/common-functions.mk
include gmake/flags.mk

# Define variable for code coverage report title
html_title = $(call to-upper,$(PROJECT_NAME)) Code Coverage Report

# Define variables for version components
major = $(call get-version-number,1,$(VERSION))
minor = $(call get-version-number,2,$(VERSION))
patch = $(call get-version-number,3,$(VERSION))

# Define variables for coverage, cache, cppcheck, dependency,
# assembly, object, and output directories

cache_dir = .cache
coverage_dir = coverage

cppcheck_dir = $(cache_prefix)cppcheck
depend_dir = $(cache_prefix)dependency
object_dir = $(output_prefix)object
output_dir = $(BUILD_DIR)

# Define variables for cache, depend, object, and output prefixes
cache_prefix = $(call get-prefix,$(cache_dir))
depend_prefix = $(call get-prefix,$(depend_dir))
object_prefix = $(call get-prefix,$(object_dir))
output_prefix = $(call get-prefix,$(output_dir))

# Define variable for build directory list
build_dirs = $(filter-out .,$(cache_dir) $(coverage_dir)	\
$(object_dir) $(output_dir))

# Define variables for other directory lists
include_dirs = $(addprefix $(include_dir)/,$(api) .)
source_dirs = $(addprefix $(source_dir)/,$(api) .)

# Define variables for filename suffixes
alias_suffix = $(if $(is_windows_api),,.so.$(major))
alias_suffixes = $(alias_suffix)
assembly_suffix = .s
binary_suffix = $(if $(is_windows_api),.exe,)
depend_suffix = .dep
include_suffix = .hpp
log_suffix = .log
object_suffix = .o
shared_suffix = $(if $(is_windows_api),.dll,.so.$(VERSION))
source_suffix = .cpp

# Define variables for enumerated file lists

compile_commands = compile_commands.json
compile_flags = compile_flags.txt
coverage_alias = coverage.html
coverage_html = $(coverage_dir)/coverage.html
coverage_json = coverage.json
cppcheck_log = cppcheck$(log_suffix)

library_common_sources = accept-socket.cpp accept-socketcore.cpp	\
acceptdata.cpp address-sa.cpp address-sin.cpp address-sin6.cpp		\
address.cpp addresserror.cpp addresslist.cpp argumentdata.cpp		\
binarybuffer.cpp bind-endpoint.cpp close.cpp connect-endpoint.cpp	\
create-runtime.cpp create-socket-acceptdata.cpp				\
create-socket-handle.cpp create-socket-hints.cpp			\
create-socketresult.cpp error.cpp familyerror.cpp format.cpp		\
get-endpoint.cpp get-endpointresult.cpp get-hostname-bool.cpp		\
get-hostname-charspan.cpp get-hostnameresult.cpp get-name.cpp		\
get-namehandler.cpp get-nameresult.cpp get-openhandler.cpp		\
get-operands.cpp get-option.cpp get-options.cpp get-runtime.cpp		\
get-sa-family.cpp get-sa-length.cpp get-sa-pointer.cpp			\
get-sa-span.cpp get-sin-addr.cpp get-sin-pointer.cpp get-sin-port.cpp	\
get-sin6-addr.cpp get-sin6-pointer.cpp get-sin6-port.cpp		\
inetsocket.cpp listen.cpp logicerror.cpp open-endpoint.cpp		\
open-handle.cpp oserror.cpp parse-argumentspan.cpp parse.cpp		\
quote.cpp rangeerror.cpp reset-api-error.cpp reset-os-error.cpp		\
run.cpp runtimeerror.cpp shutdown.cpp socketapi.cpp socketcore.cpp	\
socketdata.cpp socketfamily.cpp socketflags.cpp sockethost.cpp		\
socketlimits.cpp socketprotocol.cpp sockettype.cpp stream-address.cpp	\
stream-addrinfo.cpp stream-socket.cpp stream-version.cpp		\
textbuffer.cpp to-bytestring-void.cpp to-string-bytespan.cpp		\
to-string-in-addr.cpp to-string-in6-addr.cpp to-string-runtime.cpp	\
to-string-void.cpp validate-bs.cpp validate-sa.cpp validate-sin.cpp	\
validate-sin6.cpp

library_native_sources = apistate.cpp create-socket-socketdata.cpp	\
format-ai-error.cpp format-os-error.cpp get-api-error.cpp		\
get-os-error.cpp read.cpp set-api-error.cpp set-os-error.cpp		\
start.cpp stop.cpp write.cpp

library_unix_sources = address-sun.cpp bind-path.cpp connect-path.cpp	\
create-socketpair.cpp create-socketpairresult.cpp get-path-length.cpp	\
get-path-pointer.cpp get-sun-length.cpp get-sun-pointer.cpp		\
to-bytestring-path.cpp to-path.cpp unixsocket.cpp validate-path.cpp	\
validate-sun.cpp

test_common_sources = test-address.cpp test-bind.cpp test-connect.cpp	\
test-errors.cpp test-host.cpp test-hostname.cpp test-option.cpp		\
test-parse.cpp test-runtime.cpp test-socket-api.cpp			\
test-socket-data.cpp test-socket-inet.cpp

test_unix_sources = test-socket-pair.cpp test-socket-unix.cpp

unix_sources = unix-server.cpp unix-client.cpp

sizes = sizes.txt sizes.txt~

# Define variables for computed filenames

library_mapfile = $(output_prefix)$(library_stem).map
library_stem = lib$(PROJECT_NAME)
shared_library = $(output_prefix)$(library_stem)$(shared_suffix)
static_library = $(output_prefix)$(library_stem).a
package = $(output_prefix)$(library_stem).tar.gz

# Define variables for computed file lists

sources = $(library_sources) $(test_sources) $(if $(is_posix),$(unix_sources),)
library_sources = $(library_common_sources) $(library_native_sources)	\
$(if $(filter unix,$(api)),$(library_unix_sources),)
program_sources = $(test_sources) $(if $(is_posix),$(unix_sources),)
test_sources = $(test_common_sources) $(if $(filter	\
unix,$(api)),$(test_unix_sources),)

assemblies = $(call get-assemblies-from-sources,$(sources))
dependencies = $(call get-dependencies-from-sources,$(sources))

objects = $(call get-objects-from-sources,$(sources))
library_objects = $(call get-objects-from-sources,$(library_sources))
program_objects = $(call get-objects-from-sources,$(program_sources))

programs = $(test_programs) $(if $(is_posix),$(unix_programs),)
test_programs = $(call get-programs-from-sources,$(test_sources))
unix_programs = $(call get-programs-from-sources,$(unix_sources))

library_aliases = $(addprefix $(output_prefix)$(library_stem),$(alias_suffixes))
libraries = $(if $(with_shared_library),$(library_aliases)	\
$(shared_library),) $(if $(with_static_library),$(static_library),)

gcov_files = $(gcov_datafiles) $(gcov_notefiles)
gcov_datafiles = $(objects:$(object_suffix)=.gcda)
gcov_notefiles = $(objects:$(object_suffix)=.gcno)
listings = $(objects:$(object_suffix)=.lst)
mapfiles = $(programs:$(binary_suffix)=.map) $(library_mapfile)
stackdumps = $(programs:$(binary_suffix)=.stackdump)

logfiles = $(test_logfiles) $(if $(is_posix),$(unix_logfiles),)
test_logfiles = $(test_programs:$(binary_suffix)=$(log_suffix))
unix_logfiles = $(unix_programs:$(binary_suffix)=$(log_suffix))

artifacts = $(binary_artifacts) $(text_artifacts)
binary_artifacts = $(gcov_files) $(libraries) $(objects) $(package)	\
$(programs) TAGS
text_artifacts = $(assemblies) $(coverage_alias) $(coverage_html)	\
$(coverage_json) $(compile_commands) $(compile_flags) $(cppcheck_log)	\
$(dependencies) $(listings) $(logfiles) $(mapfiles) $(stackdumps)	\
$(sizes)

build_artifacts = $(compile_commands) $(compile_flags) $(gcov_files)	\
$(libraries) $(listings) $(mapfiles) $(objects) $(programs) $(sizes)

dos2unix_files = $(filter-out %$(depend_suffix),$(wildcard	\
$(text_artifacts)))

# Define variable for run-program arguments
program_args = $(strip $(if $(filter .,$(output_dir)),,-d $(output_dir)) -v)

# Define target list variables

all_targets = $(build_targets) test $(if $(filter Debug	\
Default,$(BUILD_TYPE)),$(if $(GCOVR),$(if $(filter	\
true,$(WITH_COVERAGE)),$(gcovr_targets),),)) $(if	\
$(is_windows_api),dos2unix,)

build_targets = assert compile-flags objects libraries programs sizes	\
$(if $(is_uctags),tags)
gcovr_targets = gcovr-html gcovr-json

# Define variables for compiler and linker commands
COMPILE$(depend_suffix) = $(CXX) $(sort $(CPPFLAGS) -MM)
COMPILE$(source_suffix) = $(CXX) $(sort $(CXXFLAGS) $(CPPFLAGS)	\
$(TARGET_ARCH)) -c
LINK$(object_suffix) = $(if $(CXX),$(CXX),c++) $(sort $(LDFLAGS))

# Set virtual paths
vpath %$(include_suffix) $(include_dirs:/.=)
vpath %$(source_suffix) $(source_dirs:/.=)

# Define pseudotargets

.PHONY: all
all: $(all_targets)

.PHONY: assembly
assembly: $(assemblies)

.PHONY: buildonly
buildonly: $(build_targets)

.PHONY: analyze
analyze: $(sources) | $(cppcheck_dir)
	cppcheck $(CPPCHECK_FLAGS) $(sort $^)

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

.PHONY: compile-flags
compile-flags: $(compile_flags)

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

.PHONY: gcovr-html
gcovr-html: $(coverage_html)

.PHONY: gcovr-json
gcovr-json: $(coverage_json)

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
tags: TAGS

.PHONY: test
test: $(logfiles)

ifneq "$(TIDY)" ""
.PHONY: tidy
tidy: $(sources)
	$(TIDY) $(sort $^) $(TIDY_FLAGS)
endif

.SECONDARY: $(objects)

# Define targets

$(compile_flags): GNUmakefile gmake/flags/*.mk gmake/flags/build/*.mk
	printf '%s\n' $(sort $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH)) >$@

$(coverage_html): $(coverage_json)
	$(generate-code-coverage-report)
	ln -sf $@ .

$(coverage_json): $(logfiles)
	$(generate-code-coverage-report)

$(library_aliases): $(shared_library)
	$(call install-aliases,$(output_dir),$(library_stem))

$(package): $(libraries) $(programs)
	$(call archive-package,$(TMPDIR:/=)/$(library_stem),$@)

$(shared_library): $(library_objects)
	$(call link-objects,$^,$@)

$(static_library): $(library_objects)
	$(strip rm -f $@ && $(AR) $(ARFLAGS) $@ $^)

$(programs): $(firstword $(libraries))

$(unix_logfiles): $(unix_programs)
	$(call run-programs,$(^F),$(program_args))

sizes.txt: $(if $(with_shared_library),$(shared_library) $(objects)) $(programs)
	@if [ -e $@ ]; then mv -f $@ $@~; fi
	size $(sort $^) >$@

TAGS:
	$(call tag-dirs,$(include_dirs:/.=),$(source_dirs:/.=))

$(assemblies): | $(object_dir)

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

$(depend_prefix)%$(depend_suffix): %$(source_suffix)
	$(call compile-source-to-depend,$<,$@)

$(object_prefix)%$(assembly_suffix): %$(source_suffix)
	$(call compile-source-to-assembly,$<,$@)

$(object_prefix)%$(object_suffix): %$(source_suffix)
	$(call compile-source-to-object,$<,$@)

$(output_prefix)%$(binary_suffix): $(object_prefix)%$(object_suffix)
	$(call link-objects,$^,$@)

test-%$(log_suffix): $(output_prefix)test-%$(binary_suffix)
	$(call run-program,$(^F))

# Include dependency files
ifeq "$(filter distclean,$(MAKECMDGOALS))" ""
include $(sort $(dependencies))
endif
