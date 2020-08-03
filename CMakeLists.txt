cmake_minimum_required(VERSION "3.16.0")

project("CharmedCpp")

add_executable(
	"${PROJECT_NAME}"
	"main.cpp"
	"lib/boardAnalyse.cpp"
	"lib/boardInterface.cpp"
	"lib/boardRecord.cpp"
	"lib/boardState.cpp"
	"lib/jsoncpp.cpp"
	"lib/shortList.cpp"
)


# these three is enough to build a exe

# install(TARGETS "${PROJECT_NAME}" DESTINATION bin)	# ${CMAKE_INSTALL_PREFIX}/bin
# install(FILES "main.cpp" DESTINATION src)