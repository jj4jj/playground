#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} playground_proto_SRCS)

file(GLOB_RECURSE playground_proto_SRCS *.cc *.h)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)
add_library(playground_proto STATIC
	${playground_proto_SRCS}
)

INSTALL(TARGETS playground_proto DESTINATION lib/)



