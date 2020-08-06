cmake_minimum_required(VERSION "3.5")

project("Stars" VERSION "1.0")

############################################################
# Create a library
############################################################

#Generate the static library from the library sources
add_library(stars_lib STATIC
	"src/boardAnalyse.cpp"
	"src/boardInterface.cpp"
	"src/boardRecord.cpp"
	"src/boardState.cpp"
	"src/jsoncpp.cpp"
)

target_include_directories(stars_lib
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

############################################################
# Create an executable
############################################################

# Add an executable with the above sources
add_executable(${PROJECT_NAME}
    main.cpp
)

# link the new hello_library target with the hello_binary target
target_link_libraries(${PROJECT_NAME}
    PRIVATE 
		stars_lib
)


############################################################
# Install
############################################################

# Binaries
install (TARGETS ${PROJECT_NAME}
    DESTINATION bin)

# Library
# Note: may not work on windows
install (TARGETS stars_lib
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

# Header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
	DESTINATION include)

# # Config
# install (FILES 
# 	etc/Stars_games.json
# 	etc/Stars_settings.json
#     DESTINATION etc)

include(CPack)

set(CPACK_BINARY_ZIP true)
set(CPACK_SOURCE_ZIP true)
