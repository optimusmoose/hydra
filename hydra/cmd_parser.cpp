#include "cmd_parser.hpp"

// CMDParser function implementations
CMDParser::CMDParser(){};

void CMDParser::parse(const int argc, const char *argv[])
{

    if (argc == 2) // check for help request
    {
        std::string s = argv[1];
        if (s == "h" || s == "help")
        {
            help();
            exit(0);
        }
    }

    if (argc % 2 == 0) // Guard clause for incorrect number of args
    {
        puts("Invalid number of user defined arguments");
        exit(0);
    }

    for (int i = 1; i < argc; ++i)
    {
        std::string s = (argv[i]);
        if (s == "mz_distance")
        {
            try
            {
                mz_distance = std::stof(argv[++i]);
                
                if (mz_distance <= 0.0)
                {
                    puts("Invalid mz_distance parameter.");
                    exit(0);
                }
            }
            catch(const std::invalid_argument &e)
            {
                puts("Invalid mz_distance parameter.");
                exit(0);
            }
        }
        else if (s == "rt_distance")
        {
            try 
            {
                rt_distance = std::stof(argv[++i]);

                if (rt_distance <= 0.0)
                {
                    puts("Invalid rt_distance parameter.");
                    exit(0);
                }
            }
            catch(const std::invalid_argument &e)
            {
                puts("Invalid rt_distance parameter.");
                exit(0);
            }
        }
        else if (s == "cs_enforced")
        {
            try 
            {
                cs_enforced = std::stoi(argv[++i]);

                if (cs_enforced != 0 && cs_enforced != 1)
                {
                    puts("Invalid cs_enforced parameter. Options = [0,1]");
                    exit(0);
                }
            }
            catch (const std::invalid_argument &e)
            {
                puts("Invalid cs_enforced parameter. Options = [0,1]");
                exit(0);
            }
        }
        else if (s == "file")
        {
            files.emplace_back(argv[++i]);
        }
        else if (s == "outfile")
        {
            outfile = argv[++i];
        }
        else 
        {
            // Unrecognized user input
            std::cout << "Unrecognized user input" << "\n\n";

            // Display help
            help();
            exit(0);
        }
    }
};
void CMDParser::print()
{
    // Print out commands
    std::cout << "mz_distance: " << mz_distance
    << " rt_distance: " << rt_distance
    << " ir_distance: " << ir_distance
    << "\n";
};

void CMDParser::help()
{
    // Hydra help, explanations
    std::cout 
    << "Hydra" << "\n"
    << "-------------" << "\n"
    << "\n"
    << "A fast MS1 direct matching algorithm for run to run feature correspondece" << "\n"
    << "\n"
    << "Version: .9" << "\n"
    << "\n"
    << "Parameters:" << "\n"
    << "\n"
    << "\tmz_distance <float> in Da, on the interval (0,infinity)" << "\n"
    << "\t\tDefault: " << mz_distance << "\n"
    << "\t\tExample: mz_distance 1" << "\n"
    << "\t\tMax distance in m/z to consider two features" << "\n"
    << "\n"
    << "\trt_distance <float>, on the interval (0,infinity)" << "\n"
    << "\t\tDefault: " << rt_distance << "\n"
    << "\t\tExample: rt_distance 25" << "\n"
    << "\t\tMax distance in RT to consider two features, should be set greater than the RT deviation from any file to another"  << "\n"
    << "\n"
    << "\tcs_enforced <int> [0,1]" << "\n"
    << "\t\t0 -> charge state equality not enforced" << "\n"
    << "\t\t1 -> charge state equality enforced" << "\n"
    << "\t\tDefault: 1" << "\n"
    << "\t\tExample: cs_enforced 1" << "\n"
    << "\n"
    << "\toutfile <string>, outfile for Hydra output. .consensusXML will be appended." << "\n"
    << "\t\tDefault: none" << "\n"
    << "\t\tExample: outfile ~/path/to/directory/outfile" << "\n"
    << "\n"
    << "\tfile <string>, specifies a JSON file for processing, more than one file required." << "\n"
    << "\t\tDefault: none" << "\n"
    << "\t\tExample: file file1.json file file2.json" << "\n"
    << "\n"
    << "\nExecution examples:" << "\n"
    << "\n"
    << "\t./Hydra outfile ~/path/outfile file ~/path/A/file.json file ~/path/B/file.json mz_distance .1 rt_distance 25 cs_enforced 1" << "\n"
    << "\n"
    ;
}
// End CMDParser function implementations
