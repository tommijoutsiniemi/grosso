#
# Grosso: lib/Makefile
#

# $Id: Makefile,v 1.1.1.1 2002/06/09 10:29:49 tommi Exp $

###########################################################################

include ../make.defs

###########################################################################

LEVELS	= 01 02 03 04 05

release debug clean dep tags:

install:
	-$(MKDIRHIER) $(DESTDIR)$(LEVELDIR)
	-$(INSTALL) --owner $(L_OWNER) --group $(L_GROUP) --mode $(L_MODE) \
		$(LEVELS) $(DESTDIR)$(LEVELDIR)
	-$(MKDIRHIER) $(DESTDIR)$(SCOREDIR)
	-$(INSTALL) --owner $(S_OWNER) --group $(S_GROUP) --mode $(S_MODE) \
		grosso.scores $(DESTDIR)$(SCOREDIR)
	-$(MKDIRHIER) $(DESTDIR)$(MANDIR)/man6
	-$(INSTALL) --mode $(M_MODE) grosso.6 $(DESTDIR)$(MANDIR)/man6
	-$(COMPRESS) $(DESTDIR)$(MANDIR)/man6/grosso.6
