#the compiling files
file(GLOB_RECURSE playground_gate_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cc *.h)
SET_SOURCE_FILES_PROPERTIES(${playground_gate_SRCS} PROPERTIES LANGUAGE CXX )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/proto/gate gate_proto_SRCS)


#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}
  ${INIPARSER_HOME}/src
)

#base dir linked lib dir
#
link_directories(
	${SSUKITS_HOME}/bin
	${PROJECT_SOURCE_DIR}/lib
	/usr/lib
	${INIPARSER_HOME}	
)

#it will generate a exe
ADD_EXECUTABLE(gate
		${playground_gate_SRCS}
		${gate_proto_SRCS}
)

#assign custrom link lib
target_link_libraries(gate
	playground_app
	playground_component
	channel
	ssukits_net
	ssukits_utility
	ssukits_base
	zmq
	rt
	iniparser
	protobuf
)


#installing
INSTALL(TARGETS  gate DESTINATION gate/)


