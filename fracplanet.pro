TEMPLATE = app

# append debug or release
CONFIG+= qt opengl debug # release

#######################################
# Installation 

INSTALLS += executable documentation
executable.path = /usr/local/bin
executable.files = fracplanet 
documentation.path = /usr/local/share/doc/fracplanet
documentation.files = fracplanet.htm fracplanet.css

##################
# Improved optimisation options from qmake defaults.
# Now leaving these ON for general distribution
# -funroll-loops unproven 

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -fomit-frame-pointer -ffast-math 

##################
# Architecture specific optimisations
# The following architecture options make the compiled executables less portable.
# (On a different setup you may need to change what's subtracted out of the flags.)
# NB -march=X implies -mcpu=X... no need to specify both the same

contains(CPU, p4 ){
  QMAKE_CXXFLAGS_RELEASE -= -march=i386 -mcpu=i686
  QMAKE_CXXFLAGS_RELEASE += -march=pentium4 -mfpmath=sse -msse2
}

contains(CPU, p3 ){
  QMAKE_CXXFLAGS_RELEASE -= -march=i386 -mcpu=i686
  QMAKE_CXXFLAGS_RELEASE += -march=pentium3 -mfpmath=sse -msse
}

contains(CPU, xp ){
  QMAKE_CXXFLAGS_RELEASE -= -march=i386 -mcpu=i686
  QMAKE_CXXFLAGS_RELEASE += -march=athlon-xp -mfpmath=sse -msse
}

###################################
# Input files

HEADERS += \
           control_about.h \
           control_render.h \
           control_save.h \
           control_terrain.h \
           fracplanet_main.h \
           geometry.h \
           license.h \
           noise.h \
           parameters_render.h \
           parameters_save.h \
           parameters_terrain.h \
           pov_mode.h \
           progress.h \
           random.h \
           rgb.h \
           triangle.h \
           triangle_mesh.h \
           triangle_mesh_terrain.h \
           triangle_mesh_viewer.h \
           triangle_mesh_viewer_display.h \
           useful.h \
           vertex.h \
           triangle_edge.h \
           xyz.h

SOURCES += \
           control_about.cpp \
           control_render.cpp \
           control_save.cpp \
           control_terrain.cpp \
           fracplanet.cpp \
           fracplanet_main.cpp \
           geometry.cpp \
           license.cpp \
           noise.cpp \
           parameters_render.cpp \
           parameters_save.cpp \
           parameters_terrain.cpp \
           pov_mode.cpp \
           progress.cpp \
           random.cpp \
           rgb.cpp \
           triangle.cpp \
           triangle_mesh.cpp \
           triangle_mesh_terrain.cpp \
           triangle_mesh_viewer.cpp \
           triangle_mesh_viewer_display.cpp \
           useful.cpp \
           vertex.cpp \
           triangle_edge.cpp \
           xyz.cpp

#######################################
# Version numbering.  This is ENTIRELY controlled by what is echoed by the VERSION script

QMAKE_CXXFLAGS_RELEASE += '-DFRACPLANET_VERSION="$$VERSION_NUMBER"'
QMAKE_CXXFLAGS_DEBUG   += '-DFRACPLANET_VERSION="$$VERSION_NUMBER"'
QMAKE_CXXFLAGS_RELEASE += '-DFRACPLANET_BUILD="$$VERSION_NUMBER (release build)"'
QMAKE_CXXFLAGS_DEBUG   += '-DFRACPLANET_BUILD="$$VERSION_NUMBER (debug build)"'

# qmake's library code can use this too (but only for shared libraries which we don't use)
VERSION=$$VERSION_NUMBER

#######################################
# Disable assertions in release version

QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
QMAKE_CFLAGS_RELEASE += -DNDEBUG

######################################
# Other stuff:
# Disable implicit cast from QString to char*

QMAKE_CXXFLAGS_RELEASE += -DQT_NO_ASCII_CAST
QMAKE_CXXFLAGS_DEBUG += -DQT_NO_ASCII_CAST

######################################
# Hide those crufty moc_ files away

MOC_DIR = moc

