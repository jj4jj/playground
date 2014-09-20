
#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}  
  ${INIPARSER_HOME}/src
)

#it will generate a static lib for comiling faster
add_library(playground_app STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/App.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/App.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/AppContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AppContext.h
)

INSTALL(TARGETS playground_app DESTINATION  lib)

