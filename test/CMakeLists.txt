
#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}
  ${PROJECT_SOURCE_DIR}/dep/iniparser/src
  ${MYSQLCLI_C_HOME}/include
)

#base dir linked lib dir
#
link_directories(
	${PROJECT_SOURCE_DIR}/lib
	${SSUKITS_HOME}/bin
	/usr/lib
	${PROJECT_SOURCE_DIR}/dep/iniparser	
	${MYSQLCLI_C_HOME}/lib
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
	channel
	ssukits_ipc
	ssukits_utility
	ssukits_net
	ssukits_base
	zmq
	iniparser
)
INSTALL(TARGETS  test_app DESTINATION test/)


###################################
ADD_EXECUTABLE(test_mysql
	${CMAKE_CURRENT_SOURCE_DIR}/TestMysql.cpp
)
target_link_libraries(test_mysql
	dbagent
	ssukits_base
	ssukits_thread
	libmysqlclient.a	
	rt
	dl
)
INSTALL(TARGETS  test_mysql DESTINATION test/)


###################################
ADD_EXECUTABLE(test_shm
	${CMAKE_CURRENT_SOURCE_DIR}/TestShm.cpp
)
target_link_libraries(test_shm
	dataagent
 	ssukits_ipc
	ssukits_base
	rt
	dl
)
INSTALL(TARGETS  test_shm DESTINATION test/)


###################################
ADD_EXECUTABLE(test_cache
	${CMAKE_CURRENT_SOURCE_DIR}/TestCache.cpp
)
target_link_libraries(test_cache
	dataagent
 	ssukits_ipc
	ssukits_base
	rt
	dl
)
INSTALL(TARGETS  test_cache DESTINATION test/)

###################################
ADD_EXECUTABLE(test_db
	${CMAKE_CURRENT_SOURCE_DIR}/TestDB.cpp
)
target_link_libraries(test_db
	dataagent
 	ssukits_ipc
	ssukits_base
	rt
	dl
)
INSTALL(TARGETS  test_db DESTINATION test/)

###################################
ADD_EXECUTABLE(test_dataagent
	${CMAKE_CURRENT_SOURCE_DIR}/TestDataAgent.cpp
)
target_link_libraries(test_dataagent
	dataagent
 	ssukits_ipc
	ssukits_base
	rt
	dl
)
INSTALL(TARGETS  test_dataagent DESTINATION test/)


###################################
ADD_EXECUTABLE(test_seri
	${CMAKE_CURRENT_SOURCE_DIR}/TestMemSeri.cpp
)
target_link_libraries(test_seri
	dataagent
 	ssukits_ipc
	ssukits_base
	playground_proto
	protobuf
	rt
	dl
)
INSTALL(TARGETS  test_seri DESTINATION test/)

