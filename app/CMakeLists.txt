
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} playground_app_SRCS)

#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}  
  ${INIPARSER_HOME}/src
)

#it will generate a static lib for comiling faster
add_library(playground_app STATIC
	${playground_app_SRCS}
)

INSTALL(TARGETS playground_app DESTINATION  lib)

