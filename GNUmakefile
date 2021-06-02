SYSTEM_PREFIX := $(shell uname -s | cut -d- -f 1)

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
	CXXFLAGS += -fno-omit-frame-pointer -g3 -O0
endif

CXXFLAGS += -std=c++98 -Wall -Werror -Wextra -Wpedantic

ifeq "$(SYSTEM_PREFIX)" "Darwin"
	CPPFLAGS += -D_DARWIN_C_SOURCE
else ifeq "$(SYSTEM_PREFIX)" "CYGWIN_NT"
	CPPFLAGS += -D_POSIX_C_SOURCE=200809L
else ifeq "$(SYSTEM_PREFIX)" "MINGW64_NT"
	LDLIBS += -lws2_32
endif

LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

all_sources = $(library_sources) $(program_sources)
library_sources = network.cpp network-address.cpp network-socket.cpp
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

network-address.o: network-address.cpp network.h

network-socket.o: network-socket.cpp network.h

test-network: test-network.o network.o network-address.o network-socket.o

test-network.o: test-network.cpp network.h
