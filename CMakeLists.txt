cmake_minimum_required( VERSION 2.6 )

if (DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel")
else()
	message(STATUS "No build type selected, default to Debug")
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel")
endif()

project( VectorStorm_Testbeds )

add_subdirectory( vectorstorm )
find_package(OpenGL)
set(VS_LIBRARIES
	${OPENGL_LIBRARIES}
	${SDL_LIBRARY}
	${SDL_IMAGE_LIBRARY}
	${SDL_MIXER_LIBRARY}
	${SDL_TTF_LIBRARY}
	${PNG_LIBRARY}
	${GLEW_LIBRARY}
	${BOX2D_LIBRARY}
	)

# find . -type d
include_directories(
	Games
	Games/Asteroids
	Games/CameraTest
	Games/FontMaker
	Games/LagTest
	Games/LerpTest
	Games/LerpTest/Lerp
	Games/Preferences
	Games/SpaceMazeExpress
	Games/StarShot
	Games/Testbed
	Games/VectorPhysics
	)

set(ASTEROIDS_SOURCES
	Games/Asteroids/AST_Asteroid.cpp
	Games/Asteroids/AST_Asteroid.h
	Games/Asteroids/AST_Camera.cpp
	Games/Asteroids/AST_Camera.h
	Games/Asteroids/AST_Game.cpp
	Games/Asteroids/AST_Game.h
	Games/Asteroids/AST_HowToPlay.cpp
	Games/Asteroids/AST_HowToPlay.h
	Games/Asteroids/AST_Hud.cpp
	Games/Asteroids/AST_Hud.h
	Games/Asteroids/AST_ModeGameOver.cpp
	Games/Asteroids/AST_ModeGameOver.h
	Games/Asteroids/AST_ModeHowTo.cpp
	Games/Asteroids/AST_ModeHowTo.h
	Games/Asteroids/AST_ModeInGame.cpp
	Games/Asteroids/AST_ModeInGame.h
	Games/Asteroids/AST_ModeTitleScreen.cpp
	Games/Asteroids/AST_ModeTitleScreen.h
	Games/Asteroids/AST_PlayerShip.cpp
	Games/Asteroids/AST_PlayerShip.h
	Games/Asteroids/AST_PlayerShot.cpp
	Games/Asteroids/AST_PlayerShot.h
	Games/Asteroids/AST_Star.cpp
	Games/Asteroids/AST_Star.h
	)
set(CAMERATEST_SOURCES
	Games/CameraTest/CT_Camera.cpp
	Games/CameraTest/CT_Camera.h
	Games/CameraTest/CT_CameraScript.cpp
	Games/CameraTest/CT_CameraScript.h
	Games/CameraTest/CT_CameraScript3D.cpp
	Games/CameraTest/CT_CameraScript3D.h
	Games/CameraTest/CT_Game.cpp
	Games/CameraTest/CT_Game.h
	Games/CameraTest/CT_Mode2D.cpp
	Games/CameraTest/CT_Mode2D.h
	Games/CameraTest/CT_Mode3D.cpp
	Games/CameraTest/CT_Mode3D.h
	Games/CameraTest/CT_Player.cpp
	Games/CameraTest/CT_Player.h
	Games/CameraTest/CT_SafeZone.cpp
	Games/CameraTest/CT_SafeZone.h
	Games/CameraTest/CT_Stage.cpp
	Games/CameraTest/CT_Stage.h
	)
set(FONTMAKER_SOURCES
	Games/FontMaker/FM_FontMaker.cpp
	Games/FontMaker/FM_FontMaker.h
	)
set(MAINMENU_SOURCES
	Games/GM_MainMenu.cpp
	Games/GM_MainMenu.h
	)
set(LAGTEST_SOURCES
	Games/LagTest/LAG_Game.cpp
	Games/LagTest/LAG_Game.h
	)
set(LERPTEST_SOURCES
	Games/LerpTest/LT_Camera.cpp
	Games/LerpTest/LT_Camera.h
	Games/LerpTest/LT_Game.cpp
	Games/LerpTest/LT_Game.h
	Games/LerpTest/LT_Graph.cpp
	Games/LerpTest/LT_Graph.h
	Games/LerpTest/LT_ResponseCurve.cpp
	Games/LerpTest/LT_ResponseCurve.h
	Games/LerpTest/LT_Scenes.h
	)
set(LERPTEST_LERP_SOURCES
	Games/LerpTest/Lerp/LT_CameraMove.cpp
	Games/LerpTest/Lerp/LT_CameraMove.h
	Games/LerpTest/Lerp/LT_Lerpable.cpp
	Games/LerpTest/Lerp/LT_Lerpable.h
	Games/LerpTest/Lerp/LT_PlayerControl.cpp
	Games/LerpTest/Lerp/LT_PlayerControl.h
	Games/LerpTest/Lerp/LT_WindowFade.cpp
	Games/LerpTest/Lerp/LT_WindowFade.h
	Games/LerpTest/Lerp/LT_WindowSlide.cpp
	Games/LerpTest/Lerp/LT_WindowSlide.h
	)
set(PREFERENCES_SOURCES
	Games/Preferences/PREF_Game.cpp
	Games/Preferences/PREF_Game.h
	)
set(SPACEMAZEEXPRESS_SOURCES
	Games/SpaceMazeExpress/SMX_Game.cpp
	Games/SpaceMazeExpress/SMX_Game.h
	Games/SpaceMazeExpress/SMX_Maze.cpp
	Games/SpaceMazeExpress/SMX_Maze.h
	Games/SpaceMazeExpress/SMX_ModeInGame.cpp
	Games/SpaceMazeExpress/SMX_ModeInGame.h
	Games/SpaceMazeExpress/SMX_PlayerShip.cpp
	Games/SpaceMazeExpress/SMX_PlayerShip.h
	Games/SpaceMazeExpress/SMX_Rope.cpp
	Games/SpaceMazeExpress/SMX_Rope.h
	)
set(STARSHOT_SOURCES
	Games/StarShot/SST_Fireworks.cpp
	Games/StarShot/SST_Fireworks.h
	Games/StarShot/SST_Game.cpp
	Games/StarShot/SST_Game.h
	Games/StarShot/SST_Hud.cpp
	Games/StarShot/SST_Hud.h
	Games/StarShot/SST_ModeInGame.cpp
	Games/StarShot/SST_ModeInGame.h
	Games/StarShot/SST_Rocket.cpp
	Games/StarShot/SST_Rocket.h
	Games/StarShot/SST_Sequence.cpp
	Games/StarShot/SST_Sequence.h
	)
set(TESTBED_SOURCES
	Games/Testbed/TB_Game.cpp
	Games/Testbed/TB_Game.h
	)
set(VECTORPHYSICS_SOURCES
	Games/VectorPhysics/VP_Drawable.cpp
	Games/VectorPhysics/VP_Drawable.h
	Games/VectorPhysics/VP_Game.cpp
	Games/VectorPhysics/VP_Game.h
	)


set(PLATFORM_SOURCES)
SET(PLATFORM_LIBRARIES)

if ( APPLE )
	SET( PLATFORM_SOURCES osx/SDLMain.m )
	SET(PLATFORM_LIBRARIES ${COCOA_LIBRARY})
endif ( APPLE )

include_directories( SYSTEM
	${OPENGL_INCLUDE_DIR}
	${SDL_INCLUDE_DIR}
	${SDL_MIXER_INCLUDE_DIR}
	${SDL_IMAGE_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${BOX2D_INCLUDE_DIR}
	)
include_directories( .
	vectorstorm
	vectorstorm/VS
	)

add_executable(
	VectorStorm_Testbeds
	MACOSX_BUNDLE
	main.cpp
	${ASTEROIDS_SOURCES}
	${CAMERATEST_SOURCES}
	${FONTMAKER_SOURCES}
	${MAINMENU_SOURCES}
	${LAGTEST_SOURCES}
	${LERPTEST_SOURCES}
	${LERPTEST_LERP_SOURCES}
	${PREFERENCES_SOURCES}
	${SPACEMAZEEXPRESS_SOURCES}
	${STARSHOT_SOURCES}
	${TESTBED_SOURCES}
	${VECTORPHYSICS_SOURCES}
	${PLATFORM_SOURCES}
	)

if (UNIX)
	set(CMAKE_CXX_FLAGS_DEBUG "-include VS_VectorStorm_Debug.h ${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-include VS_VectorStorm_Debug.h ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
	set(CMAKE_CXX_FLAGS_RELEASE "-include VS_VectorStorm.h ${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "-include VS_VectorStorm.h ${CMAKE_CXX_FLAGS_MINSIZEREL}")
else (UNIX)
	add_definitions("/FI\"${PREFIX}\"")
endif(UNIX)

source_group("Games\\Asteroids" FILES ${ASTEROIDS_SOURCES})
source_group("Games\\Cameratest" FILES ${CAMERATEST_SOURCES})
source_group("Games\\Fontmaker" FILES ${FONTMAKER_SOURCES})
source_group("Games\\Mainmenu" FILES ${MAINMENU_SOURCES})
source_group("Games\\Lagtest" FILES ${LAGTEST_SOURCES})
source_group("Games\\Lerptest" FILES ${LERPTEST_SOURCES})
source_group("Games\\Lerptest\\Lerp" FILES ${LERPTEST_LERP_SOURCES})
source_group("Games\\Preferences" FILES ${PREFERENCES_SOURCES})
source_group("Games\\SpaceMazeExpress" FILES ${SPACEMAZEEXPRESS_SOURCES})
source_group("Games\\StarShot" FILES ${STARSHOT_SOURCES})
source_group("Games\\Testbed" FILES ${TESTBED_SOURCES})
source_group("Games\\VectorPhysics" FILES ${VECTORPHYSICS_SOURCES})
source_group("Platform" FILES ${PLATFORM_SOURCES})

IF(APPLE)
	add_definitions("-D__apple_cc__")
	SET(CMAKE_INSTALL_NAME_DIR @executable_path)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

	set_target_properties(VectorStorm_Testbeds
		PROPERTIES
		#INSTALL_RPATH "@executable_path/../Frameworks"
		INSTALL_RPATH_USER_LINK_PATH TRUE
		#LINK_FLAGS -Wl
		#LINK_FLAGS -Wl,-rpath,@executable_path/../Frameworks/
		XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
		XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "YES"
        XCODE_ATTRIBUTE_FRAMEWORK_SEARCH_PATHS "/Library/Frameworks/"
		#XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH_DEBUG "YES"
		#XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH_RELEASE "NO"
		)

ENDIF(APPLE)
if ( UNIX )
	add_definitions("-D UNIX")
endif ( UNIX )


target_link_libraries( VectorStorm_Testbeds
	vectorstorm
	${PLATFORM_LIBRARIES}
	${VS_LIBRARIES}
	)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS VectorStorm_Testbeds
	BUNDLE DESTINATION bin
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)
if(APPLE)
	set_target_properties(VectorStorm_Testbeds PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/osx/Info.plist")
	add_custom_command(TARGET VectorStorm_Testbeds
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CFG_INTDIR}/VectorStorm_Testbeds.app/Contents/Resources
		COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_CFG_INTDIR}/VectorStorm_Testbeds.app/Contents/Resources/
		COMMENT "Copying Data files into place"
		)

	# Do a proper app bundle on 'install', taking libraries from /usr/local/lib.
	# (Note that this assumes that all our libraries will be found inside that
	# directory tree.  Should probably extract the directories from our various
	# library variables (SDL_LIBRARY, SDL_MIXER_LIBRARY, etc) and include those
	# directories in the DIRS list as well, just to ensure that everything gets
	# covered, even if people have installed those libraries in other odd places
	# on OSX.  (Folks who have installed SDL etc. through MacPorts probably have
	# these libraries located in /opt/local/lib or somewhere similar, for example)
	set(BUNDLE "${CMAKE_CURRENT_BINARY_DIR}/VectorStorm_Testbeds.app")
	set(DIRS "/usr/local/lib")
	install(CODE "
		set(BU_CHMOD_BUNDLE_ITEMS ON)
		include(BundleUtilities)
		fixup_bundle(\"${BUNDLE}\" \"\" \"${DIRS}\")
		" COMPONENT Runtime)

endif(APPLE)


