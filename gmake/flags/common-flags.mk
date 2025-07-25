# -*- Mode: Makefile-GMake -*-

# Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

# Define variables for language and standard
language = c++
language_release = 23
standard = $(language)$(language_release)

# Define variables for character constants
com = ,
nul =
spc = $(nul) $(nul)

# Define variable for API
api = $(if $(is_windows_api),windows,unix)

# Define variable for install command
install = $(if $(is_unix),ginstall,install)

# Define variables for OS ID and kernel
os_label_id = $(word 1,$(os_labels))
os_label_kernel = $(word 2,$(os_labels))
os_macro_kernel = $(word 2,$(os_macros))

# Define predicates for API and OS
is_darwin_source = $(filter Darwin,$(os_label_kernel))
is_gnu_source = $(filter CYGWIN_NT,$(os_label_kernel))
is_posix = $(filter FreeBSD Linux,$(os_label_kernel))
is_unix = $(filter Darwin FreeBSD,$(os_label_kernel))
is_windows_api = $(filter MINGW64_NT,$(os_label_kernel))
is_windows_os = $(filter windows,$(os_label_id))

# Define predicates for compiler and linker features
is_as_listings = $(filter listings,$(as_features))
is_ld_install_name = $(filter install_name,$(ld_features))
is_ld_rpath = $(filter rpath,$(ld_features))
is_ld_soname = $(filter soname,$(ld_features))

# Define predicates for object and shared object files
is_depend = $(filter %$(depend_suffix),$@)
is_object = $(filter %$(object_suffix),$@)
is_shared = $(filter %$(shared_suffix),$@)

# Define install_name and soname linker option variables
install_name = $(library_stem)$(alias_suffix)
soname = $(library_stem)$(alias_suffix)

# Define rpath linker option component variables
origin = $(if $(filter gnu,$(ld_family)),$(if $(is_posix),$$ORIGIN,),)
rpaths = $(if $(origin),$(addprefix $(origin)/,. ../lib),)

# ARFLAGS
ARFLAGS = r

# CPPCHECK_FLAGS

CPPCHECK_FLAGS = --check-level=exhaustive				\
--checkers-report=cppcheck.log --cppcheck-build-dir=$(cppcheck_dir)	\
--enable=all --inline-suppr --quiet $(if				\
$(standard),--std=$(standard),) --suppress=missingIncludeSystem		\
$(filter-out -I$(SDKROOT)/usr/include,$(CPPFLAGS))

# CPPFLAGS

# Add OS macro flags and feature flags to CPPFLAGS
cpp_symbols = $(if $(is_darwin_source),_DARWIN_C_SOURCE,) $(if	\
$(is_gnu_source),_GNU_SOURCE,)

CPPFLAGS += $(addprefix -I,$(include_dirs:/.=))
CPPFLAGS += $(addprefix -D,$(cpp_symbols))

# CXX_ASFLAGS

# CXXFLAGS

# Add warning flags to CXXFLAGS
CXXFLAGS += -Wall -Werror -Wextra -Wshadow -Wsign-conversion

# Add position-independent code flag to CXXFLAGS
CXXFLAGS += $(if $(with-shared-library),-fPIC,)

# Add automatic variable initialization flag to CXXFLAGS
CXXFLAGS += -ftrivial-auto-var-init=zero

# Add pipe flag to CXXFLAGS
CXXFLAGS += $(if $(filter gnu,$(as_family)),-pipe,)

# Add language standard flag to CXXFLAGS
CXXFLAGS += $(if $(standard),-std=$(standard),)

# Add standard library specification flag to CXXFLAGS (for Clang compiler)
CXXFLAGS += $(if $(filter clang,$(cxx_family)),$(if $(filter libc++	\
libstdc++,$(USING_STDLIB)),-stdlib=$(USING_STDLIB),),)

# Define variables for assembler listing option flags
listing = $(@:$(object_suffix)=.lst)
asflags = $(if $(is_object),$(if $(filter true,$(WITH_LISTINGS)),$(if	\
$(is_as_listings),$(if $(filter gnu,$(cxx_family)),$(call		\
get-listing-flag,$(listing)),),),),)

# Add assembler option flags to CXXFLAGS
CXXFLAGS += $(call join-flags,-Wa,$(asflags))

# GCOVRFLAGS

GCOVRFLAGS_HTML = --html-nested --html-theme=$(HTML_THEME)	\
--html-title='$(html_title)' $(foreach				\
file,$^,--json-add-tracefile=$(file))

GCOVRFLAGS = --exclude-unreachable-branches $(if	\
$(GCOV),--gcov-executable='$(GCOV)',) $(if $(filter	\
%.json,$@),--json,$(GCOVRFLAGS_HTML)) --output=$@

# LDFLAGS

# Define variables for load map filename and linker option
map = $(@:$(if $(is_shared),$(shared_suffix),$(binary_suffix))=.map)
map_opt = $(if $(filter gnu,$(ld_family)),-Map,-map)

# Add load map linker option flag to LDFLAGS
ldflags += $(if $(filter true,$(WITH_LOADMAPS)),$(call	\
format-ld-flag,$(map_opt),$(map)),)

# Add -install_name linker option flag to LDFLAGS
ldflags += $(if $(is_shared),$(if $(is_ld_install_name),$(call	\
get-install-name-flag,$(install_name)),),)

# Add -soname linker option flag to LDFLAGS
ldflags += $(if $(is_shared),$(if $(is_ld_soname),$(call	\
get-soname-flag,$(soname)),),)

# Add -rpath linker option flag to LDFLAGS
ldflags += $(if $(is_shared),,$(if $(is_ld_rpath),$(call	\
get-rpath-flag,$(call shell-quote,$(rpaths:/.=))),))

# Add linker option flags to LDFLAGS
LDFLAGS += $(call join-flags,-Wl,$(ldflags))

# Add shared library flag to LDFLAGS
LDFLAGS += $(if $(is_shared),-shared,)

# Add linker selection flag to LDFLAGS
LDFLAGS += $(if $(filter clang,$(cxx_family)),$(if	\
$(USING_LINKER),-fuse-ld=$(USING_LINKER),),)

# LDLIBS

# Add flag to link standard library to LDLIBS (for Clang compiler)
LDLIBS += $(if $(filter clang,$(cxx_family)),$(if $(filter libc++	\
libstdc++,$(USING_STDLIB)),$(patsubst lib%,-l%,$(USING_STDLIB)),),)

# Add flag to link WinSock 2.0 library to LDLIBS (for Windows API)
LDLIBS += $(if $(is_windows_api),-lws2_32,)

# TIDY_FLAGS

TIDY_FLAGS = -config-file='.clang-tidy' -header-filter='.*' --	\
$(if $(standard),-std=$(standard),) $(CPPFLAGS)

# Local Variables:
# mode: makefile
# End:
