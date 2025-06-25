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

# $(call archive-package,ARCHIVE-DIRECTORY,ARCHIVE-FILENAME)
define archive-package
$(call install-package,$1)
tar -C $1 -acf $2 .
/bin/rm -rf $1
endef

# $(call clean-all,PATHNAMES)
define clean-all
$(strip printf '%s\n' $(sort $(wildcard $(filter-out		\
$(cache_dir)/%,$(filter-out $(coverage_dir)/%,$(filter-out	\
$(object_dir)/%,$1) $(build_dirs))))) | xargs -r /bin/rm -rf)
endef

# $(call clean-build,PATHNAMES)
define clean-build
$(strip printf '%s\n' $(sort $(wildcard $(filter-out		\
$(object_dir)/%,$1) $(object_dir))) | xargs -r /bin/rm -rf)
endef

# $(call clean-files,PATHNAMES)
define clean-files
$(strip printf '%s\n' $(sort $(wildcard $1)) | xargs -r /bin/rm -f)
endef

# $(call compile-source-to-depend,SOURCE-FILENAME,OUTPUT-FILENAME)
define compile-source-to-depend
$(strip $(COMPILE$(depend_suffix)) $1 | $(script_dir)/make-rule -d	\
$(object_dir) -o $2 TAGS)
endef

# $(call compile-source-to-object,SOURCE-FILENAMES,OBJECT-FILENAME)
define compile-source-to-object
$(strip $(COMPILE$(source_suffix)) $(OUTPUT_OPTION) $1 $(if	\
$(wildcard $(2:$(object_suffix)=.gcda)), && /bin/rm -f		\
$(2:$(object_suffix)=.gcda),))
endef

# $(call dos-to-unix,TEXT-FILENAMES)
define dos-to-unix
$(strip printf '%s\n' $(sort $(wildcard $1)) | xargs dos2unix -q)
endef

# $(call format-as-flag,FLAG-OPTION,FLAG-ARG)
format-as-flag = $(call format-xx-flag,$(as_family),$1,$2)

# $(call format-ld-flag,FLAG-OPTION,FLAG-ARG)
format-ld-flag = $(call format-xx-flag,$(ld_family),$1,$2)

# $(call format-xx-flag,TOOL,FLAG-OPTION,FLAG-ARG)
format-xx-flag = $(if $(strip $3),$(if $(filter gnu,$1),$2=$3,$2 $3),)

# $(generate-code-coverage-report)
generate-code-coverage-report = $(strip $(GCOVR) $(GCOVRFLAGS))

# $(call get-dependencies-from-sources,SOURCE-FILENAMES)
define get-dependencies-from-sources
$(addprefix $(depend_dir)/,$(addsuffix $(depend_suffix),$(basename $1)))
endef

# $(call get-install-name-flag,INSTALL-NAME-ARG)
get-install-name-flag = $(call format-ld-flag,-install_name,$1)

# $(call get-listing-flag,LISTING-FILENAME)
get-listing-flag = $(call format-as-flag,-adghln,$1)

# $(call get-objects-from-sources,SOURCE-FILENAMES)
define get-objects-from-sources
$(addprefix $(object_prefix),$(addsuffix $(object_suffix),$(basename $1)))
endef

# $(call get-prefix,DIRECTORY)
get-prefix = $(if $(filter .,$1),,$1/)

# $(call get-programs-from-sources,SOURCE-FILENAMES)
define get-programs-from-sources
$(addprefix $(output_prefix),$(addsuffix $(binary_suffix),$(basename $1)))
endef

# $(call get-rpath-flag,RPATH-ARG)
get-rpath-flag = $(foreach rpath,$1,$(call format-ld-flag,-rpath,$(rpath:/.=)))

# $(call get-soname-flag,SONAME-ARG)
get-soname-flag = $(call format-ld-flag,-soname,$1)

# $(call get-version-number,VERSION-STRING)
define get-version-number
$(word $1,$(call split-string,.,$2))
endef

# $(call install-aliases,LIBRARY-DIRECTORY,LIBRARY-STEM)
define install-aliases
$(strip $(if $(alias_suffixes),$(if $(filter .,$1),,cd $1 && )	\
$(foreach suffix,$(alias_suffixes), ln -sf $(addprefix		\
$2,$(shared_suffix) $(suffix)) && ) true,))
endef

# $(call install-binaries,BINARY-DIRECTORY)
define install-binaries
$(install) -d $1
$(install) $(sort $(test_programs)) $1
endef

# $(call install-includes,INCLUDE-DIRECTORY)
define install-includes
$(install) -d $1/$(PROJECT_NAME)
$(install) -m 644					\
$(include_dir)/$(PROJECT_NAME)/*$(include_suffix)	\
$1/$(PROJECT_NAME)
$(install) -d $1/$(api)/$(PROJECT_NAME)
$(install) -m 644						\
$(include_dir)/$(api)/$(PROJECT_NAME)/*$(include_suffix)	\
$1/$(api)/$(PROJECT_NAME)
endef

# $(call install-libraries,LIBRARY-DIRECTORY)
define install-libraries
$(install) -d $1
$(install) $(sort $(shared_library) $(static_library)) $1
$(call install-aliases,$1,$(library_stem))
endef

# $(call install-objects,OBJECT-DIRECTORY)
define install-objects
$(install) -d $1
$(install) $(object_prefix)*.gc?? $1
endef

# $(call install-sources,SOURCE-DIRECTORY)
define install-sources
$(install) -d $1
$(install) -m 644 $(source_dir)/*.cpp $1
$(install) -d $1/$(api)
$(install) -m 644 $(source_dir)/$(api)/*.cpp $1/$(api)
endef

# $(call install-package,PREFIX-DIRECTORY)
define install-package
$(call install-binaries,$1/bin)
$(call install-includes,$1/include)
$(call install-libraries,$1/lib)
$(if $(filter true,$(WITH_COVERAGE)),$(call install-objects,$1/object),)
$(if $(filter true,$(WITH_COVERAGE)),$(call install-sources,$1/src),)
endef

# $(is-shared-library)
is-shared-library = $(filter shared,$(WITH_LIBRARY))
is-static-library = $(filter static,$(WITH_LIBRARY))

# $(call join-flags,FLAG-OPTION,FLAG-ARG)
join-flags = $(if $(strip $2),$(subst $(spc),$(com),$1 $(strip $2)),)

# $(call link-objects,INPUT-FILENAMES,OUTPUT-FILENAME)
define link-objects
$(strip $(LINK$(object_suffix)) -o $2 $1 $(LOADLIBES) $(LDLIBS))
endef

# $(call run-programs,PROGRAM-FILENAME,PROGRAM-ARGUMENTS)
run-programs = $(script_dir)/run-programs $2 $(1:$(binary_suffix)=)

# $(call shell-quote SHELL-STRING)
shell-quote = $(subst $$,\$$,$1)

# $(call split-string,DELIMITER,STRING)
split-string = $(subst $1, ,$2)

# $(call tag-dirs,INCLUDE-DIRS,SOURCE-DIRS)
define tag-dirs
ctags --output-format=etags $(filter -D%,$(CPPFLAGS)) \
$(addsuffix /$(PROJECT_NAME)/*$(include_suffix),$1) \
$(addsuffix /*$(source_suffix),$2)
endef

# Local Variables:
# mode: makefile
# End:
