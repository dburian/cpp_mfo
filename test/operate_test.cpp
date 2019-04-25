#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../src/operate.h"

namespace fs = std::filesystem;

bool test_copy() {
    bool success = false;
    auto tmp = fs::temp_directory_path();

    std::string test_file_name = "test.txt";
    std::string test_file_copy_name = "test_copy.txt";
    std::string test_file_content = "Content of test file.";

    std::ofstream test_file (tmp/test_file_name);
    test_file << test_file_content << std::endl;
    test_file.close();

    std::cout << "Copying from " + test_file_name + " to " + test_file_copy_name << std::endl;

    std::cout << "In tmp:" << std::endl;
    std::for_each(fs::directory_iterator(tmp), fs::directory_iterator(), [](const fs::directory_entry& d) {
        std::cout << "\t" + d.path().string() << std::endl;
    });


    mfo::operate::copy(tmp/test_file_name, tmp/test_file_copy_name);

    std::ifstream copied_file (tmp/test_file_copy_name);

    if (copied_file.is_open()) {
        std::string line;

        std::istream& rest = std::getline(copied_file, line);
        std::cout << line << std::endl;

        if(line == test_file_content && rest.eof()) success = true; 
        
        copied_file.close();
    }else {
        std::cout << "Test file could not be opened." << std::endl;
    }

    return success;
}

int main()
{
    throw fs::filesystem_error("adsf", std::error_code{});
    bool copy_t = test_copy();
    std::cout << "Test copy succeeded: " + copy_t << std::endl;
}

