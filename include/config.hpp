/**
 * 对config的解析
 */
#pragma once

#include <vector>
#include <sstream>
#include <fstream>
#include <string>

#include "yaml-cpp/yaml.h"

namespace RBOOK {

struct config_node {
    config_node(std::string_view a) : url{a} {}
    config_node(std::string_view a,std::string_view b) : name{a},url{b} {}

    config_node(std::string&& a) : url{std::move(a)} {}
    config_node(std::string&& a,std::string&& b) 
        : name{std::move(a)},url{std::move(b)} {}
    std::string name;
    std::string url;
};

class Config {
public:
    Config(std::string_view path)
        :m_path{path}
    {}
    
    void update(){
        std::fstream fs(m_path);
        std::stringstream ss;
        ss << fs.rdbuf(); //读取所有的内容
        YAML::Node _yml = YAML::Load(ss.str());
        m_books.clear();
        if( _yml["books"] && _yml["books"].Type() == YAML::NodeType::Sequence){
            auto books = _yml["books"];
            for (std::size_t i=0;i<books.size();i++) {
                if( books[i].Type() == YAML::NodeType::Map ){
                  m_books.emplace_back(
                            books[i]["name"].as<std::string>(),
                            books[i]["url"].as<std::string>()
                        );
                }
                else
                  m_books.emplace_back(books[i].as<std::string>());
            }
            
        }
        fs.close();
    }
    void print_books(){
        for (const auto& e : m_books) {
            std::cout << "name " << e.name << '\n';
            std::cout << "url " << e.url << "\n\n";
        }
        std::cout << std::endl;
    }
private:
    std::string m_path; //config_path
    std::vector<config_node> m_books;
};

}
