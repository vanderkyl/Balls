ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BallSim.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= BallSim.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS)

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
