// #include <stdio.h>
#include <ctype.h>

#define NBNET_IMPL

#include "netproto_shared.h"

struct ClientState {
    bool running = true;
    bool connected = false;
    bool disconnected = false;
};

static ClientState g_client;

void OnConnected(void)
{
    Log(LOG_INFO, "Connected");
    g_client.connected = true;
}

void OnDisconnected(void)
{
    Log(LOG_INFO, "Disconnected");
    g_client.disconnected = true;
    g_client.running = false;

    if (NBN_GameClient_GetServerCloseCode() == CONNECTION_REFUSED_SERVER_FULL)
    {
        Log(LOG_INFO, "Another Client is already connected");
    }
}

void OnMessageReceived(void)
{
    NBN_MessageInfo msg_info = NBN_GameClient_GetMessageInfo();
    
    Log(LOG_INFO, "Recieved Messsage %d\n", msg_info.type);

    //EchoMessage_Destroy(msg);
}



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
    else {
        printf("Started client success....\n");
    }
    
    printf("Mainloop\n");
    while (g_client.running)
    {
        int ev;        

        // Poll for client events
        while ((ev = NBN_GameClient_Poll()) != NBN_NO_EVENT)
        {
            if (ev < 0)
            {
                Log(LOG_ERROR, "An error occured while polling client events. Exit");

                // Stop main loop
                g_client.running = false;
                break;
            }

            switch (ev)
            {
                printf("Got event %d\n", ev);

                // Client is connected to the server
                case NBN_CONNECTED:
                    OnConnected();                    
                    break;

                    // Client has disconnected from the server
                case NBN_DISCONNECTED:
                    OnDisconnected();
                    break;

                    // A message has been received from the server
                case NBN_MESSAGE_RECEIVED:
                    //OnMessageReceived();
                    printf("Message Received....\n");
                    break;
            }
        }

        if (g_client.disconnected)
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
            g_client.running = false;
            break;
        }

        // Cap the client tick rate
#if defined(_WIN32) || defined(_WIN64)
        Sleep(0.1 * 1000);
#else
        long nanos = 0.1 * 1e9;
        struct timespec t = {.tv_sec = nanos / 999999999, .tv_nsec = nanos % 999999999};
        nanosleep(&t, &t);        
#endif
    }

    // Stop and deinitialize the client
    NBN_GameClient_Stop();


}