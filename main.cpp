#include <iostream>
#include "getlib.hpp"


int main()
{
    try {
        Connection req;
        
        req.SetHost("www.google.com");
        req.SetGetPath("/");
        req.Request();

        std::cout << req.response;
    }
    catch (const char *error) {
        std::cout << error << std::endl;
    }

    return 0;
}