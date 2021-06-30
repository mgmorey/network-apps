SYSTEM_PREFIX := $(shell uname -s | cut -d- -f 1)

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

ifeq "$(SYSTEM_PREFIX)" "Darwin"
	CPPFLAGS += -D_DARWIN_C_SOURCE -DHAVE_SOCKADDR_SA_LEN
else ifeq "$(SYSTEM_PREFIX)" "CYGWIN_NT"
	CPPFLAGS += -D_POSIX_C_SOURCE=200809L
else ifeq "$(SYSTEM_PREFIX)" "FreeBSD"
	CPPFLAGS += -DHAVE_SOCKADDR_SA_LEN -I/usr/local/include
	LDLIBS += -L/usr/local/lib
else ifeq "$(SYSTEM_PREFIX)" "MINGW64_NT"
	LDLIBS += -lws2_32
endif

CPPFLAGS += -Iinclude
CXXFLAGS += -std=c++98 -Wall -Werror -Wextra -Wpedantic

LINK.o = $(CXX) $(LDFLAGS)

prefix=/usr/local

executable_sources = test-address.cpp test-hostname.cpp
library_sources = network-addrinfo.cpp network-buffer.cpp network-close.cpp \
network-connect.cpp network-endpoint.cpp network-family.cpp network-flags.cpp \
network-format.cpp network-host.cpp network-hosts.cpp network-hostname.cpp \
network-name.cpp network-protocol.cpp network-result.cpp network-sockaddr.cpp \
network-socket.cpp network-sockets.cpp network-socktype.cpp network-unique.cpp

ifndef HTTP_PROXY
	executable_sources += test-connect.cpp
endif

ifneq "$(SYSTEM_PREFIX)" "MINGW64_NT"
	executable_sources += test-socket.cpp unix-client.cpp unix-server.cpp
endif

sources = $(executable_sources) $(library_sources)

dependencies = $(addprefix tmp/,$(subst .cpp,.dep,$(sources)))
executables = $(subst .cpp,,$(executable_sources))
libraries = libnetwork.a

executable_objects = $(addprefix tmp/,$(subst .cpp,.o,$(executable_sources)))
library_objects = $(addprefix tmp/,$(subst .cpp,.o,$(library_sources)))
objects = $(library_objects) $(executable_objects)

all: TAGS $(executables)

clean:
	rm -f $(executables) $(libraries) $(objects)

realclean: clean
	rm -f $(dependencies) TAGS

test: $(executables)
	for f in $^; do if expr $$f ':' 'test-.*' >/dev/null; then ./$$f; fi; done

unix: $(executables)
	./unix-server & (sleep 1; ./unix-client 2 2; ./unix-client 10 6; ./unix-client DOWN)

install: $(libraries)
	install libnetwork.a $(prefix)/lib
	install include/*.h $(prefix)/include

TAGS:
	etags $^

$(executables): libnetwork.a

libnetwork.a: $(patsubst %.o,libnetwork.a(%.o),$(library_objects))

include $(dependencies)

.PHONY: all clean realclean test

.SECONDARY: $(objects)

%: tmp/%.o
	$(LINK.o) -o $@ $^ $(LDLIBS)

tmp/%.o: %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

tmp/%.dep: %.cpp
	$(CXX) -M $(CPPFLAGS) $< >$@.$$$$ && \
sed 's,\($*\)\.o[ :]*,tmp/\1.o $@ TAGS: ,' $@.$$$$ >$@; \
rm $@.$$$$

vpath %.cpp src
vpath %.h include
