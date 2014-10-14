
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
	${HIREDIS_HOME}
)




#it will generate a exe
ADD_EXECUTABLE(config_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.cpp
)

#assign custrom link lib
target_link_libraries(config_test
	playground_component
	libssukits_base.a
	libiniparser.a
)
#installing
INSTALL(TARGETS  config_test DESTINATION test/)



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
ADD_EXECUTABLE(app_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestApp.cpp
)
target_link_libraries(app_test
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
INSTALL(TARGETS  app_test DESTINATION test/)


###################################
ADD_EXECUTABLE(mysql_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestMysql.cpp
)
target_link_libraries(mysql_test
	dbagent
	ssukits_base
	ssukits_thread
	libmysqlclient.a	
	rt
	dl
)
INSTALL(TARGETS  mysql_test DESTINATION test/)


###################################
ADD_EXECUTABLE(shm_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestShm.cpp
)
target_link_libraries(shm_test
	datacenter
 	ssukits_ipc
	ssukits_base
	rt
	dl
)
INSTALL(TARGETS  shm_test DESTINATION test/)


###################################
ADD_EXECUTABLE(seri_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestMemSeri.cpp
)
target_link_libraries(seri_test
	datacenter
 	ssukits_ipc
	ssukits_base
	playground_proto
	protobuf
	rt
	dl
)
INSTALL(TARGETS  seri_test DESTINATION test/)


###################################
ADD_EXECUTABLE(cache_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestCache.cpp
)
target_link_libraries(cache_test
	datacenter
 	ssukits_ipc
	ssukits_base
	playground_proto
	protobuf
	libhiredis.a
	libev.a
	rt
	dl
)
INSTALL(TARGETS  cache_test DESTINATION test/)

###################################
ADD_EXECUTABLE(db_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestDB.cpp
)
target_link_libraries(db_test
	datacenter
	dbagent
 	ssukits_ipc
	ssukits_thread
	ssukits_base
	libmysqlclient.a		
	libev.a
	playground_proto
	protobuf
	rt
	dl
)
INSTALL(TARGETS  db_test DESTINATION test/)

###################################
ADD_EXECUTABLE(dataagent_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestDataAgent.cpp
)
target_link_libraries(dataagent_test
	datacenter
 	ssukits_ipc
	ssukits_base
	rt
	dl
)
INSTALL(TARGETS  dataagent_test DESTINATION test/)


###################################
ADD_EXECUTABLE(pb_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestProtoBuf.cpp
)
target_link_libraries(pb_test
	ssukits_base
	playground_proto
	protobuf
	rt
	dl
)
INSTALL(TARGETS  pb_test DESTINATION test/)
###################################
ADD_EXECUTABLE(res_test
	${CMAKE_CURRENT_SOURCE_DIR}/TestRes.cpp
)
target_link_libraries(res_test
	datacenter
	playground_proto
	ssukits_base
	protobuf
	rt
	dl
)
INSTALL(TARGETS  res_test DESTINATION test/)


