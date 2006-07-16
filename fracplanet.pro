TEMPLATE = app

# append debug or release :
CONFIG+= qt opengl stl exceptions release

# Input files

HEADERS += \
           control.h \
           control_about.h \
           control_render.h \
           control_save.h \
           control_terrain.h \
           dialog_documentation.h \
           fracplanet_main.h \
           geometry.h \
           image.h \
           license.h \
           matrix33.h \
           matrix34.h \
           noise.h \
           notifiable.h \
           parameters_cloud.h \
           parameters_noise.h \
           parameters_object.h \
           parameters_render.h \
           parameters_save.h \
           parameters_terrain.h \
           progress.h \
           random.h \
           rgb.h \
           scan.h \
           triangle.h \
           triangle_mesh.h \
           triangle_mesh_cloud.h \
           triangle_mesh_terrain.h \
           triangle_mesh_viewer.h \
           triangle_mesh_viewer_display.h \
           useful.h \
           vertex.h \
           triangle_edge.h \
           xyz.h

SOURCES += \
           control.cpp \
           control_about.cpp \
           control_render.cpp \
           control_save.cpp \
           control_terrain.cpp \
           dialog_documentation.cpp \
           fracplanet.cpp \
           fracplanet_main.cpp \
           geometry.cpp \
           image.cpp \
           license.cpp \
           matrix33.cpp \
           matrix34.cpp \
           noise.cpp \
           notifiable.cpp \
           parameters_cloud.cpp \
           parameters_noise.cpp \
           parameters_object.cpp \
           parameters_render.cpp \
           parameters_save.cpp \
           parameters_terrain.cpp \
           progress.cpp \
           random.cpp \
           rgb.cpp \
           scan.cpp \
           triangle.cpp \
           triangle_mesh.cpp \
           triangle_mesh_cloud.cpp \
           triangle_mesh_terrain.cpp \
           triangle_mesh_viewer.cpp \
           triangle_mesh_viewer_display.cpp \
           useful.cpp \
           vertex.cpp \
           triangle_edge.cpp \
           xyz.cpp

#LIBS += -lboost_program_options-gcc-mt-1_32
LIBS += -lboost_program_options

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
OBJECTS_DIR = obj
