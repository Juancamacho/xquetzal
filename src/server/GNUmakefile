#
# $Id$
#
# MAKEFILE GNU MAKE for Linux Version 1.0
# Copyright (C) 2002 by Free Software Foundation, Inc.
# Author: Victor H De la Luz <itztli@gmail.com>
#
#                         COPYING
#
# This  program  is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License  as
# published by the Free Software Foundation; either version 2, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty  of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE.  See  the
# GNU General Public License for more details.
#
# You  should  have  received  a  copy  of the GNU General Public
# License along with this program; if  not,  write  to  the  Free
# Software Foundation, Inc., 675  Mass  Ave, Cambridge, MA 02139,
# USA.
#
# ###################### Revision History #######################
#
# $Log$
# ###############################################################

#================================================================
# GNU C COMPILER FOR LINUX
#================================================================
OBJS := $(patsubst %.c, %.o, $(CFILES))
RM := rm -f


#----------------------------------------------------------------

# Make rules

.PHONY: all clean distclean

compile: 
	$(MAKE) -C xitris-server
	$(MAKE) -C xitris-simulator
	@echo "Now type: make intall."

all:	clean compile install
	@echo -en "\n"

clean:
	$(RM) *~ core $(OBJS) "#*"
	$(MAKE) -C xitris-server clean

distclean: clean
	$(RM) $(PROGRAM)

install:
	$(MAKE) -C xitris-server install
	$(MAKE) -C xitris-simulator install

# End Make