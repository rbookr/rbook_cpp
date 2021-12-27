// book test
#include <iostream>
//#include "./third-party/yaml-cpp/include/yaml-cpp/yaml.h"
#include "include/book.hpp"

using namespace RBOOK;
int main(){
    std::cout << "helloworld" ;
    GIT::getIns().setBasePath("/home/rainboy/tmp");
    Book a("https://github.com/Rainboylvx/bookData.git");
    a.update();
    std::cout << "s" << std::endl;
    std::cout << a.get_name() ;
    return 0;
}


