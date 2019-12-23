#pragma once


#include <arpa/inet.h>
#include <unistd.h>


/*
    This is how big the request being receive
    are maximum allowed to be.
*/
#define REQUEST_SIZE_LIMIT 1024

/*
    This is the maximum size that can be received
    per read() at a time.
*/
#define MAX_RESPONSE_LEN_PER_READ 1418

/*
    This is how many characters the response from
    the server is going to be at absolutly maximum.
    Could be changed to bigger if it doesn´t hold up
    (or for extreme memory efficiency).
*/
#define MAX_RESPONSE_SIZE 20480


class Connection
{
private:

    int sock;
    unsigned short port = 80;
    struct hostent *hostent;
    struct sockaddr_in addrInfo;
    in_addr_t in_addr;

    int requestSize;
    std::string host;
    std::string getPath;
    // Manipulating Windows using firefox.
    std::string userAgent = "Mozilla/4.0 (compatible; MSIE5.01; Windows NT)";

    char request[REQUEST_SIZE_LIMIT];
    char buffer[MAX_RESPONSE_LEN_PER_READ];


    void Connect();

    void SendRequest();

    void ReceiveRequest();


public:

    /*
        This variable contains the response from
        the website.
    */
    char response[MAX_RESPONSE_SIZE];


    /*
        Constructor for the library which
        initiallizes the socket.
        (Everything set to default)
   */
    Connection();

    /*
        Constructor for the library which
        initiallizes the socket.
    */
    Connection(const int &domain, const int &type, const int &protocol);

    ~Connection();


    /*
        This functions sets the port that
        the remote server uses. (default 80).
    */
    void SetPort(const int &passedPort);

    /*
        This function sets the hostname
        (domain name) that the remote server has.
    */
    void SetHost(const std::string &passedHost);

    /*
        This function sets the path that
        the library is going to fetch. (default /).
    */
    void SetGetPath(const std::string &passedPath);
    
    void SetUserAgent(const std::string &passedUserAgent);


    /*
        This function makes the actual request to
        the remote server.
    */
    void Request();
};