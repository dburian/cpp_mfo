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
    std::ifstream fileS (file);

    if (fileS.is_open()) {
        std::string line;

        std::istream& rest = std::getline(fileS, line);
        std::cout << line << std::endl;

        std::cout << "Original content: " << against << std::endl;
        std::cout << "New content: " << line << std::endl;

        if(line == against && rest.eof()) checks = true; 
        else {
            std::cout << "Rest of the file (contents did not match):" << std::endl;

            while(std::getline(fileS, line)) {
                std::cout << "\t" << line << std::endl;
            }
        }

        fileS.close();
    }else {
        std::cout << "Test file could not be opened." << std::endl;
    }

    return checks;
}

bool test_find_recursive() {
    auto cwd = fs::current_path();
    fs::path searchedFile = cwd/"doc/log.txt";

    std::cout << "TESTING FIND" << std::endl;

    list_contents_recursive(cwd);

    std::cout << "Trying to find " << searchedFile << std::endl;

    auto filesFound = mfo::operate::find_recursive(cwd, [searchedFile](const fs::directory_entry& d) {
        return fs::equivalent(d.path(), searchedFile);
    });

    std::cout << "Found :" << std::endl;
    for(auto&& f: filesFound) {
        std::cout << '\t' << f.path() << std::endl;
    }

    return filesFound.size() == 1;
}

bool test_find() {
    auto cwd = fs::current_path(); 
    fs::path searchedFile = cwd/".gitignore";

    std::cout << "TESTING FIND_LOCAL" << std::endl;

    list_contents(cwd);

    std::cout << "Trying to find " << searchedFile << std::endl;

    auto filesFound = mfo::operate::find(cwd, [searchedFile] (const fs::directory_entry& d) {
        return fs::equivalent(d.path(), searchedFile);
    });

    std::cout << "Found:" << std::endl;
    for(auto& d:filesFound) {
        std::cout << '\t' << d.path() << std::endl;
    }

    return filesFound.size() == 1;
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

    auto dirsFound = mfo::operate::find(tmp, [tmp](const fs::directory_entry&d) { return fs::equivalent(d.path(), tmp/"dir_one"); });

    return dirsFound.empty();
}

bool test_move() {
    bool success = false;
    auto tmp = fs::temp_directory_path();

    std::cout << "TESTING MOVE" << std::endl;

    std::string testFileName = "test.txt";
    std::string testFileMovedName = "test_moved.txt";
    std::string testFileContent = "Content of test file.";

    create_file(tmp/testFileName, testFileContent);

    std::cout << "Moving from " << tmp/testFileName << " to " << tmp/testFileMovedName << std::endl;
    
    list_contents(tmp);

    mfo::operate::move(tmp/testFileName, tmp/testFileMovedName);

    list_contents(tmp);

    success = check_file_contents(tmp/testFileMovedName, testFileContent);

    mfo::operate::remove(tmp/testFileMovedName);
    
    success &= mfo::operate::find(tmp, [tmp, testFileMovedName](const fs::directory_entry& d) {
        return d.path() == tmp/testFileMovedName;
    }).empty();

    return success;
}


bool test_copy() {
    bool success = false;
    auto tmp = fs::temp_directory_path();

    std::cout << "TESTING COPY" << std::endl;

    std::string testFileName = "test.txt";
    std::string testFileCopyName = "test_copy.txt";
    std::string testFileContent = "Content of test file.";

    create_file(tmp/testFileName, testFileContent);

    std::cout << "Copying from " + testFileName + " to " + testFileCopyName << std::endl;

    list_contents(tmp);

    mfo::operate::copy(tmp/testFileName, tmp/testFileCopyName);

    list_contents(tmp);

    success = check_file_contents(tmp/testFileCopyName, testFileContent);

    mfo::operate::remove(tmp/testFileName);
    mfo::operate::remove(tmp/testFileCopyName);

    return success;
}

int main()
{
    try
    { 
        bool findT = test_find();
        std::cout << std::endl;
        
        bool findRecursiveT = test_find_recursive();
        std::cout << std::endl;

        bool removeT = test_remove();
        std::cout << std::endl;

        bool moveT = test_move();
        std::cout << std::endl;

        bool copyT = test_copy();
        std::cout << std::endl;

        std::cout << std::endl << std::endl;
        std::cout << "Test find_local succeeded: " << std::boolalpha << findT << std::noboolalpha << std::endl;
        std::cout << "Test find succeeded: " << std::boolalpha << findRecursiveT << std::noboolalpha << std::endl;
        std::cout << "Test remove succeeded: " << std::boolalpha << removeT << std::noboolalpha << std::endl;
        std::cout << "Test moveT succeeded: " << std::boolalpha << moveT << std::noboolalpha << std::endl;
        std::cout << "Test copy succeeded: " << std::boolalpha << copyT << std::noboolalpha << std::endl;
    }
    catch(const fs::filesystem_error& e)
    {
        std::cout << e.what() << std::endl;
    }
    
   
}

