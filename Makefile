.PHONY: all
.FORCE:

CXX=g++-10
SDIR=src
ODIR=obj

SRCS=$(wildcard $(SDIR)/wh-sdk/*.cpp)
SRCS+=$(wildcard $(SDIR)/wh-sdk/utils/*.cpp)
SRCS+=$(wildcard $(SDIR)/core/*.cpp)
SRCS+=$(wildcard $(SDIR)/core/impl/*.cpp)
SRCS+=$(wildcard $(SDIR)/core/interfaces/*.cpp)

OBJS=$(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRCS))

# Header dependency target files; generated by g++ with -MMD
DEPS=$(OBJS:%.o=%.d)

WARNINGS=-Wall -Wno-parentheses -Wno-unknown-pragmas -Wno-delete-non-virtual-dtor
CXXFLAGS=-std=c++17 -m32 $(WARNINGS) -I$(SDIR) -fPIC -D_GNU_SOURCE -I$(SDIR)/deps/subhook
LDFLAGS=-m32 -shared -lstdc++fs -L$(SDIR)/deps/subhook -lsubhook

# Import config.mk, which can be used for optional config
-include config.mk

all: wormhole.so

-include $(DEPS)

wormhole.so: $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@
