/**
 * rbook 主类
 *  用来管理各种东西
 */

#pragma 
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <shared_mutex>

#include "base.hpp"
#include "config.hpp"
#include "book.hpp"

namespace RBOOK {

using BOOK_SHP = std::shared_ptr<Book>;

class RBook {

    RBook(std::string_view config_path)
        :m_config{config_path}
    {}

    void update(); //更新

    //根据仓库的名字和id 得到一个文章
    void getOneAricle(std::string_view bookName,std::string_view id);

private:
    Config m_config;    //配置
    std::unordered_map<std::string,BOOK_SHP> m_Books;
	mutable std::shared_mutex mutex_;
};


void RBook::update(){
    std::unique_lock<std::shared_mutex> lock(mutex_);
    //保证只有一个线程能运行更新
    m_config.update(); //1. 更新配置
}
//根据仓库的名字和id 得到一个文章
void RBook::getOneAricle(std::string_view bookName,std::string_view id){
    std::shared_lock<std::shared_mutex> lock(mutex_);

}

} //namespace RBOOK
