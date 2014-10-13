aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} data_agent_SRCS)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}
  ${MYSQLCLI_C_HOME}/include
  ${HIREDIS_HOME}
)
add_library(dataagent STATIC
	${data_agent_SRCS}
)

INSTALL(TARGETS dataagent DESTINATION lib/)

