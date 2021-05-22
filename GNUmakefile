LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

ifeq "$(USING_DMALLOC)" "true"
	CPPFLAGS += -DDMALLOC -DMALLOC_FUNC_CHECK
	LDLIBS += -ldmalloc
endif

ifeq "$(USING_LIBASAN)" "true"
	CXXFLAGS += -fsanitize=address
	LDLIBS += -lasan
endif

ifdef NDEBUG
        CPPFLAGS += -D_FORTIFY_SOURCE=2
        CXXFLAGS += -O2
else
	CXXFLAGS += -fno-omit-frame-pointer -g3 -Og
endif

CXXFLAGS += -std=c++98 -Wall -Werror -Wextra -Wpedantic

all_sources = $(library_sources) $(program_sources)
library_sources = network.cpp
program_sources = test-network.cpp

all_objects = $(library_objects) $(program_objects)
library_objects = $(subst .cpp,.o,$(library_sources))
program_objects = $(subst .cpp,.o,$(program_sources))
programs = $(subst .cpp,,$(program_sources))

.PHONY:	all
all: $(programs)

.PHONY:	clean
clean:
	@/bin/rm -f $(all_objects) $(programs)

.PHONY:	test
test: $(programs)
	@for f in $^; do ./$$f; done

network.o: network.cpp network.h

test-network: test-network.o network.o

test-network.o: test-network.cpp network.h
