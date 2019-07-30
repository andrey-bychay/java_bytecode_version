#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::right;
using std::hex;
using std::dec;
using std::setw;
using std::setfill;

void printHexByte(char ch) {
    unsigned int byte = static_cast<unsigned int>(ch) & 0xFF;
    cout << hex << setw(2) << setfill('0') << byte << " ";
}

template <typename T>
void fixByteOrderToNative(T & value) {
    // assuming native is little-endian
    char * begin = reinterpret_cast<char *>(&value);
    char * end = begin + sizeof(value) - 1;
    while (begin < end) {
        std::swap(*begin++, *end--);
    }
}

struct JavaClassHeader {
  unsigned int magic;   // 4 bytes
  unsigned short minor; // 2 bytes
  unsigned short major; // 2 bytes
};

void printJdkVersion(JavaClassHeader & header) {
    // see https://en.wikipedia.org/wiki/Java_class_file
    if (header.magic != 0xCAFEBABE) {
        cout << "binary file";
        return;
    }

    cout << "jdk v" << dec << setw(1);
    if (header.major >= 45 && header.major <= 48) {
        cout << "1." << (header.major - 45 + 1);
    } else {
        cout << (header.major - 49 + 5);
    }
}

void printJavaClassHeader(const std::string & fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        cerr << "failed to open the file" << endl;
        return;
    }

    JavaClassHeader header;
    if (!file.read(reinterpret_cast<char *>(&header), sizeof(header))) {
        cerr << "failed to read class header" << endl;
        return;
    }

    cout << " [ ";
    std::for_each(reinterpret_cast<char *>(&header), reinterpret_cast<char *>(&header) + sizeof(header), printHexByte);
    cout << "] ";

    fixByteOrderToNative(header.magic);
    fixByteOrderToNative(header.minor);
    fixByteOrderToNative(header.major);

    printJdkVersion(header);
}

int main (int argc, char* argv[]) {
    if (argc < 2) {
        cout << "usage: " << argv[0] << " [java-class-file] [java-class-file] ..." << endl;
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        char const * file = argv[i];
        cout << "* " << setw(32) << setfill(' ') << right << file << ":";
        printJavaClassHeader(file);
        cout << endl;
    }
    return 0;
}

