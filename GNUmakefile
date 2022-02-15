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

version = 0.0.1

language = c++
standard = $(language)20

install_prefix = /usr/local

cache_dir = .cache
cppbuild_dir = $(cache_dir)/cppcheck
dependency_dir = $(cache_dir)/dependency
include_dir = include
object_dir = object
script_dir = script
src_dir = src

dependency_suffix = .dep
include_suffix = .h
object_suffix = .o
source_suffix = .cpp

# Set virtual paths

vpath %$(include_suffix) $(include_dir)/network
vpath %$(source_suffix) $(src_dir)

# Include common functions and flag variables

include common.gmk
include flags.gmk

# Define file list variables

libnetwork_sources = address-sa.cpp address-sin.cpp address-sin6.cpp	\
address-sun.cpp address.cpp addresserror.cpp addrinfo.cpp		\
bind-endpoint.cpp bind-fd.cpp buffer.cpp bytestring.cpp close.cpp	\
connect-endpoint.cpp connect-fd.cpp context.cpp dispatch-os-error.cpp	\
error.cpp family.cpp fd.cpp flags.cpp format-ai-error.cpp		\
format-os-error.cpp format.cpp get-endpoint.cpp get-hostname.cpp	\
get-hosts.cpp get-last-os-error.cpp get-length.cpp get-name.cpp		\
get-peername.cpp get-sa-data.cpp get-sa-family.cpp get-sa-length.cpp	\
get-sa-pointer.cpp get-sin-addr.cpp get-sin-pointer.cpp			\
get-sin-port.cpp get-sin6-addr.cpp get-sin6-pointer.cpp			\
get-sin6-port.cpp get-size.cpp get-socket.cpp get-socketpair.cpp	\
get-sockets-endpoint.cpp get-sockets-hostname.cpp get-sockname.cpp	\
get-sun-length.cpp get-sun-path.cpp get-sun-path-length.cpp		\
get-sun-pointer.cpp hints.cpp host.cpp is-valid.cpp logicerror.cpp	\
open-endpoint.cpp open-fd.cpp oserrorresult.cpp protocol.cpp		\
reset-last-os-error.cpp runtimeerror.cpp set-last-os-error.cpp		\
socket.cpp socktype.cpp stream-address.cpp stream-addrinfo.cpp		\
stream-socket.cpp to-byte-span-sa.cpp to-byte-span-sun.cpp		\
to-byte-span-void.cpp to-byte-string-bs.cpp to-byte-string-path.cpp	\
to-byte-string-sa.cpp to-byte-string-sun.cpp to-string-bs.cpp		\
to-string-sin.cpp to-string-sin6.cpp version.cpp

test_sources = test-address.cpp test-bind.cpp test-connect.cpp	\
test-context.cpp test-host.cpp test-hostname.cpp

ifneq "$(os_name)" "MINGW64_NT"
	test_sources += test-socket.cpp
	unix_sources = unix-client.cpp unix-server.cpp
endif

sources = $(libnetwork_sources) $(test_sources) $(unix_sources)
sources_with_dir = $(addprefix $(src_dir)/,$(sources))

objects = $(addprefix $(object_dir)/,$(addsuffix	\
$(object_suffix),$(basename $(sources))))

libnetwork_objects = $(addprefix $(object_dir)/,$(addsuffix	\
$(object_suffix),$(basename $(libnetwork_sources))))

libnetwork_members = $(patsubst					\
%$(object_suffix),$(libnetwork_archive)(%$(object_suffix)),	\
$(libnetwork_objects))

ifneq "$(WITH_SHARED_OBJS)" "false"
	libnetwork_so = libnetwork.so.$(version)
	libnetwork_so_alias = $(call get-library-alias,$(libnetwork_so))
endif

libnetwork_archive = libnetwork.a

libraries = $(libnetwork_so_alias) $(libnetwork_so) $(libnetwork_archive)

program_sources = $(test_sources) $(unix_sources)

program_objects = $(addprefix $(object_dir)/,$(addsuffix	\
$(object_suffix),$(basename $(program_sources))))

ifeq "$(os_type)" "ms-windows"
	program_suffix = .exe
endif

test_programs = $(addsuffix $(program_suffix),$(basename $(test_sources)))
unix_programs = $(addsuffix $(program_suffix),$(basename $(unix_sources)))
programs = $(addsuffix $(program_suffix),$(basename $(program_sources)))

commands = compile_commands.json
dependencies = $(addprefix $(dependency_dir)/,$(subst	\
$(source_suffix),$(dependency_suffix),$(sources)))
listings = $(subst $(object_suffix),.lst,$(objects))
logfiles = $(addsuffix .log,$(basename $(programs)))
mapfiles = $(addsuffix .map,$(basename $(programs)) libnetwork)

dumps = $(addsuffix .stackdump,$(programs))
sizes = sizes.txt
tags = TAGS

