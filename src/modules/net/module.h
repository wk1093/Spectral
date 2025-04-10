/**
 * @file net/module.h
 * @brief Network module header file
 * @details This file implements very basic, but cross-platform network functionality.
 * There will be a part of the ECS for server and client data management. It will use this module.
 * This file does add a few nice features, such as packet management and ids.
 * It also helps the server identify clients.
 * I will probably make authentication a separate module so the game developer can choose how the authentication works, for now there will be a lambda authentication where the game developer can define the authentication function.
 */

#pragma once

#include "moduleLib.h"

typedef uint16_t sPacketId;
typedef uint32_t sClientId;

// Preuser defined packet ids (THESE CANNOT BE USED FOR DATA PACKETS)
// since the serverbound and clientbound packets are completely separate, they can use the same ids.
// very often the serverbound ids will be corresponding to the clientbound response ids, but not always.
#define PACKET_SB_AUTH 0x0001 // Serverbound authentication packet
#define PACKET_CB_AUTHSUCCESS 0x0001 // Clientbound authentication success packet
#define PACKET_SB_DISCONNECT 0x0002 // Serverbound disconnect packet
#define PACKET_CB_DISCONNECT 0x0002 // Clientbound disconnect packet, will be sent before the server kicks/disconnects the client, this is the response of a failed authentication.
#define PACKET_SB_PING 0x0003 // Serverbound keepalive packet, ping/pong
#define PACKET_CB_PING 0x0003 // Clientbound keepalive packet, ping/pong

inline void serializeU16(std::ostream& os, uint16_t value) {
    // os.write(reinterpret_cast<const char*>(&value), sizeof(value));
    // reinterpret_cast is risky since some machines are little endian and some are big endian.
    // So we will use bit shifting to uniformly convert the value to bytes.
    os.put((value >> 8) & 0xFF); // high byte
    os.put(value & 0xFF); // low byte
}
inline void serializeU32(std::ostream& os, uint32_t value) {
    os.put((value >> 24) & 0xFF); // high byte
    os.put((value >> 16) & 0xFF); // high byte
    os.put((value >> 8) & 0xFF); // low byte
    os.put(value & 0xFF); // low byte
}
inline void serializeU8(std::ostream& os, uint8_t value) {
    os.put(value);
}
inline void serializeString(std::ostream& os, const std::string& str) {
    uint16_t len = str.length();
    if (len > 0xFFFF) {
        printf("String length exceeds maximum length of 65535 bytes\n");
        const char* error = "String length exceeds maximum length of 65535 bytes\n";
        serializeU16(os, (uint16_t)strlen(error));
        os.write(error, strlen(error));
        return;
    }
    serializeU16(os, len);
    os.write(str.c_str(), len);
}

struct sServerboundPacket {
    sPacketId id;
    sClientId clientId;
    size_t size;
    uint8_t *data;
};

struct sClientboundPacket {
    sPacketId id;
    size_t size;
    uint8_t *data;
};

struct sPacketSBAuth {
    sPacketId id = PACKET_SB_AUTH;
    sClientId clientId = 0;
    std::string serverIp; // server ip (from the client's perspective)
    uint8_t* authData;
    size_t authSize;

    /**
     * Format:
     * Header - same as all packets, PacketId, ClientId, Complete size of the packet's data (not including the header itself)
     * serializeString(serverIp) - the server ip
     * byte array - the authentication data. Maybe a string, but depends on the chosen method of authentication.
     */
};

struct sPacketCBAuthSuccess {
    sPacketId id = PACKET_CB_AUTHSUCCESS;
    sClientId assignedId; // id assigned by the server.
};

struct sServer {
    void* internal;

    int port;
    const char* ip;

    std::vector<std::pair<sClientId, std::string>> connectedClients; // <clientId, clientIp> the ip being here helps prevent a bad actor from trying to connect by guessing ids.

    struct BadAuthAttempt {
        std::string ip;
        size_t attempts;
        size_t timeToDecrement;
    };

    std::vector<BadAuthAttempt> badAuthAttempts; // whenever a client fails to authenticate, the ip will be stored here and the number will be incremented. It will add 5 minutes to the current time and put that in the timeToDecrement field.
    // if the attempt count is greater than 3, the client will be unable to connect, this number will slowly drop due to the timeToDecrement field. which will subtract 1 every 5 minutes until it reaches 0.
    // if the client tries to connect while having 3 or more, it will add another even if they authenticated properly, and it will act as if they didn't authenticate properly.

};

struct sClient {
    void* internal;

    int port;
    const char* ip;

    sClientId id;
    bool authenticated;
    bool connected;
};

namespace network {
    // basic functionality functions
    typedef sServer*(*CreateServer)(const char* ip, int port);
    typedef sClient*(*CreateClient)(const char* ip, int port);
    typedef void(*DestroyServer)(sServer* server);
    typedef void(*DestroyClient)(sClient* client);

    typedef void(*StartServer)(sServer* server);
    typedef void(*ConnectClient)(sClient* client, const uint8_t* authData, size_t authSize);
    typedef void(*DisconnectClient)(sClient* client);
    typedef void(*KickClient)(sServer* server, sClientId clientId);
    typedef void(*SendPacket)(sClient* client, sClientboundPacket packet);
    typedef void(*SendPacketToClient)(sServer* server, sClientId clientId, sClientboundPacket packet);



}