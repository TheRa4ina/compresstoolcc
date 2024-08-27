#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <src/compressor.h>

int main(){
    Compressor c;
    try {
        std::ifstream in("tests/5chars.txt",std::ios::in | std::ios::binary);
        std::ofstream out("tests/5chars3.huffman",std::ios::out | std::ios_base::binary);
        c.compress(in, out);
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    return 0;
}