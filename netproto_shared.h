#ifndef NETPROTO_SHARED_H
#define NETPROTO_SHARED_H

#define NETPROTO_PROTOCOL_NAME "netproto"
#define NETPROTO_PORT 42042

#include <stdint.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

enum
{
    LOG_INFO,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_TRACE,
    LOG_WARNING
};

enum ConnectStatus {
    CONNECTION_REFUSED_SERVER_FULL = 100,    
};

struct ClientGameInfo {
    uint64_t game_id; 
    uint8_t num_players;
};


#define NBN_LogInfo(...) Log(LOG_INFO,  __VA_ARGS__)
#define NBN_LogError(...) Log(LOG_ERROR, __VA_ARGS__)
#define NBN_LogDebug(...) Log(LOG_DEBUG, __VA_ARGS__)
#define NBN_LogTrace(...) Log(LOG_TRACE, __VA_ARGS__)
#define NBN_LogWarning(...) Log(LOG_WARNING, __VA_ARGS__)

void Log(int, const char*, ...);

#include <nbnet.h>
#include <net_drivers/udp.h>

#endif // NETPROTO_SHARED_H
