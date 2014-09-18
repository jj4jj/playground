#the compiling files
#file(GLOB_RECURSE benchmark_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h)
#SET_SOURCE_FILES_PROPERTIES(${benchmark_SRCS} PROPERTIES LANGUAGE CXX )


#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
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
ADD_EXECUTABLE(gate_cli
	${CMAKE_CURRENT_SOURCE_DIR}/TestGate.cpp
)
#assign custrom link lib
target_link_libraries(gate_cli
	libssukits_base.a
	libssukits_net.a	
	gate_client.a
	protobuf.a
)



###################################
ADD_EXECUTABLE(channel_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestChannel.cpp
)
target_link_libraries(channel_test
	libssukits_base.a
	channel
	zmq
)

#installing
INSTALL(TARGETS  gate_cli DESTINATION benchmark/)
INSTALL(TARGETS  channel_test DESTINATION benchmark/)


	
