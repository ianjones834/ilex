#include "tests.h"
#include "utils.h"
#include <filesystem>

int main() {
    string path = "../tests/ilex-tests";

    for (const auto & entry : filesystem::directory_iterator(path)) {
        cout << "Current File: " << entry.path().string() << endl << endl;


        system(("./ilex " + entry.path().string()).c_str());
        system("g++ --std=c++20 ilex.yy.cpp -o ilex.yy");
        system("./ilex.yy");
    }
}