// STL
#include <iostream>
#include <chrono>

// Custom
#include "cmd_parser.hpp"
#include "grid.hpp"
#include "correspondence.hpp"
#include "json.hpp"

using namespace std::chrono;

int main(int argc, const char * argv[]) {

    // Instantiate commands parser
    CMDParser cmds = CMDParser();
    
    auto start = high_resolution_clock::now();
    // Read in user-defined params
    cmds.parse(argc, argv);

    // Perform Correspondence
    Correspondence c = Correspondence(cmds);
    c.perform();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "Runtime: " << duration.count() / 1000.0 << " seconds\n";
    
    return 0;
}
