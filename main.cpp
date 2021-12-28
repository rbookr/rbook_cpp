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
    std::cerr << "s" << std::endl;
    std::cerr << a.get_name() << std::endl;
    auto x = a.search("944c9cd39b");
    if( x != nullptr)
    std::cerr << x->title << std::endl;
    std::cerr << a.m_Btree->root->n  << std::endl ;
    std::cerr << a.m_Btree->root->keys[0].id  << std::endl ;
    std::cerr << a.m_Btree->root->keys[0].title  << std::endl ;

    return 0;
}


