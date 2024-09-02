#include <iostream>
#include <fstream>
#include <src/compressor.h>

int main(){
    Compressor c;
    try {
        std::ifstream in("tests/5chars.txt",std::ios::in | std::ios::binary);
        std::ofstream out("tests/5chars.huffman",std::ios::out | std::ios_base::binary);
        c.compress(in, out);
        out.flush();
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    return 0;
}