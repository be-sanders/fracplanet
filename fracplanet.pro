TEMPLATE = app

# append debug or release
CONFIG+= qt opengl release

# Input
HEADERS += \
           control_about.h \
           control_render.h \
           control_save.h \
           control_terrain.h \
           fracplanet_main.h \
           geometry.h \
           license.h \
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
# Disable assertions in release version
#
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
QMAKE_CFLAGS_RELEASE += -DNDEBUG

#######################################
# Install targets
#
INSTALLS += executable
executable.path = /usr/local/bin
executable.files = fracplanet 

#######################################
# Make a tgz
#
tgz.target = fracplanet.tgz
tgz.depends = README BUILD LICENSE TODO fracplanet.pro doxygen.cfg $$HEADERS $$SOURCES
tgz.commands = tar cvfz fracplanet.tgz README BUILD LICENSE TODO fracplanet.pro doxygen.cfg $$HEADERS $$SOURCES 
QMAKE_EXTRA_UNIX_TARGETS += tgz


#####################################
# Stuff to drive doxygen
#
doc.target = doc
doc.commands = mkdir -p doc ; doxygen doxygen.cfg
doc.depends = $$HEADERS $$SOURCES
QMAKE_EXTRA_UNIX_TARGETS += doc

#Add this to build documentation every time
#TARGETDEPS += $$doc.target

#Unfortunately it doesn't generate rm -r -f doc, so this doesn't work
#QMAKE_CLEAN += doc

# Separate target for cleaning up documentation
QMAKE_EXTRA_UNIX_TARGETS += realclean
realclean.depends = clean
realclean.commands = rm -r -f doc

######################################
# Hide those crufty moc_ files away
#
MOC_DIR = moc

