CXX ?= g++

CXXFLAGS ?= -std=c++17 -O2 -g
#CXXFLAGS ?= -D _DEBUG -ggdb3 -std=c++17 -O2 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion  -Wstrict-overflow=2  -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


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
