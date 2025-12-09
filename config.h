#ifndef config_h
#define config_h
#include <string>

// Declare a Config class
class Config
{
public:
    // Indicates whether to print debug messages
    bool debug;
    bool quiet;

    // Constructor
    Config();

    // Destructor
    virtual ~Config();

    // Parses command-line args
    void parse_flags(int argc, char** argv);
};

// Declare a global variable
extern Config config;

// Declare a function for printing debug messages
void log(const std::string&);

#endif