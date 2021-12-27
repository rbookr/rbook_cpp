//封装 对git 的调用
#pragma once

#include "base.hpp"

#include "utils.hpp"
#include "subprocess.hpp"

namespace RBOOK {
    
class GIT {
public:

    static GIT& getIns() {
        static GIT git;
        return git;
    }

    bool update(std::string_view url,std::string_view name);

    std::string getLastId(std::string& name){
        //std::cout << "path " ;
        //std::cout << path  << std::endl;
        std::vector<std::string> args{"-C",(base_path/name).string(),"rev-parse","HEAD"};
        subprocess::popen git_getid("/usr/bin/git",args);
        std::ostringstream ss; 
        ss << git_getid.stdout().rdbuf();
        return ss.str();
    }

    void setBasePath(fs::path path){ base_path = path; }
    const auto& getBasePath() const {return base_path;}


private:
    GIT(){};
    fs::path base_path; //基础路径
};

bool GIT::update(std::string_view url,std::string_view name){
    std::cout << "GIT::Update" << std::endl ;
    std::string res;
    try {
        //1. 是否存在
        auto local = base_path/name;
        if( ! fs::exists(local)) { //不存在 clone
            std::vector<std::string> args{"clone",std::string(url),local.string()};
            //std::string cmd = "git clone ";
            //cmd += url;
            //cmd += " ";
            //cmd += local;
            //res = exec(cmd.c_str());
            //
            subprocess::popen git_clone("/usr/bin/git",args);
            git_clone.wait();
            std::cout << git_clone.stdout().rdbuf();
            std::cout << git_clone.stderr().rdbuf();
        }
        else { // pull
            std::vector<std::string> args{"-C",local.string(),"pull origin master"};
            subprocess::popen git_pull("/usr/bin/git",args);
            //res = exec(cmd.c_str());
            git_pull.wait();
            std::cout << git_pull.stdout().rdbuf();
            std::cout << git_pull.stderr().rdbuf();
        }
        std::cout << "git:update close" << std::endl;
    }
    catch(std::exception & e){
        std::cerr << " Exception : " << e.what() << "\n"; //TODO log
        return false;
    }
    catch(...){
        std::cerr << " error: " << res << "\n";//TODO log
        return false;
    }
    return true;
}

} // end namespace RBOOK
