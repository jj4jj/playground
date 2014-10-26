#the compiling files
file(GLOB_RECURSE playground_agent_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cc *.h)
SET_SOURCE_FILES_PROPERTIES(${playground_gate_SRCS} PROPERTIES LANGUAGE CXX )


#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/proto/gen
  ${SSUKITS_HOME}
  ${INIPARSER_HOME}/src
  ${MYSQLCLI_C_HOME}/include
  ${LUA_HOME}/include
)

#base dir linked lib dir
#
link_directories(
	${SSUKITS_HOME}/bin
	${PROJECT_SOURCE_DIR}/lib
	/usr/lib
	${INIPARSER_HOME}	
	${MYSQLCLI_C_HOME}/lib
	${HIREDIS_HOME}
  	${LUA_HOME}/lib
)

#it will generate a exe
ADD_EXECUTABLE(agent
		${playground_agent_SRCS}
)

#assign custrom link lib
target_link_libraries(agent
	playground_app
	playground_component
	datacenter
	dbagent
	playground_proto
	channel
	ssukits_utility
	ssukits_net
	ssukits_ipc
	ssukits_thread
	ssukits_script
	ssukits_base
	libmysqlclient.a
	libhiredis.a
	liblua.a
	libev.a
	iniparser
	protobuf
	zmq
	rt
	dl
)


#installing
INSTALL(TARGETS  agent DESTINATION agent/)



