#pragma once
#include "iostream"
#include "web.h"
#include <functional>

class Hello {
public:
    Hello();
    Hello(const char* message);
    void sayHello();
    void hellocall(int val, std::function<void(int,int)>callback);
private:
    const char* _message=nullptr;
    web _web;
};