#ifndef cmd_parser_hpp
#define cmd_parser_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

class CMDParser{

public:
    float mz_distance = 1.0;
    float rt_distance = 25.0;
    float ir_distance = 90.0;
    std::string outfile;
    bool validate = false;
    int method = 1;
    bool cs_enforced = true;
    std::vector<std::string> files;
    
    CMDParser();

    void parse(const int argc, const char * argv[]);
    void print();
    void help();
};

#endif /* cmd_parser_hpp */
