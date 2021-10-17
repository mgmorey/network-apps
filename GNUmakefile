STANDARD := c++17
SYSTEM := $(shell uname -s | cut -d- -f 1)

ifdef NDEBUG
	CPPFLAGS += -D_FORTIFY_SOURCE=2
	CXXFLAGS += -O2
else
ifeq "$(USING_DMALLOC)" "true"
	CPPFLAGS += -DDMALLOC -DMALLOC_FUNC_CHECK
	LDLIBS += -ldmalloc
else ifeq "$(USING_LIBASAN)" "true"
	CXXFLAGS += -fno-omit-frame-pointer -fsanitize=address
	CXXFLAGS += -fsanitize-address-use-after-scope
	LDFLAGS += -fsanitize=address
else
	CXXFLAGS += -fno-omit-frame-pointer
endif
ifneq "$(SYSTEM)" "Darwin"
	CPPFLAGS += -D_GLIBCXX_DEBUG
endif
	CXXFLAGS += -g3 -O0
endif

ifeq "$(SYSTEM)" "Darwin"
	CPPFLAGS += -D_DARWIN_C_SOURCE -DHAVE_SOCKADDR_SA_LEN
else ifeq "$(SYSTEM)" "CYGWIN_NT"
	CPPFLAGS += -D_POSIX_C_SOURCE=200809L
else ifeq "$(SYSTEM)" "FreeBSD"
	CPPFLAGS += -DHAVE_SOCKADDR_SA_LEN
	CPPFLAGS += -I/usr/local/include
	LDLIBS += -L/usr/local/lib
else ifeq "$(SYSTEM)" "MINGW64_NT"
	LDLIBS += -lws2_32
endif

CPPFLAGS += -Iinclude
CXXFLAGS += -std=$(STANDARD) -Wall -Werror -Wextra -Wpedantic -Wshadow

ifeq "$(SYSTEM)" "Darwin"
	LDFLAGS += -Wl,-map,$@.map
else
ifneq "$(SYSTEM)" "FreeBSD"
	CXXFLAGS += -Wa,-adghln=$(subst .o,.lst,$@)
endif
	LDFLAGS += -Wl,-Map=$@.map
endif

LINK.o = $(CXX) $(LDFLAGS)

prefix = /usr/local

sources = $(executable_sources) $(library_sources)

executable_sources = test-address.cpp test-bind.cpp \
test-buffer.cpp test-connect.cpp test-hostname.cpp
library_sources = network-address.cpp network-addrinfo.cpp \
network-bind.cpp network-buffer.cpp network-close.cpp \
network-connect.cpp network-context.cpp network-endpoint.cpp \
network-error.cpp network-family.cpp network-flags.cpp \
network-format.cpp network-hints.cpp network-host.cpp \
network-hostname.cpp network-name.cpp network-open.cpp \
network-peername.cpp network-protocol.cpp network-result.cpp \
network-sockaddr.cpp network-socket.cpp network-sockets.cpp \
network-sockname.cpp network-socktype.cpp network-string.cpp \
stream-address.cpp stream-addrinfo.cpp stream-hints.cpp \
stream-socket.cpp stream-string.cpp

ifneq "$(SYSTEM)" "MINGW64_NT"
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
	cppcheck --cppcheck-build-dir=tmp --enable=all --inline-suppr --quiet --std=$(STANDARD) $(CPPFLAGS) .

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
	install include/network-*.h $(prefix)/include

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
