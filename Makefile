CXX ?= g++

CXXFLAGS ?= -std=c++17 -O2 -g
BUILDDIR ?= ./build/
SRCDIR ?= ./source/

COMMONINC = -I./include

CXXFLAGS += $(COMMONINC)

CCSRCS = $(SRCDIR)triangles.cpp

SUBS := $(CCSRCS)
SUBS := $(subst $(SRCDIR), $(BUILDDIR), $(SUBS))

OBJS = $(SUBS:.cpp=.o)

DEPS = $(SUBS:.cpp=.d)

.PHONY: all
all: $(OBJS)

$(BUILDDIR)%.o: $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

include $(DEPS)

$(BUILDDIR)%.d: $(SRCDIR)%.cpp
	@echo "Collecting deps for $<"
	@mkdir -p $(dir $@)
	@$(CXX) -E $(CXXFLAGS) $< -MM -MT $(@:.d=.o) > $@

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
