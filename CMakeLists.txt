cmake_minimum_required(VERSION 3.2.3)

IF(ARCH_64BIT)
	SET(PROJ_NAME gw2dps64)
	SET(BOOST_LIBRARYDIR $ENV{BOOST_ROOT}/stage64/lib)
ELSE()
	SET(PROJ_NAME gw2dps)
ENDIF()

PROJECT(${PROJ_NAME})

#wxWidget directory
SET(wxWidgets_ROOT_DIR $ENV{WXWIDGET_ROOT})

FIND_PACKAGE(wxWidgets REQUIRED core base)

# boost directory
SET(BOOST_ROOT $ENV{BOOST_ROOT})
SET(Boost_USE_STATIC_LIBS ON)

FIND_PACKAGE(Boost 1.59.0 REQUIRED system thread date_time timer chrono regex)

# to use unicode and avoid errors
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

# MFC
# ADD_DEFINITIONS(-D_AFXDLL)
# SET(CMAKE_MFC_FLAG 2)

# gw2dps.dll
ADD_LIBRARY(${PROJ_NAME}_dll SHARED
	gw2dps/config.h
	gw2dps/config.cpp
	gw2dps/keymap.h
	gw2dps/keymap.cpp
	gw2dps/hotkey.h
	gw2dps/hotkey.cpp
	gw2dps/preferences.h
	gw2dps/preferences.cpp
	gw2dps/assist_drawer.h
	gw2dps/assist_drawer.cpp
	gw2dps/common_typedefs.h
	gw2dps/raid/encounter_timer.h
	gw2dps/raid/encounter_timer.cpp
	gw2dps/raid/squad_member.h
	gw2dps/raid/squad_member.cpp
	gw2dps/raid/squad.h
	gw2dps/raid/squad.cpp
	gw2dps/boss/skills/draw/meter.h
	gw2dps/boss/skills/draw/meter.cpp
	gw2dps/boss/skills/unstable_pylon.h
	gw2dps/boss/skills/unstable_pylon.cpp
	gw2dps/boss/skills/seeker.h
	gw2dps/boss/skills/seeker.cpp
	gw2dps/boss/skills/magic_storm.h
	gw2dps/boss/skills/magic_storm.cpp
	gw2dps/boss/skills/flamewall.h
	gw2dps/boss/skills/flamewall.cpp
	gw2dps/boss/skills/cannon.h
	gw2dps/boss/skills/cannon.cpp
	gw2dps/boss/raid_boss.h
	gw2dps/boss/raid_boss.cpp
	gw2dps/boss/unknown_boss.h
	gw2dps/boss/unknown_boss.cpp
	gw2dps/boss/vale_guardian.h
	gw2dps/boss/vale_guardian.cpp
	gw2dps/boss/gorseval.h
	gw2dps/boss/gorseval.cpp
	gw2dps/boss/sabetha.h
	gw2dps/boss/sabetha.cpp
	gw2dps/boss/raid_boss_factory.h
	gw2dps/boss/raid_boss_factory.cpp
	gw2dps/gw2dps.h
	gw2dps/gw2dps.cpp
	gw2dps/gw2dps_dll.rc)

SET_TARGET_PROPERTIES(${PROJ_NAME}_dll PROPERTIES
	FOLDER ${PROJ_NAME}_dll
	OUTPUT_NAME ${PROJ_NAME})

TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME}_dll PUBLIC
	${Boost_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(${PROJ_NAME}_dll
	hacklib_gw2
	${Boost_LIBRARIES})

# gw2dps.exe
IF(NOT ARCH_64BIT)
ADD_EXECUTABLE(${PROJ_NAME}_exe WIN32
	gw2dps/config.h
	gw2dps/config.cpp
	gw2dps/keymap.h
	gw2dps/keymap.cpp
	gw2dps/hotkey.h
	gw2dps/hotkey.cpp
	injector/MainFrame.h
	injector/MainFrame.cpp
	injector/ChangeKeyFrame.h
	injector/ChangeKeyFrame.cpp
	injector/MainPane.h
	injector/MainPane.cpp
	injector/MainApp.h
	injector/MainApp.cpp
	injector/Inject.h
	injector/Inject.cpp
	injector/HotKeyChangeButton.h
	injector/HotKeyChangeButton.cpp
	injector/Icon.rc)

SET_TARGET_PROPERTIES(${PROJ_NAME}_exe PROPERTIES
	FOLDER ${PROJ_NAME}_exe
	OUTPUT_NAME ${PROJ_NAME})

TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME}_exe PUBLIC
	${Boost_INCLUDE_DIR}
	${wxWidgets_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(${PROJ_NAME}_exe
	${Boost_LIBRARIES}
	${wxWidgets_LIBRARIES})
ENDIF()

# proxy64.exe
IF(ARCH_64BIT)
ADD_EXECUTABLE(proxy64 WIN32
	proxy64/Main.cpp)

SET_TARGET_PROPERTIES(proxy64 PROPERTIES
	FOLDER ${PROJ_NAME}_exe
	OUTPUT_NAME proxy64)
ENDIF()
