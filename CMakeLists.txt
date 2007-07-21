
project( flashplayer_kpart )

# KDE 3
find_package(Qt3 REQUIRED)
find_package(KDE3 REQUIRED)
add_definitions( ${QT_DEFINITIONS} ${KDE3_DEFINITIONS} )

include_directories(${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR})
include_directories(${CMAKE_BINARY_DIR})

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

kde3_automoc(${flashplayer_PART_SRCS})
kde3_add_kpart(flashplayerpart WITH_PREFIX ${flashplayer_PART_SRCS} )

# add link flags for Glib, Cairo, Swfdec
set_target_properties( flashplayerpart PROPERTIES LINK_FLAGS "${GLibLinkFlags} ${CairoLinkFlags} ${SWFDecLinkFlags}" )
# add compile flags for Glib, Cairo, Swfdec
set_source_files_properties( ${flashplayer_PART_SRCS} PROPERTIES 
                             COMPILE_FLAGS "${GLibCFlags} ${CairoCFlags} ${SWFDecCFlags}" )


target_link_libraries( flashplayerpart ${KDE3_KPARTS_LIBS} swfdec-0.5 cairo ) 
install(TARGETS flashplayerpart DESTINATION ${KDE3PREFIX}/lib/kde3 )

# install .desktop file to associate part with Flash mimetype
install( FILES flashplayerpart.desktop DESTINATION ${KDE3PREFIX}/share/services )
