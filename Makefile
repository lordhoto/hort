CXXFLAGS:=-g
CPPFLAGS:=
LDFLAGS:=-g
CXX:=g++
DEPDIR:=.deps

OBJS:=base/main.o
DEPDIRS = $(addsuffix $(DEPDIR),$(sort $(dir $(OBJS))))

-include $(wildcard $(addsuffix /*.d,$(DEPDIRS)))

# common rule for .cpp files
%.o: %.cpp
	mkdir $(*D)/$(DEPDIR)
	$(CXX) -Wp,-MMD,"$(*D)/$(DEPDIR)/$(*F).d",-MQ,"$@",-MP $(CXXFLAGS) $(CPPFLAGS) -c $(<) -o $*.o

hort: $(OBJS)
	$(CXX) -o hort $(LDFLAGS) $(OBJS)

clean:
	rm -f $(OBJS)
	rm -fR $(DEPDIRS)
	rm -f hort
