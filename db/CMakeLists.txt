aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} db_agent_SRCS)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${SSUKITS_HOME}
  ${MYSQLCLI_C_HOME}/include
)
add_library(dbagent STATIC
	${db_agent_SRCS}
)

INSTALL(TARGETS dbagent DESTINATION lib/)
