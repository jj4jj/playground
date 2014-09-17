
#the compiling files
file(GLOB_RECURSE components_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)
#
#

message(${components_SRCS})
#message(${gate_client_SRCS})
#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}  
  ${INIPARSER_HOME}/src
)

#base dir linked lib dir
#
#link_directories(
#  /usr/local/lib
#)

#it will generate a static lib for comiling faster
add_library(playground_component STATIC
	${components_SRCS}
)

#set_target_properties(ssukits_net PROPERTIES LINKER_LANGUAGE CXX)

INSTALL(TARGETS playground_component DESTINATION  lib)


#add_library(gate_proto STATIC
#	${gate_proto_SRCS} )
#INSTALL(TARGETS gate_proto DESTINATION client/)

