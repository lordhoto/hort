CXXFLAGS:=-g -Wall
CPPFLAGS:=-I.
LDFLAGS:=-g -lncurses
CXX:=g++
DEPDIR:=.deps

OBJS := \
		ai/monster.o \
		ai/fsm.o \
		base/geo.o \
		base/main.o \
		base/parser.o \
		base/rnd.o \
		game/defs.o \
		game/event.o \
		game/game.o \
		game/level.o \
		game/levelloader.o \
		game/map.o \
		game/maploader.o \
		game/monster.o \
		game/monsterdatabase.o \
		game/monsterdefinitionloader.o \
		game/state.o \
		game/tiledatabase.o \
		game/tiledefinitionloader.o \
		gui/screen.o \
		gui/intern/drawdesc.o \
		gui/intern/input.o \
		gui/intern/screen.o \
		gui/intern/window.o

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
