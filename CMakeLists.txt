project( netproto )

set(TOOLKITS_BASE "c:/Toolkits/")

set(CMAKE_CXX_STANDARD 20)

include_directories( ${TOOLKITS_BASE}/nbnet/  )


add_executable( netproto_serv
                    netproto_shared.h
                    netproto_shared.cpp
                    server_main.cpp )

target_link_libraries(netproto_serv wsock32 ws2_32)



