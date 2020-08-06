cmake_minimum_required(VERSION "3.5")

project("Stars" VERSION "1.0")

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
 
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "current platform: Linux ")
	set(jsoncpp_DIR "~/vcpkg/installed/x64-linux/share/jsoncpp")
	set(CMAKE_INSTALL_PREFIX "/usr/local")
	set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	MESSAGE(STATUS "current platform: Windows")
	set(CMAKE_INSTALL_PREFIX "D:\\Program Files\\Stars")
	set(jsoncpp_DIR "C:\\dev\\vcpkg\\installed\\x64-windows\\share\\jsoncpp")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

find_package(jsoncpp 1.9.2 CONFIG REQUIRED)

get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})

add_library(stars_lib STATIC
	"src/boardAnalyse.cpp"
	"src/boardInterface.cpp"
	"src/boardRecord.cpp"
	"src/boardState.cpp"
)

target_include_directories(stars_lib
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
		stars_lib
		jsoncpp_lib
)

# Install - quite a failure
# Binaries
install (TARGETS ${PROJECT_NAME}
	DESTINATION bin
)

# Library
# install (TARGETS stars_lib
# 	LIBRARY DESTINATION lib
# 	ARCHIVE DESTINATION lib
# 	RUNTIME DESTINATION bin
# )

# install (TARGETS jsoncpp_lib
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib
#	RUNTIME DESTINATION bin
#)

# Header files
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
# 	DESTINATION include)

include(CPack)
