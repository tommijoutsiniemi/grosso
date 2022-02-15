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
	-$(INSTALL) --owner root --group root --mode 644 \
		$(LEVELS) $(DESTDIR)$(LEVELDIR)
	-$(MKDIRHIER) $(DESTDIR)$(SCOREDIR)
	-$(INSTALL) --owner root --group games --mode 660 \
		grosso.scores $(DESTDIR)$(SCOREDIR)
	-$(MKDIRHIER) $(DESTDIR)$(MANDIR)/man6
	-$(INSTALL) --mode 644 grosso.6 $(DESTDIR)$(MANDIR)/man6
	-$(COMPRESS) $(DESTDIR)$(MANDIR)/man6/grosso.6
