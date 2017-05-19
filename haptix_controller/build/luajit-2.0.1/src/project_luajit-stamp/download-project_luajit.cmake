message(STATUS "downloading...
     src='http://luajit.org/download/LuaJIT-2.0.1.tar.gz'
     dst='/home/jeremywang/haptix_controller/build/luajit-2.0.1/src/LuaJIT-2.0.1.tar.gz'
     timeout='none'")




file(DOWNLOAD
  "http://luajit.org/download/LuaJIT-2.0.1.tar.gz"
  "/home/jeremywang/haptix_controller/build/luajit-2.0.1/src/LuaJIT-2.0.1.tar.gz"
  SHOW_PROGRESS
  # no EXPECTED_HASH
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'http://luajit.org/download/LuaJIT-2.0.1.tar.gz' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
