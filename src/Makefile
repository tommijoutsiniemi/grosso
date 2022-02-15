#
# Grosso: src/Makefile
#

# $Id: Makefile,v 1.1.1.1 2002/06/09 10:29:49 tommi Exp $

###########################################################################

include ../make.defs

###########################################################################

OBJS    = Field.o Fred.o Game.o Grosso.o ScoreBoard.o Worm.o main.o

SRCS    = $(OBJS:.o=.cc)

###########################################################################

release debug all: $(BINARY)

$(BINARY): $(OBJS)
	$(CXX) -o $@ $(DEBUG) $(LDFLAGS) $(OBJS) $(LIBS)

install: all
	-$(MKDIRHIER) $(DESTDIR)$(BINDIR)
	-$(INSTALL) --owner root --group games --mode 2755 --strip \
		$(BINARY) $(DESTDIR)$(BINDIR)

tags:
	$(ETAGS) $(ETAGSFLAGS) *.cc *.hh

clean:
	$(RM) $(BINARY) $(OBJS) TAGS Makefile.bak

dep:
	$(MAKEDEPEND) $(DEPFLAGS) $(SRCS)

# DO NOT DELETE

Field.o: config.hh Field.hh
Fred.o: config.hh Fred.hh Worm.hh Field.hh
Game.o: config.hh Game.hh Fred.hh Worm.hh Field.hh Grosso.hh
Grosso.o: config.hh Grosso.hh Worm.hh Field.hh
ScoreBoard.o: config.hh ScoreBoard.hh
Worm.o: config.hh Field.hh Worm.hh
main.o: config.hh ScoreBoard.hh Game.hh Fred.hh Worm.hh Field.hh Grosso.hh
main.o: version.hh
