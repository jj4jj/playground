
#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}
  ${PROJECT_SOURCE_DIR}/dep/iniparser/src
)

#base dir linked lib dir
#
link_directories(
	${PROJECT_SOURCE_DIR}/lib
	${SSUKITS_HOME}/bin
	/usr/lib
	${PROJECT_SOURCE_DIR}/dep/iniparser	
)

#it will generate a exe
ADD_EXECUTABLE(test_config
	${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.cpp
)

#assign custrom link lib
target_link_libraries(test_config
	playground_component
	libssukits_base.a
	libiniparser.a
)
#installing
INSTALL(TARGETS  test_config DESTINATION test/)



###################################
ADD_EXECUTABLE(channel_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestChannel.cpp
)
target_link_libraries(channel_test
	libssukits_base.a
	channel
	zmq
)
INSTALL(TARGETS  channel_test DESTINATION test/)


###################################
ADD_EXECUTABLE(test_app
	${CMAKE_CURRENT_SOURCE_DIR}/TestApp.cpp
)
target_link_libraries(test_app
	playground_app
	playground_component
	iniparser
	ssukits_utility
	ssukits_net
	ssukits_base
)
INSTALL(TARGETS  test_app DESTINATION test/)




