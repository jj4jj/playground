
# playeground cmake cfg 
project(playeground CXX)
set(playeground_VERSION 0.0.1)

# 
cmake_minimum_required(VERSION 2.6)

# building must be in not source
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" BUILDING_IN_SOURCE)

if(BUILDING_IN_SOURCE)
  message(FATAL_ERROR
    "playeground compile dir must not be source dir , please remove 'CMakeCache.txt' in current dir , then create a building dir in which dir you can exec commands like this 'cmake <src dir> [options]' "
  )
endif()

# compile option
option(DEBUG "Debug mode" 1)
option(PCH "Use precompiled headers" 0)

#ADD_DEFINITIONS(-D_USE_SYSTEM_LUA)

if(DEBUG)
  message("Build in debug-mode   : Yes")
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
  message("Build in debug-mode   : No  (default)")
endif()



# debug common
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wfatal-errors -Wextra -g -rdynamic -gdwarf-2 -g3 -ggdb3 -pg -Wl,-Bdynamic -ldl -lm -pthread  ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -rdynamic -gdwarf-2 -Wall -Wfatal-errors -pg -Wextra -Wno-unused-parameter -ldl -lm -pthread ")

# release mode
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -g --no-warnings -ldl -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -g --no-warnings -ldl -pthread")


# output dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


set(SSUKITS_HOME ${CMAKE_SOURCE_DIR}/dep/ssukits)
set(PROTOBUF_HOME ${CMAKE_SOURCE_DIR}/dep/protobuf)
set(INIPARSER_HOME ${CMAKE_SOURCE_DIR}/dep/iniparser)
set(ZEROMQ_HOME ${CMAKE_SOURCE_DIR}/dep/zeromq)
set(MYSQLCLI_C_HOME	${CMAKE_SOURCE_DIR}/dep/mysqlcconn)
set(HIREDIS_HOME ${CMAKE_SOURCE_DIR}/dep/hiredis)
set(LUA_HOME ${CMAKE_SOURCE_DIR}/dep/ssukits/dep/lua-5.2.3)

# install dir
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)


add_subdirectory(channel)
add_subdirectory(gate)
add_subdirectory(client/gate)
add_subdirectory(benchmark)
add_subdirectory(component)
add_subdirectory(app)
add_subdirectory(db)
add_subdirectory(proto)
add_subdirectory(datacenter)
add_subdirectory(test)
add_subdirectory(tools)
add_subdirectory(rgtd/zone)
add_subdirectory(client)