binary_artifacts = $(libraries) $(objects) $(programs) $(tags)
build_artifacts = $(libraries) $(listings) $(mapfiles) $(objects)	\
$(programs) $(sizes) $(sizes)~
text_artifacts = $(commands) $(dependencies) $(dumps) $(listings)	\
$(logfiles) $(mapfiles) $(sizes) $(sizes)~

artifacts = $(binary_artifacts) $(text_artifacts)

all = $(libraries) $(programs) $(sizes)

ifeq "$(is_ctags_universal)" "true"
ifeq "$(shell $(script_dir)/compare-versions $(ctags_version) 5)" "greater"
	all += $(tags)
endif
endif

all += test

ifeq "$(os_name)" "MINGW64_NT"
	all += dos2unix
endif

# Define compiler and linker variables

COMPILE.cc = $(strip $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c)
LINK$(object_suffix) = $(strip $(CXX) $(LDFLAGS))

# Define pseudotargets

.PHONY: all
all: $(all)

.PHONY: analyze
analyze: $(cppbuild_dir)
	cppcheck $(CPPCHECK_FLAGS) $(CPPFLAGS) $(sources_with_dir)

.PHONY: check
check: $(test_programs)
	$(script_dir)/run-test-programs $^

.PHONY: check-syntax
check-syntax:
	$(CXX) -fsyntax-only $(CXXFLAGS) $(CPPFLAGS) $(CHK_SOURCES)

.PHONY: clean
clean:
	rm -f $(sort $(wildcard $(build_artifacts)))

.PHONY: commands
commands: $(commands)

.PHONY: distclean
distclean:
	rm -rf $(sort $(cache_dir) $(object_dir) $(filter-out	\
$(cache_dir)/%,$(filter-out $(object_dir)/%,$(wildcard		\
$(artifacts)))))

.PHONY: dos2unix

dos2unix:
	dos2unix $(sort $(filter-out %$(dependency_suffix),$(wildcard	\
$(text_artifacts))))

.PHONY: install
install: $(libraries)
	install $(libraries) $(intsall_prefix)/lib
	install $(include_dir)/network/* $(install_prefix)/include

.PHONY: libraries
libraries: $(libraries)

.PHONY: programs
programs: $(programs)

.PHONY: realclean
realclean: distclean

.PHONY: sizes
sizes: $(sizes)
	test -e $<~ && diff -b $<~ $< || true

.PHONY: test
test: check

.PHONY: tags
tags: $(tags)

.PHONY: tidy
tidy: $(sort $(sources))
	clang-tidy$(LLVM_SUFFIX) $^ $(TIDY_FLAGS)

.PHONY: unix
unix:
	./unix-server & (sleep 1; ./unix-client 2 2; ./unix-client DOWN)

.SECONDARY: $(objects)

# Define targets

$(libnetwork_so): $(sort $(libnetwork_objects))
	$(LINK$(object_suffix)) -o $@ $^ $(LDLIBS)

$(libnetwork_so_alias): $(libnetwork_so)
	ln -sf $< $@

ifeq "$(USING_ARCHIVE_MEMBER_RULE)" "true"
$(libnetwork_archive): $(sort $(libnetwork_members))
else
$(libnetwork_archive): $(sort $(libnetwork_objects))
	rm -f $@ && $(AR) $(ARFLAGS) $@ $^
endif

$(programs): $(word 1,$(libraries))

# Define suffix rules

(%): %
	$(AR) $(ARFLAGS) $@ $<

%$(program_suffix): $(object_dir)/%$(object_suffix)
	$(LINK$(object_suffix)) -o $@ $^ $(LDLIBS)

$(dependency_dir)/%$(dependency_suffix): %$(source_suffix)
	$(CXX) $(CPPFLAGS) -MM $< | $(script_dir)/make-makefile -f $(tags) -o $@

$(object_dir)/%$(object_suffix): %$(source_suffix)
	$(COMPILE$(source_suffix)) $(OUTPUT_OPTION) $<

$(commands): $(MAKEFILE_LIST)
	bear -- $(MAKE_COMMAND) $(MFLAGS) clean programs

$(tags):
	ctags -e $(filter -D%,$(CPPFLAGS)) -R include $(src_dir)

sizes.txt: $(sort $(libnetwork_so) $(objects) $(programs))
	if [ -e $@ ]; then mv -f $@ $@~; fi
	size $^ >$@

$(dependencies): | $(dependency_dir)

$(objects): | $(object_dir)

$(cppbuild_dir):
	mkdir -p $(cppbuild_dir)

$(dependency_dir):
	mkdir -p $(dependency_dir)

$(object_dir):
	mkdir -p $(object_dir)

# Include dependency files

ifeq "$(filter %clean,$(MAKECMDGOALS))" "$(filter-out %clean,$(MAKECMDGOALS))"
include $(dependencies)
endif
