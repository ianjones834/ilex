#include "tests.h"
#include "utils.h"
#include <filesystem>

int main() {
    string path = "../tests/ilex-tests";

    for (const auto & entry : filesystem::directory_iterator(path)) {
        cout << "Current File: " << entry.path().string() << " ";

        system(("./ilex " + entry.path().string()).c_str());

        cout << "Compile Time: ";
        cout.flush();
        auto start  = high_resolution_clock::now();
        system("g++ --std=c++20 ilex.yy.cpp -o ilex.yy");
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(end - start);
        cout << duration << endl << endl;

        system("./ilex.yy");
    }
}