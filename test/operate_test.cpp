#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../src/operate.h"

namespace fs = std::filesystem;

void create_file(const fs::path& path, const std::string& content) {
    std::ofstream file(path);
    file << content;
    file.close();
}

void list_contents(const fs::path& dir) {
    std::cout << "In "<< dir << " :" << std::endl;
    std::for_each(fs::directory_iterator(dir), fs::directory_iterator(), [](const fs::directory_entry& d) {
        std::cout << '\t' << d.path() << std::endl;
    });
}

void list_contents_recursive(const fs::path& dir) {
    std::cout << "In "<< dir << " :" << std::endl;
    std::for_each(fs::recursive_directory_iterator(dir), fs::recursive_directory_iterator(), [](const fs::directory_entry& d) {
        std::cout << '\t' << d.path() << std::endl;
    });
}

bool check_file_contents(const fs::path& file, const std::string& against) {
    bool checks = false;
    std::ifstream file_s (file);

    if (file_s.is_open()) {
        std::string line;

        std::istream& rest = std::getline(file_s, line);
        std::cout << line << std::endl;

        std::cout << "Original content: " << against << std::endl;
        std::cout << "New content: " << line << std::endl;

        if(line == against && rest.eof()) checks = true; 
        else {
            std::cout << "Rest of the file (contents did not match):" << std::endl;

            while(std::getline(file_s, line)) {
                std::cout << "\t" << line << std::endl;
            }
        }

        file_s.close();
    }else {
        std::cout << "Test file could not be opened." << std::endl;
    }

    return checks;
}

bool test_find() {
    auto cwd = fs::current_path();
    fs::path searched_file = cwd/"doc/log.txt";

    std::cout << "TESTING FIND" << std::endl;

    list_contents_recursive(cwd);

    std::cout << "Trying to find " << searched_file << std::endl;

    auto files_found = mfo::operate::find_if(cwd, [searched_file](const fs::directory_entry& d) {
        return fs::equivalent(d.path(), searched_file);
    });

    std::cout << "Found :" << std::endl;
    for(auto&& f: files_found) {
        std::cout << '\t' << f.path() << std::endl;
    }

    return files_found.size() == 1;
}

bool test_find_local() {
    auto cwd = fs::current_path(); 
    fs::path searched_file = cwd/".gitignore";

    std::cout << "TESTING FIND_LOCAL" << std::endl;

    list_contents(cwd);

    std::cout << "Trying to find " << searched_file << std::endl;

    auto files_found = mfo::operate::find_if_local(cwd, [searched_file] (const fs::directory_entry& d) {
        return fs::equivalent(d.path(), searched_file);
    });

    std::cout << "Found:" << std::endl;
    for(auto& d:files_found) {
        std::cout << '\t' << d.path() << std::endl;
    }

    return files_found.size() == 1;
}

bool test_remove() {
    auto tmp = fs::temp_directory_path();
    fs::create_directories(tmp/"dir_one/dir_two/dir_three");

    std::cout << "TESTING REMOVE" << std::endl;

    list_contents(tmp);
    list_contents_recursive(tmp/"dir_one");

    std::cout << "Removing dir_one." << std::endl;

    mfo::operate::remove(tmp/"dir_one");

    list_contents(tmp);

    auto dirs_found = mfo::operate::find_if_local(tmp, [tmp](const fs::directory_entry&d) { return fs::equivalent(d.path(), tmp/"dir_one"); });

    return dirs_found.empty();
}

bool test_move() {
    bool success = false;
    auto tmp = fs::temp_directory_path();

    std::cout << "TESTING MOVE" << std::endl;

    std::string test_file_name = "test.txt";
    std::string test_file_moved_name = "test_moved.txt";
    std::string test_file_content = "Content of test file.";

    create_file(tmp/test_file_name, test_file_content);

    std::cout << "Moving from " << tmp/test_file_name << " to " << tmp/test_file_moved_name << std::endl;
    
    list_contents(tmp);

    mfo::operate::move(tmp/test_file_name, tmp/test_file_moved_name);

    list_contents(tmp);

    success = check_file_contents(tmp/test_file_moved_name, test_file_content);

    mfo::operate::remove(tmp/test_file_moved_name);
    
    success &= mfo::operate::find_if_local(tmp, [tmp, test_file_moved_name](const fs::directory_entry& d) {
        return d.path() == tmp/test_file_moved_name;
    }).empty();

    return success;
}


bool test_copy() {
    bool success = false;
    auto tmp = fs::temp_directory_path();

    std::cout << "TESTING COPY" << std::endl;

    std::string test_file_name = "test.txt";
    std::string test_file_copy_name = "test_copy.txt";
    std::string test_file_content = "Content of test file.";

    create_file(tmp/test_file_name, test_file_content);

    std::cout << "Copying from " + test_file_name + " to " + test_file_copy_name << std::endl;

    list_contents(tmp);

    mfo::operate::copy(tmp/test_file_name, tmp/test_file_copy_name);

    list_contents(tmp);

    success = check_file_contents(tmp/test_file_copy_name, test_file_content);

    mfo::operate::remove(tmp/test_file_name);
    mfo::operate::remove(tmp/test_file_copy_name);

    return success;
}

int main()
{
    try
    { 
        bool find_local_t = test_find_local();
        std::cout << std::endl;
        
        bool find_t = test_find();
        std::cout << std::endl;

        bool remove_t = test_remove();
        std::cout << std::endl;

        bool move_t = test_move();
        std::cout << std::endl;

        bool copy_t = test_copy();
        std::cout << std::endl;

        std::cout << std::endl << std::endl;
        std::cout << "Test remove succeeded: " << std::boolalpha << remove_t << std::noboolalpha << std::endl;
        std::cout << "Test find_local succeeded: " << std::boolalpha << find_local_t << std::noboolalpha << std::endl;
        std::cout << "Test find succeeded: " << std::boolalpha << find_t << std::noboolalpha << std::endl;
        std::cout << "Test move succeeded: " << std::boolalpha << move_t << std::noboolalpha << std::endl;
        std::cout << "Test copy succeeded: " << std::boolalpha << copy_t << std::noboolalpha << std::endl;
    }
    catch(const fs::filesystem_error& e)
    {
        std::cout << e.what() << std::endl;
    }
    
   
}

