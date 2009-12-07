CXXFLAGS:=-g -Wall
CPPFLAGS:=-I./
LDFLAGS:=-g -lncurses
CXX:=g++
DEPDIR:=.deps

OBJS := base/main.o \
		base/rnd.o \
		gui/screen.o \
		gui/window.o \
		gui/input.o

DEPDIRS = $(addsuffix $(DEPDIR),$(sort $(dir $(OBJS))))

hort: $(OBJS)
	$(CXX) -o hort $(LDFLAGS) $(OBJS)

-include $(wildcard $(addsuffix /*.d,$(DEPDIRS)))

# common rule for .cpp files
%.o: %.cpp
	mkdir -p $(*D)/$(DEPDIR)
	$(CXX) -Wp,-MMD,"$(*D)/$(DEPDIR)/$(*F).d",-MQ,"$@",-MP $(CXXFLAGS) $(CPPFLAGS) -c $(<) -o $*.o

clean:
	rm -f $(OBJS)
	rm -fR $(DEPDIRS)
	rm -f hort
