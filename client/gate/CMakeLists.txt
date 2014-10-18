
#the compiling files
file(GLOB_RECURSE gate_client_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)

#message(${gate_proto_SRCS})
#message(${gate_client_SRCS})
#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}
)

#base dir linked lib dir
#
#link_directories(
#  /usr/local/lib
#)

#it will generate a static lib for comiling faster
add_library(gate_client STATIC
	${gate_client_SRCS}
)

#set_target_properties(ssukits_net PROPERTIES LINKER_LANGUAGE CXX)

INSTALL(TARGETS gate_client DESTINATION lib/)

