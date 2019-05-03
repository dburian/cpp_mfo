#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "../include/controller.h"
#include "../src/even_optimalizer.h"

namespace fs = std::filesystem;

template<class Optimalizer>
void test_copy(const std::vector<mfo::copy_arg>& args, mfo::controller<Optimalizer>& c) {
    std::cout << "Trying to copy with following arguments:" << std::endl;

    std::size_t i = 0;
    for(auto&& a: args) {
        std::cout << i << ")" << std::endl;
        std::cout << "\tfrom: " << a.from << std::endl;
        std::cout << "\tto: " << a.to << std::endl;

        ++i;
    }

    auto&& res = c.copy(args, 5);

    std::cout << "Results: " << std::endl;
    i = 0;
    for(auto&& r: res) {
        std::cout << i << ")" << std::endl;
        std::cout << "\tfrom: " << r.peek_operation_arguments().from << std::endl;
        std::cout << "\tto: " << r.peek_operation_arguments().to << std::endl;

        try {
            std::cout << "\tresult: " << r.get() << std::endl;
        }catch(fs::filesystem_error& err) {
            std::cout << "\tresult: " << err.what() << std::endl;
        }

        ++i;
    }

    std::cout << std::endl;
}


template<class Optimalizer>
void test_remove(const std::vector<mfo::remove_arg>& args, mfo::controller<Optimalizer>& c) {
    std::cout << "Trying to remove with following arguments:" << std::endl;

    std::size_t i = 1;
    for(auto&& a: args) {
        std::cout << i << ")" << std::endl;
        std::cout << "\ttarget; " << a.target << std::endl;
        ++i;
    }

    auto&& res = c.remove(args, 5);

    std::cout << "Results: " << std::endl;
    i = 0;
    for(auto&& r: res) {
        std::cout << i << ")" << std::endl;
        std::cout << "\ttarget: " << r.peek_operation_arguments().target << std::endl;
        try {
            std::cout << "\tresult: " << r.get() << std::endl;
        } catch(const fs::filesystem_error& er) {
            std::cout << "\tresult: " << er.what() << std::endl;
        }
        
        ++i;
    }

    std::cout << std::endl;
}

template<class Optimalizer>
void test_move(const std::vector<mfo::move_arg>& args, mfo::controller<Optimalizer>& c) {
    std::cout << "Trying to move with following arguments:" << std::endl;

    std::size_t i = 0;
    for(auto&& a: args) {
        std::cout << i << ")" << std::endl;
        std::cout << "\tfrom: " << a.from << std::endl;
        std::cout << "\tto: " << a.to << std::endl;

        ++i;
    }

    auto&& res = c.move(args, 5);

    std::cout << "Results: " << std::endl;
    i = 0;
    for(auto&&r : res) {
        std::cout << i << ")" << std::endl;
        std::cout << "\tfrom: " << r.peek_operation_arguments().from << std::endl;
        std::cout << "\tto: " << r.peek_operation_arguments().to << std::endl;
        try {
            std::cout << "\tresult: " << r.get() << std::endl;
        }catch(const fs::filesystem_error& err) {
            std::cout << "\tresult: " << err.what() << std::endl;
        }

        ++i;
    }

    std::cout << std::endl;
}

template<class Optimalizer, class UnaryPredicate>
void test_find(const std::vector<mfo::find_arg<UnaryPredicate>>& args, mfo::controller<Optimalizer>& c) {
    std::cout << "Trying to execute find with following arguments: " << std::endl;

    std::size_t i = 0;
    for(auto&& a: args) {
        std::cout << i << ")" << std::endl;
        std::cout << "\tin directory: " << a.in_dir << std::endl;

        ++i;
    }

    auto&& res = c.find(args, 5);

    std::cout << "Results: " << std::endl;
    i = 0;
    for(auto&& r: res) {
        std::cout << i << ")" << std::endl;
        std::cout << "\tin_directory: " << r.peek_operation_arguments().in_dir << std::endl;
        std::cout << "\tfound: " << std::endl;
        try {
            auto&& found = r.get();

            for(auto&& dirEntry: found)
                std::cout << dirEntry.path() << std::endl;
        }catch(const fs::filesystem_error& err) {
            std::cout << err.what() << std::endl;
        }

        ++i;
    }

    std::cout << std::endl;
}

