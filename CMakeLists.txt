
project( flashplayer_kpart )

# KDE 4
find_package(KDE4 REQUIRED)
include( KDE4Defaults )
include( MacroLibrary )
include_directories(${KDE4_INCLUDES})

include( UsePkgConfig )

# GLib library setup
   PKGCONFIG(glib-2.0 GLibIncludeDir GLibLibDir GLibLinkFlags GLibCFlags)
 
# Cairo graphics library setup
   PKGCONFIG(cairo CairoIncludeDir CairoLibDir CairoLinkFlags CairoCFlags)

# swfdec flash player library setup
   PKGCONFIG(swfdec-0.5 SWFDecIncludeDir SWFDecLibDir SWFDecLinkFlags SWFDecCFlags)

# Flash Player KPart

set( flashplayer_PART_SRCS 
     flashplayerwidget.cpp
     flashplayer_part.cpp )

kde4_add_plugin(flashplayerpart WITH_PREFIX ${flashplayer_PART_SRCS} )

# add link flags for Glib, Cairo, Swfdec
set_target_properties( flashplayerpart PROPERTIES LINK_FLAGS "${GLibLinkFlags} ${CairoLinkFlags} ${SWFDecLinkFlags}" )
# add compile flags for Glib, Cairo, Swfdec
set_source_files_properties( ${flashplayer_PART_SRCS} PROPERTIES 
                             COMPILE_FLAGS "${GLibCFlags} ${CairoCFlags} ${SWFDecCFlags}" )


target_link_libraries( flashplayerpart ${KDE4_KPARTS_LIBS} swfdec-0.5 cairo ) 
install(TARGETS flashplayerpart DESTINATION ${PLUGIN_INSTALL_DIR} )

# install .desktop file to associate part with Flash mimetype
install( FILES flashplayerpart.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
