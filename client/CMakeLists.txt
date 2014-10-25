#the compiling files
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} console_client_SRCS)

#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/proto/gen
  ${SSUKITS_HOME}  
)

#base dir linked lib dir
#
link_directories(
	${PROJECT_SOURCE_DIR}/lib
	${SSUKITS_HOME}/bin
	/usr/lib
)

#it will generate a exe
ADD_EXECUTABLE(console
	${CMAKE_CURRENT_SOURCE_DIR}/ConsoleClient.cpp
)

#assign custrom link lib
target_link_libraries(console
	ssukits_net
	ssukits_base
)

#installing
INSTALL(TARGETS  console DESTINATION client/)



#it will generate a exe
###################################
#it will generate a exe
ADD_EXECUTABLE(rgtdClient
	${CMAKE_CURRENT_SOURCE_DIR}/rgtdClient.cpp
)
#assign custrom link lib
target_link_libraries(rgtdClient
	libssukits_base.a
	libssukits_net.a	
	gate_client
	libssukits_thread.a	
	playground_proto
	protobuf.a
)
INSTALL(TARGETS  rgtdClient DESTINATION client/)


