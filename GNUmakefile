GCC_4_8_5_RH := g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-44)
SYSTEM_PREFIX := $(shell uname -s | cut -d- -f 1)

ifdef NDEBUG
	CPPFLAGS += -D_FORTIFY_SOURCE=2
	CXXFLAGS += -O2
else
ifeq "$(USING_DMALLOC)" "true"
	CPPFLAGS += -DDMALLOC -DMALLOC_FUNC_CHECK
	LDLIBS += -ldmalloc
else ifeq "$(USING_LIBASAN)" "true"
	CXXFLAGS += -fno-omit-frame-pointer -fsanitize=address
ifneq "$(shell $(CXX) --version | head -n 1)" "$(GCC_4_8_5_RH)"
	CXXFLAGS += -fsanitize-address-use-after-scope
endif
	LDFLAGS += -fsanitize=address
else
	CXXFLAGS += -fno-omit-frame-pointer
endif
ifneq "$(SYSTEM_PREFIX)" "Darwin"
	CPPFLAGS += -D_GLIBCXX_DEBUG
endif
	CXXFLAGS += -g3 -O0
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
CXXFLAGS += -std=c++98 -Wall -Werror -Wextra -Wpedantic -Wshadow

ifeq "$(SYSTEM_PREFIX)" "Darwin"
	LDFLAGS += -Wl,-map,$@.map
else
ifneq "$(SYSTEM_PREFIX)" "FreeBSD"
	CXXFLAGS += -Wa,-adghln=$(subst .o,.lst,$@)
endif
	LDFLAGS += -Wl,-Map=$@.map
endif

LINK.o = $(CXX) $(LDFLAGS)

prefix = /usr/local

sources = $(executable_sources) $(library_sources)

executable_sources = test-address.cpp test-buffer.cpp \
test-hostname.cpp test-stream.cpp
library_sources = network-address.cpp network-addrinfo.cpp \
network-buffer.cpp network-close.cpp network-connect.cpp \
network-context.cpp network-endpoint.cpp network-family.cpp \
network-flags.cpp network-format.cpp network-host.cpp \
network-hosts.cpp network-hostname.cpp network-nullable.cpp \
network-peername.cpp network-protocol.cpp network-result.cpp \
network-socket.cpp network-sockets.cpp network-socktype.cpp \
network-unique.cpp stream-address.cpp stream-addrinfo.cpp

ifndef HTTP_PROXY
	executable_sources += test-connect.cpp
endif

ifneq "$(SYSTEM_PREFIX)" "MINGW64_NT"
	executable_sources += test-socket.cpp unix-client.cpp unix-server.cpp
endif

executables = $(subst .cpp,,$(executable_sources))
libraries = libnetwork.a

objects = $(library_objects) $(executable_objects)

executable_objects = $(addprefix $(tmp_dir)/,$(subst .cpp,.o,$(executable_sources)))
library_objects = $(addprefix $(tmp_dir)/,$(subst .cpp,.o,$(library_sources)))

dependencies = $(addprefix $(tmp_dir)/,$(subst .cpp,.dep,$(sources)))
listings = $(addprefix $(tmp_dir)/,$(subst .cpp,.lst,$(sources)))
maps = $(subst .cpp,.map,$(executable_sources))

tmp_dir = tmp

.PHONY:	all
all: $(executables) TAGS

.PHONY:	check
check:
	cppcheck --cppcheck-build-dir=tmp --enable=all --quiet --std=c++03 $(CPPFLAGS) .

.PHONY:	clean
clean:
	rm -f $(executables) $(libraries) $(objects) $(maps) $(listings)

.PHONY:	realclean
realclean: clean
	rm -f $(dependencies) TAGS

.PHONY:	test
test: $(executables)
	for f in $^; do if expr $$f ':' 'test-.*' >/dev/null; then ./$$f; fi; done

.PHONY:	unix
unix: $(executables)
	./unix-server & (sleep 1; ./unix-client 2 2; ./unix-client 10 6; ./unix-client DOWN)

.PHONY:	install
install: $(libraries)
	install libnetwork.a $(prefix)/lib
	install include/network-*.h include/stream-*.h $(prefix)/include

TAGS:
	etags $^

$(executables): libnetwork.a

libnetwork.a: $(patsubst %.o,libnetwork.a(%.o),$(library_objects))

$(dependencies) $(objects): | $(tmp_dir)

$(tmp_dir):
	mkdir -p $(tmp_dir)

include $(dependencies)

.SECONDARY: $(objects)

%: $(tmp_dir)/%.o
	$(LINK.o) -o $@ $^ $(LDLIBS)

$(tmp_dir)/%.o: %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(tmp_dir)/%.dep: %.cpp
	$(CXX) $(CPPFLAGS) -MM $< | ./make-makefile -f TAGS -o $@

vpath %.cpp src
vpath %.h include
