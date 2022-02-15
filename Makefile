#
# Grosso: Makefile
#

# $Id: Makefile,v 1.1.1.1 2002/06/09 10:29:49 tommi Exp $

###########################################################################

include make.defs

###########################################################################

SUBDIRS	= src lib

GO_THROUGH_SUBDIRS = \
	for i in $(SUBDIRS); \
	do \
		(cd $$i; $(MAKE) $@); \
	done

###########################################################################

release install tags clean dep debug:
	@$(GO_THROUGH_SUBDIRS)
# DO NOT DELETE
