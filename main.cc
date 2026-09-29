/**
 * This program serves as a dummy executable to link against the shared library
 * for testing.
 * */


#include "json.h"
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: cjson <path>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string json_source = buffer.str();


    try {
        Json json;
        
        // auto value = json.at("name");
        //
        // std::cout << value.get<std::string>() << "\n";

        std::cout << json.dumps() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

}
