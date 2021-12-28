/**
 * Book类 代码一个仓库
 */
#pragma once
#include <filesystem>
#include <vector>

#include "Btree.hpp"
#include "git.hpp"
#include "md5.h"

#include "logWrapper.hpp"

#include "yaml-cpp/include/yaml-cpp/yaml.h"

namespace RBOOK {

/**
 * md的信息
 */
class MdInfo {
public:
    std::vector<std::string> tags; // tag
    std::string catalog; //分类
    std::string author;//作者
    std::filesystem::path path; //路径
    std::string title; //标题
    std::string id; // id
    std::string create_time; //创建时间
    std::string update_time; //更新时间

    MdInfo() =default;
    MdInfo(std::string_view& id) : id{id} {}

    MdInfo(const fs::path & mdfile) 
    {
        id = MD5(mdfile.string()).toStr();
        title = mdfile.stem().string(); //文件名
    }

    MdInfo(YAML::Node &node,const fs::path & mdfile) 
    {
        //得到id
        id =  node["_id"] ? node["_id"].as<std::string>()
            : MD5(mdfile.string()).toStr();
        title = node["title"] ? node["title"].as<std::string>()
            : mdfile.stem().string(); //文件名
        
        if(node["catalog"] ) catalog = node[catalog].as<std::string>();

    }

    void operator= (const MdInfo& md){ throw "Do not use copy assign operator";}
    void operator= (MdInfo&& md){
        path        = std::move(md.path); //路径
        tags        = std::move(md.tags); // tag
        catalog     = std::move(md.catalog    ); //分类
        author      = std::move(md.author     );//作者
        title       = std::move(md.title      ); //标题
        id          = std::move(md.id         ); //id
        create_time = std::move(md.create_time); //创建时间
        update_time = std::move(md.update_time); //更新时间
    }

    MdInfo(MdInfo&& md){ *this = std::move(md);}

    bool operator <(const MdInfo & md){ return id < md.id; }
    bool operator <=(const MdInfo & md){ return id <= md.id; }
    bool operator >(const MdInfo & md){ return id > md.id; }
    bool operator ==(const MdInfo & md){ return id == md.id; }

};

/**
 * 一个书的目录,可能有多个
 */
struct subMenu {
    std::string name; //名字
    std::string menu; //内容
    std::string desc; //描述
    std::filesystem::path icon; //图标
};

class Book {
public:
    Book() = delete;
    // eg url : 
    // 1. https://github.com/Rainboylvx/bookData
    // 2. https://github.com/Rainboylvx/bookData.git
    Book(std::string_view url)
        : m_url{url}
    { 
        get_name_for_url(); 
        m_local = GIT::getIns().getBasePath() / m_name;
    }


    Book(std::string_view name,std::string_view url)
        : m_url{url},m_name{name}
    {
        m_local = GIT::getIns().getBasePath() / m_name;
    }

public:
    
    //更新仓库,在更新的时候不能查询
    //返回值: 是否更新 true 就是更新了
    bool update();

    void scan(); //扫描所有的书

    //查找对应的md
    MdInfo * search(std::string_view id) {
        MdInfo tmp(id);
        auto res = m_Btree->search(tmp);
        if( res == nullptr) {
            std::cerr << "没有找到" << std::endl;
            return nullptr;
        }
        for(int i=0;i<res->n;++i){
            if( res->keys[i] == tmp)
                return &(res->keys[i]);
        }
        return nullptr;
    }

    const std::string_view get_name() const{ return m_name; }

    //通过id 得到 一个md文章的相关信息
    const MdInfo * getMdById(std::string_view id);
    //std::string getMdByPath(std::string_view id);

    //得到yaml的头
    std::string get_yaml_header(std::fstream & md);

    MdInfo parseMdInfo(const fs::path& mdfile){
        std::fstream md(mdfile);
        std::string yaml = get_yaml_header(md);
        if( yaml.length() != 0){
            //std::cerr << yaml << std::endl;
            // 进行 ejscpp
            // 进行yaml 解析
            YAML::Node node = YAML::Load(yaml);
            //std::cerr << node << std::endl;
            // TODO parseError
            return MdInfo(node,mdfile);
        }
        return MdInfo(mdfile);
    }

    std::string get_content(fs::path mdfile){

    }


//private:
    //解析url,得到 name raw_url
    //void parse(const std::string_view & url,bool parse_name); 
    
    void get_name_for_url(){
        auto last = m_url.end();
        auto it = last-1;
        for(; it != m_url.begin(); it--){
            if( *it == '.' && last-it == 4){ //是不是git
                // 为了支持c++17 使用这种构造函数
                if(std::string_view(&*it,last-it) == ".git"){
                    last = it;
                }
            }
            if( *it == '/') break;
        }
        m_name = std::string(it+1,last);
    }
    //BTree<MdInfo,3> m_Btree; //b树
    using BtreeType = BTree<MdInfo,3>;
    std::shared_ptr<BtreeType> m_Btree{nullptr};
    std::string m_name; //名字
    std::string m_url;  //仓库地址
    std::string last_id; //最后个git log 的id
    std::filesystem::path m_local; //本地地址
    std::vector<subMenu> m_menu; //目录
};

//更新仓库
bool Book::update(){
    dbg("book update start ===");
    GIT::getIns().update(m_url,m_name);
    auto old_id = last_id;
    //std::cerr << "last_id" << last_id << std::endl ;
    last_id = GIT::getIns().getLastId(m_name);
    //std::cerr << "last_id" << last_id << std::endl ;
    if( old_id == last_id ) return false;

    //创建一个新的BTree 类型
    m_Btree = std::make_shared<BtreeType>();
    std::cerr << "filesystem " << m_local << std::endl ;
    dbg(GIT::getIns().getBasePath());
    dbg(m_name);
    dbg(m_local);
    // 递归 遍历
    scan();
    dbg("book update end ===");
    return true;
}

void Book::scan(){
    int cnt=0;
    for (fs::recursive_directory_iterator i(m_local), end; i != end; ++i) 
        if (!fs::is_directory(i->path()) && i->path().extension() == ".md") {
            //得到地址
            std::cerr << fs::relative(i->path(),m_local)<< "\n";
            auto res = parseMdInfo(i->path());
            std::cerr << res.id << "\n";
            std::cerr << res.title << "\n";
            m_Btree->insert(std::move(res));
            cnt++;
            //std::cerr << i->path().relative_path() << "\n";
            // 得到头
            // 加入到Btree 里
        }
    dbg(cnt);
}


std::string Book::get_yaml_header(std::fstream & md){
    std::string ret;
    char pre_c;
    char c = md.get();
    if( c != '-') return ret; //第一个char不是 - ,不符合规定
    while ( c =='-' ) c=md.get(); //过滤掉 起始行的 -
    if(c != '\r' && c != '\n') return ret;

    pre_c = c;
    while (c != EOF) {
        if( c == '-' && pre_c == '\n' && md.peek() == '-') break;
        ret += c;
        pre_c = c;
        c = md.get();
    }
    while ( c =='-' ) c=md.get(); //过滤掉 终点行的 -
    return ret;
}

} // end namespace RBOOK
