// #include <stdio.h>
#include <ctype.h>

#define NBNET_IMPL

#include "netproto_shared.h"

int main( int argc, char **argv )
{
    printf("Testclient...\n");

    NBN_UDP_Register(); // Register the UDP driver

    if (NBN_GameClient_StartEx( NETPROTO_PROTOCOL_NAME, 
                                "127.0.0.1", 
                                NETPROTO_PORT, 
                                false, NULL, 0) < 0)
    {
        printf("Failed to start client....\n");
        return 1;
    }

    bool running = true;
    while (running)
    {
        int ev;

        bool disconnected = false;

        // Poll for client events
        while ((ev = NBN_GameClient_Poll()) != NBN_NO_EVENT)
        {
            if (ev < 0)
            {
                Log(LOG_ERROR, "An error occured while polling client events. Exit");

                // Stop main loop
                running = false;
                break;
            }

            switch (ev)
            {
                // Client is connected to the server
                case NBN_CONNECTED:
                    // OnConnected();
                    printf("Connected....\n");
                    break;

                    // Client has disconnected from the server
                case NBN_DISCONNECTED:
                    // OnDisconnected();
                    printf("Disconnected....\n");
                    disconnected = true;
                    break;

                    // A message has been received from the server
                case NBN_MESSAGE_RECEIVED:
                    //OnMessageReceived();
                    printf("Message Received....\n");
                    break;
            }
        }

        if (disconnected)
            break;

        // if (connected)
        // {
            // if (SendEchoMessage(msg) < 0)
            // {
            //     Log(LOG_ERROR, "Failed to send message. Exit");

            //     // Stop main loop
            //     running = false;
            //     break;
            // }
        // }

        // Pack all enqueued messages as packets and send them
        if (NBN_GameClient_SendPackets() < 0)
        {
            Log(LOG_ERROR, "Failed to send packets. Exit");

            // Stop main loop
            running = false;
            break;
        }

        // Cap the client tick rate
        long nanos = 0.1 * 1e9;
        struct timespec t = {.tv_sec = nanos / 999999999, .tv_nsec = nanos % 999999999};
        nanosleep(&t, &t);
    }

    // Stop and deinitialize the client
    NBN_GameClient_Stop();


}