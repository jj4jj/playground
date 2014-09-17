#the compiling files
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} console_client_SRCS)

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
ADD_EXECUTABLE(console
	${console_client_SRCS}	
)

#assign custrom link lib
target_link_libraries(console
	ssukits_net
	ssukits_base
)



#installing
INSTALL(TARGETS  console DESTINATION client/)

