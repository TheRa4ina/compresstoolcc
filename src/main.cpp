#include <iostream>
#include <fstream>
#include <format>
#include <chrono>
#include <src/compressor.h>
#include <src/decompressor.h>

int main(){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    Compressor c;
    Decompressor d;
    std::string filename="bigfile";
    try {
        std::ifstream in(std::format("tests/{}.txt",filename),std::ios::in | std::ios::binary);
        std::ofstream out(std::format("tests/{}.huffman", filename),std::ios::out | std::ios_base::binary);
        auto c1 = high_resolution_clock::now();
        c.compress(in, out);
        out.flush();
        auto c2 = high_resolution_clock::now();
        auto c_ms_int = duration_cast<milliseconds>(c2 - c1);
        std::cout <<"Time to compress:" << c_ms_int.count() << "ms\n";
        in.close();
        out.close();
        auto d1 = high_resolution_clock::now();
        in.open(std::format("tests/{}.huffman", filename), std::ios::in | std::ios::binary);
        out.open(std::format("tests/{}.dec.txt", filename), std::ios::out | std::ios_base::binary);
        d.decompress(in, out);
        auto d2 = high_resolution_clock::now();
        auto d_ms_int = duration_cast<milliseconds>(d2 - d1);
        std::cout << "Time to decompress:" << d_ms_int.count() << "ms\n";
        out.flush();
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    return 0;
}