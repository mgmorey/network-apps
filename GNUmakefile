SYSTEM_PREFIX := $(shell uname -s | cut -d- -f 1)

ifeq "$(SYSTEM_PREFIX)" "FreeBSD"
	CPPFLAGS += -I/usr/local/include
	LDLIBS += -L/usr/local/lib
endif

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

LDLIBS += -L. -lnetwork

LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

all_sources = $(library_sources) $(program_sources)
library_sources = network-address.cpp network-addresses.cpp \
network-connect.cpp network-endpoint.cpp network-hostname.cpp \
network-socket.cpp network-sockets.cpp network-string.cpp
program_sources = test-network.cpp

ifndef HTTP_PROXY
	program_sources += test-connect.cpp
endif

all_objects = $(library_objects) $(program_objects)
library_objects = $(subst .cpp,.o,$(library_sources))
program_objects = $(subst .cpp,.o,$(program_sources))
libraries = libnetwork.a
programs = $(subst .cpp,,$(program_sources))

.PHONY:	all
all: $(programs)

.PHONY:	clean
clean:
	@/bin/rm -f $(all_objects) $(libraries) $(programs)

.PHONY:	test
test: $(programs)
	@for f in $^; do ./$$f; done

libnetwork.a: \
libnetwork.a(network-address.o) \
libnetwork.a(network-addresses.o) \
libnetwork.a(network-connect.o) \
libnetwork.a(network-endpoint.o) \
libnetwork.a(network-hostname.o) \
libnetwork.a(network-socket.o) \
libnetwork.a(network-sockets.o) \
libnetwork.a(network-string.o)

test-connect: test-connect.o libnetwork.a

test-network: test-network.o libnetwork.a

include $(subst .cpp,.d,$(all_sources))

%.d: %.cpp
	@$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	/bin/rm -f $@.$$$$