template<class Optimalizer, class UnaryPredicate>
void test_find_recursive(const std::vector<mfo::find_recursive_arg<UnaryPredicate>>& args, mfo::controller<Optimalizer>& c) {
    std::cout << "Trying to execute recursive find with following arguments:" << std::endl;

    std::size_t i = 0;
    for(auto&& a: args) {
        std::cout << i << ")" << std::endl;
        std::cout << "\tin directory: " << a.in_dir << std::endl;

        ++i;
    }

    auto&& res = c.find_recursive(args, 5);

    std::cout << "Results: " << std::endl;
    i = 0;
    for(auto&& r: res) {
        std::cout << i << ")" << std::endl;
        std::cout << "\tin_directory: " << r.peek_operation_arguments().in_dir << std::endl;
        std::cout << "\tfound: " << std::endl;
        try {
            auto&& found = r.get();

            for(auto&& dirEntry: found)
                std::cout << dirEntry.path() << std::endl;
        }catch(const fs::filesystem_error& err) {
            std::cout << err.what() << std::endl;
        }

        ++i;
    }

    std::cout << std::endl;    
}

int main(int argc, char**argv) {
    try {
        mfo::controller<mfo::even_optimalizer> c;
    
        fs::path existentFile("/tmp/test.txt"); // must exist
        fs::path copiedFile("/tmp/test_copy.txt");
        fs::path copiedFile2("/tmp/test_copy2.txt");

        std::vector<mfo::copy_arg> args_c{mfo::copy_arg(existentFile, copiedFile), mfo::copy_arg(existentFile, copiedFile2)};
        test_copy(args_c, c);

        std::vector<mfo::remove_arg> args_r{mfo::remove_arg(copiedFile), mfo::remove_arg(copiedFile2)};
        test_remove(args_r, c);

        fs::path movedFile("/tmp/test_moved.txt");
        fs::path movedFile2("/tmp/test_moved2.txt");

        std::vector<mfo::move_arg> args_m{mfo::move_arg(existentFile, movedFile), mfo::move_arg(existentFile, movedFile2)};         //one of the move must fail
        test_move(args_m, c);

        std::vector<mfo::move_arg> args_m2{mfo::move_arg(movedFile, existentFile), mfo::move_arg(movedFile2, existentFile)};        //consequently one these moves must fail too
        test_move(args_m2, c);

        std::function<bool(const fs::directory_entry&)> alwaysTrue = [](const fs::directory_entry& d) { return true; };
        std::function<bool(const fs::directory_entry&)> hiddenDirs = [](const fs::directory_entry& d) { return d.is_directory() && d.path().filename().string()[0] == '.'; };

        std::vector<mfo::find_arg<decltype(alwaysTrue)>> args_f{mfo::find_arg(fs::temp_directory_path(), &alwaysTrue), mfo::find_arg(fs::path("/home/david"), &hiddenDirs)};
        test_find(args_f, c);
        
        std::function<bool(const fs::directory_entry&)> bashScripts = [] (const fs::directory_entry& d) { return d.path().extension() == ".sh"; };
        std::function<bool(const fs::directory_entry&)> zshScripts = [] (const fs::directory_entry& d) { return d.path().extension() == ".zsh"; };

        std::vector<mfo::find_recursive_arg<decltype(bashScripts)>> args_f_r{mfo::find_recursive_arg(fs::path("/home/david"), &bashScripts), mfo::find_recursive_arg(fs::path("/home/david"), &zshScripts)};
        test_find_recursive(args_f_r, c);

    }catch(const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }catch(...) {
        std::cout << "Uncaught exception of undefined type." << std::endl;
        int x;
        std::cin >> x;
    }
}