#include <stdio.h>
#include <ctype.h>

#define NBNET_IMPL

#include "netproto_shared.h"

#define ID_INACTIVE (0)

#define MAX_CLIENTS (8)

struct GameClient
{
    NBN_ConnectionHandle _hclient;
};

struct GameSession
{
    u64 _id; // session ID
    
    int _numClients = 0;
    GameClient _client[MAX_CLIENTS];
};


int main( int argc, char **argv )
{
    printf("Hello netproto...\n");

    // For processing incoming connections
    uint8_t connectionDataBuff[NBN_CONNECTION_DATA_MAX_SIZE];
    
    // For now just one session allowed, if this gets beyond proto
    // could host multiple sessions
    GameSession session = {};

    NBN_UDP_Register(); // Register the UDP driver

    if (NBN_GameServer_StartEx(NETPROTO_PROTOCOL_NAME, NETPROTO_PORT, false ) < 0)
    {
        printf("Failed to start server....\n");
        return 1;
    }

    // main loop
    bool error = false;
    while (!error) {

        int ev;

        while ((ev = NBN_GameServer_Poll()) != NBN_NO_EVENT)
        {
            if (ev < 0) {
                printf("GameServer_Poll error? %d\n", ev);
                error = true;
                break;
            }

            switch (ev) {
                case NBN_NEW_CONNECTION:
                    if (session._id == ID_INACTIVE) {

                        
                        // make sure connection session ID matches our session
                        //NBN_GameServer_ReadIncomingConnectionData(connectionDataBuff);


                        // No session yet, use this one
                        NBN_GameServer_AcceptIncomingConnection();

                        // TODO read session data
                        session._id = 1234;

                        GameClient* client = &session._client[session._numClients];
                        client->_hclient = NBN_GameServer_GetIncomingConnection();
                    }
                    else {
                        NBN_GameServer_RejectIncomingConnectionWithCode( CONNECTION_REFUSED_SERVER_FULL );
                    }
                    break;

                case NBN_CLIENT_DISCONNECTED:
                {
                    // TODO handle this
                    NBN_ConnectionHandle client = NBN_GameServer_GetDisconnectedClient();
                    printf("Client %d disconnected\n");
                    break;
                }

                case NBN_CLIENT_MESSAGE_RECEIVED:
                    NBN_MessageInfo msg_info = NBN_GameServer_GetMessageInfo();
                    break;
            }
        }

    }
    
    NBN_GameServer_Stop();

    return 0;
}
