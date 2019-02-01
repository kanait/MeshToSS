## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright (C) 2000 Gary V. Vaughan
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

MAINTAINERCLEANFILES = Makefile.in

INCLUDES	= -I$(top_srcdir) -I$(top_builddir)
CXXFLAGS	= -Wall -O2

EXTRA_DIST              = optmesh.dsp optmesh.dsw optmesh.vcproj

lib_LIBRARIES		= libmesh.a libsimp.a

libmesh_a_SOURCES	= file.cxx file.h \
				intsec.cxx intsec.h \
				tricube.cxx tricube.h \
				matlib.cxx matlib.h \
				octree.cxx octree.h \
				oppd.cxx oppd.h \
				ppd.cxx ppd.h \
				ppdedge.cxx ppdedge.h \
				ppdface.cxx ppdface.h \
				ppdlist.cxx ppdlist.h \
				ppdnormal.cxx ppdnormal.h \
				ppdpart.cxx ppdpart.h \
				ppdsolid.cxx ppdsolid.h \
				ppdvertex.cxx ppdvertex.h \
				ps.cxx ps.h \
				mytime.cxx mytime.h \
				veclib.cxx veclib.h \
				params.h \
				smd.h

libsimp_a_SOURCES	= qem.cxx qem.h \
				lodprique.cxx lodprique.h \
				star.cxx star.h \
				simpmesh.cxx simpmesh.h \
				subdiv.cxx subdiv.h

bin_PROGRAMS	= meshtoss simpcheck simperror

meshtoss_SOURCES = meshtoss.cxx
meshtoss_LDADD	= libsimp.a libmesh.a

simpcheck_SOURCES = simpcheck.cxx simpcheck.h
simpcheck_LDADD	= libsimp.a libmesh.a

simperror_SOURCES = simperror.cxx
simperror_LDADD	  = libsimp.a libmesh.a



