#pragma once
#include <iostream>

//#define dbg(...) [](const auto&...x){ char c='='; cerr<<#__VA_ARGS__<<" "; ((cerr<<exchange(c,',')<<" "<<x),...); cerr<<endl; }(__VA_ARGS__);
//兼容问题，没有使用c++17的 fold expression,使用了c++11递归 variadic template
template<char Delimiter = ' ',typename Args>
void print(std::ostream &os, Args&& args){ os << args << std::endl; }

template<char Delimiter = ' ',typename T,typename... Args>
void print(std::ostream &os,T t,Args&&... args){
    os << t << Delimiter;
    print<Delimiter>(os,args...);
}
#define dbg(...) std::cerr << "Line: " << __LINE__ << ": " #__VA_ARGS__ " = "; print<','>(std::cerr,__VA_ARGS__)
