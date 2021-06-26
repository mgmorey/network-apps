SYSTEM_PREFIX := $(shell uname -s | cut -d- -f 1)

ifeq "$(SYSTEM_PREFIX)" "FreeBSD"
	CPPFLAGS += -I/usr/local/include
	LDLIBS += -L/usr/local/lib
endif

ifdef NDEBUG
        CPPFLAGS += -D_FORTIFY_SOURCE=2
        CXXFLAGS += -O2
else
ifeq "$(USING_DMALLOC)" "true"
	CPPFLAGS += -DDMALLOC -DMALLOC_FUNC_CHECK
	LDLIBS += -ldmalloc
else ifeq "$(USING_LIBASAN)" "true"
	CXXFLAGS += -fsanitize=address
	LDLIBS += -lasan
endif
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

%.d: %.cpp
	$(CXX) -M $(CPPFLAGS) $< >$@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,' $@.$$$$ >$@; \
	rm $@.$$$$

executable_sources = test-address.cpp test-hostname.cpp
library_sources = network-address.cpp network-addresses.cpp \
network-addrinfo.cpp network-buffer.cpp network-connect.cpp \
network-endpoint.cpp network-family.cpp network-flags.cpp \
network-format.cpp network-hostname.cpp network-name.cpp \
network-protocol.cpp network-result.cpp network-sockaddr.cpp \
network-socket.cpp network-sockets.cpp network-socktype.cpp

ifndef HTTP_PROXY
	executable_sources += test-connect.cpp
endif

libraries = libnetwork.a
sources = $(executable_sources) $(library_sources)

dependencies = $(subst .cpp,.d,$(sources))
executables = $(subst .cpp,,$(executable_sources))

executable_objects = $(subst .cpp,.o,$(executable_sources))
library_objects = $(subst .cpp,.o,$(library_sources))
objects = $(library_objects) $(executable_objects)

.PHONY:	all
all: TAGS $(executables)

.PHONY:	clean
clean:
	rm -f $(executables) $(libraries) $(objects)

.PHONY: realclean
realclean: clean
	rm -f TAGS $(dependencies)

.PHONY:	test
test: $(executables)
	for f in $^; do ./$$f; done

ifneq "$(SYSTEM_PREFIX)" "Darwin"
.INTERMEDIATE: $(objects)
endif

TAGS: *.cpp *.h
	etags $^

$(executables): libnetwork.a

libnetwork.a: $(patsubst %.o,libnetwork.a(%.o),$(library_objects))

include $(dependencies)
