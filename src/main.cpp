#include "ilex.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Please give the file path" << endl;
        return -1;
    }

    string filePath = argv[1];

    ifstream inFile;
    inFile.open(filePath);

    ofstream outFile;
    outFile.open("./ilex.yy.cpp");

    if (!inFile.is_open()) {
        cout << "File could not be opened. Please try again." << endl;
        return -1;
    }

    string curLine;
    int curState = 0;

    int (*funArr[3])(ifstream&, ofstream&) = {&definitionsScanner, &rulesScanner, &subroutinesScanner};

    for (int i = 0; i < 3; i++) {
        switch (funArr(inFile, outFile)) {

        }
    }

    inFile.close();
    outFile.close();

    return 0;
}

int definitionsScanner(ifstream& inFile, ofstream& outFile) {

}

int rulesScanner(ifstream& inFile, ofstream& outFile) {

}

int subroutinesScanner(ifstream& inFile, ofstream& outFile) {

}