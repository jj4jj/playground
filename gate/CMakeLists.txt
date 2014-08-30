#the compiling files
file(GLOB_RECURSE playground_gate_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cc *.h)
SET_SOURCE_FILES_PROPERTIES(${playground_gate_SRCS} PROPERTIES LANGUAGE CXX )


#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}
)

#base dir linked lib dir
#
link_directories(
	${SSUKITS_HOME}/lib
	/usr/lib
)

#it will generate a exe
ADD_EXECUTABLE(pingpang_server
		${playground_gate_SRCS}
)

#assign custrom link lib
target_link_libraries(gate
	ssukits_base.a
	ssukits_net.a	
)

#installing
INSTALL(TARGETS  gate DESTINATION gate/)

