CXXFLAGS:=-g -Wall
CPPFLAGS:=-I.
LDFLAGS:=-g -lncurses
CXX:=g++
DEPDIR:=.deps

OBJS := base/main.o \
		base/rnd.o \
		base/geo.o \
		base/token.o \
		base/parser.o \
		gui/intern/screen.o \
		gui/intern/window.o \
		gui/intern/input.o \
		gui/screen.o \
		game/map.o \
		game/state.o \
		game/game.o \
		game/level.o \
		game/event.o \
		game/monster.o \
		game/defs.o \
		ai/fsm.o \
		ai/monster.o

DEPDIRS = $(addsuffix $(DEPDIR),$(sort $(dir $(OBJS))))

hort: $(OBJS)
	$(CXX) -o hort $(OBJS) $(LDFLAGS)

-include $(wildcard $(addsuffix /*.d,$(DEPDIRS)))

# common rule for .cpp files
%.o: %.cpp
	mkdir -p $(*D)/$(DEPDIR)
	$(CXX) -Wp,-MMD,"$(*D)/$(DEPDIR)/$(*F).d",-MQ,"$@",-MP $(CXXFLAGS) $(CPPFLAGS) -c $(<) -o $*.o

clean:
	rm -f $(OBJS)
	rm -fR $(DEPDIRS)
	rm -f hort
