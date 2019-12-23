
/*

Made for the sake of simple, easy and readable code.
It comes under no licens or warrany.

Author: Addanpadda (Adam Herbertsson) December 2019 (Marry Cristmas!)

*/


#include <iostream>
#include "getlib.hpp"

#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netdb.h>

#include <unistd.h>
#include <alloca.h>
#include <cstring>


void Connection::Connect()
{
    addrInfo.sin_port = htons(port);

    if(connect(sock, (struct sockaddr*)&addrInfo, sizeof(addrInfo)) == -1)
        throw("ERROR: Could not connect to host");
}

void Connection::SendRequest()
{
    if (write(sock, request, requestSize) == -1)
        throw("ERROR: Could not send request");
}

void Connection::ReceiveRequest()
{
    int responseIndex = 0;
    int responseLen;

    while ((responseLen = read(sock, buffer, MAX_RESPONSE_LEN_PER_READ)) > 0)
    {
        for (int index = 0; index < responseLen; index++)
        {
            response[responseIndex] = buffer[index];
            responseIndex++;
        }
        // Most likely last read cycle
        if (responseLen != MAX_RESPONSE_LEN_PER_READ)
            break;
    }

    if (responseLen == -1)
        throw("ERROR: Could not read the response");
}


Connection::Connection()
{
    if ((sock = socket(AF_INET, SOCK_STREAM, 6)) == -1)
        throw("ERROR: Could not initialize socket");

    addrInfo.sin_family = AF_INET;
    getPath             = "/";
}

Connection::Connection(const int &domain, const int &type, const int &protocol)
{
    if ((sock = socket(AF_INET, SOCK_STREAM, 6)) == -1);
        throw("ERROR: Could not initialize socket");

    addrInfo.sin_family = domain;
    getPath             = "/";
}

Connection::~Connection()
{
    close(sock);
}


void Connection::SetPort(const int &passedPort)
{
    port = passedPort;
}

void Connection::SetHost(const std::string &passedHost)
{
    if ((hostent = gethostbyname(passedHost.c_str())) == NULL)
        throw("ERROR: Could not get host by name");
    
    host                     = passedHost;
    in_addr                  = inet_addr(inet_ntoa(*(struct in_addr*)*(hostent->h_addr_list)));
    addrInfo.sin_addr.s_addr = in_addr;
}

void Connection::SetGetPath(const std::string &passedPath)
{
    getPath = passedPath;
}

void Connection::SetUserAgent(const std::string &passedUserAgent)
{
    userAgent = passedUserAgent;
}


void Connection::Request()
{
    if ((requestSize = snprintf(request, REQUEST_SIZE_LIMIT, "GET %s HTTP/1.1\nUser-Agent: %s\nHost: %s\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\n\n", getPath.c_str(), userAgent.c_str(), host.c_str())) < 0)
        throw("ERROR: Could not put togheter the request");

    Connect();
    SendRequest();
    ReceiveRequest();
}
