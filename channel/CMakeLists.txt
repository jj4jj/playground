#the compiling files
file(GLOB_RECURSE channel_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)


#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}
)

#base dir linked lib dir
#
link_directories(
  /usr/local/lib
  ${SSUKITS_HOME}/lib
  ${PROJECT_SOURCE_DIR}/lib
)

#it will generate a static lib for comiling faster
add_library(channel STATIC
  ${channel_SRCS}
)
#set_target_properties(channel PROPERTIES LINKER_LANGUAGE CXX)




