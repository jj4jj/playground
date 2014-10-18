#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} playground_proto_SRCS)

file(GLOB_RECURSE playground_proto_SRCS *.cc *.h)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/gen  
)
add_library(playground_proto STATIC
	${playground_proto_SRCS}
)

INSTALL(TARGETS playground_proto DESTINATION lib/)



#######################################################################

file(GLOB_RECURSE playground_proto_gate_SRCS gen/gate/*.cc gen/gate/*.h)
add_library(playground_proto_gate STATIC
	${playground_proto_gate_SRCS}
)
INSTALL(TARGETS playground_proto_gate DESTINATION lib/)



