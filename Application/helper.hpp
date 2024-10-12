#pragma once
#include <fstream>

class helper {
public:
    static void get_png_image_dimensions(std::string& file_path, unsigned int& width, unsigned int& height)
    {
        unsigned char buf[8];

        std::ifstream in(file_path);
        in.seekg(16);
        in.read(reinterpret_cast<char*>(&buf), 8);

        width = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3] << 0);
        height = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + (buf[7] << 0);
    }
};





































