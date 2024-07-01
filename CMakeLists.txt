cmake_minimum_required(VERSION 3.27)
project( netproto )

# THis is where I keep other libs
set( TOOLKITS_BASE $ENV{TOOLKITS_BASE})

if (NOT TOOLKITS_BASE)
   set(TOOLKITS_BASE "c:/Toolkits/")
endif()

set(CMAKE_CXX_STANDARD 20)

include_directories( ${TOOLKITS_BASE}/nbnet/  )


add_executable( netproto_serv
                    netproto_shared.h
                    netproto_shared.cpp
                    server_main.cpp )

add_executable( np_testclient
                    netproto_shared.h
                    netproto_shared.cpp
                    testclient_main.cpp )


if(WIN32)
target_link_libraries(netproto_serv wsock32 ws2_32)
endif()



