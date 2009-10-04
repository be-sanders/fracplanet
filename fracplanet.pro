TARGET = fracplanet
TEMPLATE = app

CONFIG+= qt stl precompile_header exceptions release  # debug/release
QT += opengl

PRECOMPILED_HEADER = precompiled.h

HEADERS += $$system(ls *.h)
SOURCES += $$system(ls *.cpp)
LIBS += -lboost_program_options

DEFINES += QT_DLL

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

# -DBOOST_SP_USE_PTHREADS is a workround for debian bug 485434 (maybe only needed on sparc?)
QMAKE_CXXFLAGS_RELEASE += -DQT_NO_ASCII_CAST -pthread -DBOOST_SP_USE_PTHREADS
QMAKE_CXXFLAGS_DEBUG += -DQT_NO_ASCII_CAST -pthread -DBOOST_SP_USE_PTHREADS

######################################
# Hide those crufty moc_ files away

MOC_DIR = moc
OBJECTS_DIR = obj
