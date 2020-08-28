cmake_minimum_required(VERSION "3.5")


# Prepare
## change these versions below to make a version change that applies to the whole
## program
set(STARS_VERSION_DEBUG "0.1.1-beta")
set(STARS_VERSION_RELEASE "0.1.1")
project("Stars" VERSION ${STARS_VERSION_RELEASE})
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()
if (NOT STARS_LANGUAGE_SETIING)
	message("Setting language to 'English' as none was specified.")
	set(STARS_LANGUAGE_SETIING "English" CACHE STRING "Choose the language of program." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE STARS_LANGUAGE_SETIING PROPERTY STRINGS "English" "Chinese")
endif ()
set(DEPENDENCIES_SOURCE_DIR "${PROJECT_SOURCE_DIR}/dependencies")


# Main building session
add_library(json_lib STATIC
	${DEPENDENCIES_SOURCE_DIR}/src/json/jsoncpp.cpp
)
target_include_directories(json_lib
	PUBLIC
		${DEPENDENCIES_SOURCE_DIR}/include
)

add_library(starboard_lib STATIC
	src/boardAnalyse.cpp
	src/boardInterface.cpp
	src/boardRecord.cpp
	src/boardRoute.cpp
	src/boardState.cpp
	src/boardTest.cpp
	src/boardTools.cpp
)
target_include_directories(starboard_lib
	PUBLIC 
		${PROJECT_SOURCE_DIR}/include
		${DEPENDENCIES_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
	PRIVATE 
		starboard_lib
		json_lib
)


# Set macros
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_compile_definitions(${PROJECT_NAME}
		PUBLIC STARS_PLATFORM_LINUX
	)
	message (STATUS "Current platform: Linux")
	if (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
		target_compile_definitions(starboard_lib 
			PUBLIC STARS_DEBUG_INFO
		)
		add_definitions(-DSTARS_VERSION_DEBUG="${STARS_VERSION_DEBUG}")
		message (STATUS "Current version: ${STARS_VERSION_DEBUG}")
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (with debug info)")
	else () # release
		add_definitions(-DSTARS_VERSION_RELEASE="${STARS_VERSION_RELEASE}")
		message (STATUS "Current version: ${STARS_VERSION_RELEASE}")
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (without debug info)")
	endif()
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
	################## change this to make a Debug version ###################
	# set(CMAKE_BUILD_TYPE "Debug")
	set(CMAKE_BUILD_TYPE "Release")
	################## change this to make a release version ###################
	
	############### change this to make a English version ######################
	set(STARS_LANGUAGE_SETIING "English" CACHE STRING "Choose the language of program." FORCE)
	# set(STARS_LANGUAGE_SETIING "Chinese" CACHE STRING "Choose the language of program." FORCE)
	############### change this to make a Chinese version ######################
	target_compile_definitions(${PROJECT_NAME}
		PUBLIC STARS_PLATFORM_WINDOWS
	)
	if (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
		add_definitions(-DSTARS_VERSION_DEBUG="${STARS_VERSION_DEBUG}")
		message (STATUS "Current version: ${STARS_VERSION_DEBUG}")
		target_compile_definitions(starboard_lib PUBLIC STARS_DEBUG_INFO)
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (with debug info)")
	else () # release and such
		add_definitions(-DSTARS_VERSION_RELEASE="${STARS_VERSION_RELEASE}")
		message (STATUS "Current version: ${STARS_VERSION_RELEASE}")
		message (STATUS "Current build type: ${CMAKE_BUILD_TYPE} (without debug info)")
	endif()
	message (STATUS "Current platform: Windows")
endif ()

## set language
if (STARS_LANGUAGE_SETIING MATCHES "Chinese")
	target_compile_definitions(starboard_lib PUBLIC STARS_LANG_CHINESE)
	message(STATUS "Program language: Chinese")
elseif (STARS_LANGUAGE_SETIING MATCHES "English")
	message(STATUS "Program language: English")
else ()
	message(WARNING "Wrong language settings")
endif()


# Install - quite a failure
install (TARGETS ${PROJECT_NAME}
	DESTINATION bin
)
