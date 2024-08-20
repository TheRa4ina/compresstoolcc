#include <iostream>
#include <src/hello.hpp>
#include "hello.hpp"

Hello::Hello()
{
    std::cout<<"Hello"<<std::endl;
}

Hello::~Hello()
{
    std::cout<<"bahbay"<<std::endl;
}
int Hello::helloStuff(int arg)
{
    return arg;
}

