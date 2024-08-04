#include "hello.h"


Hello::Hello() : _message("Default") {}

Hello::Hello(const char* message) : _message(message) {}

void Hello::sayHello() {
    std::cout << _message << std::endl;
}

void Hello::hellocall(int val, std::function<void(int,int)>callback)
{
    callback(val, val*2);
}
