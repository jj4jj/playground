#the compiling files
file(GLOB_RECURSE playground_test_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)
#

SET_SOURCE_FILES_PROPERTIES(${benchmark_SRCS} PROPERTIES LANGUAGE CXX )


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
	${playground_test_SRCS}	
)

#assign custrom link lib
target_link_libraries(test_config
	playground_component
	libssukits_base.a
	libiniparser.a
)



#installing
INSTALL(TARGETS  test_config DESTINATION test/)
