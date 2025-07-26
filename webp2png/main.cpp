#include "../src/webp.hpp"
#include <gempyre_bitmap.h>
#include <gempyre.h>
#include <gempyre_utils.h>
#include <iostream>
#include <filesystem>
#include <format>

bool write_png(const auto& name, const Gempyre::Bitmap& bmp) {
    const auto png = bmp.png_image();
    std::ofstream ofs(name.string() + ".png", std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(png.data()), png.size());
    return ofs.good();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << std::format("{} WEBP_IMAGE", std::filesystem::path(argv[0]).stem().string()) << std::endl;
        std::exit(1);
    }
    const auto source = argv[1];
    const auto bytes = GempyreUtils::slurp<uint8_t>(source);
    WebPGempyre::Bitmap bmp(bytes);
    const auto dirname = std::filesystem::path(source).stem();
    auto path = std::filesystem::current_path();
    path /= dirname;
    if (bmp.info().frames > 1) {
        std::filesystem::create_directory(path);
        unsigned index = 1;
        for(const auto& frame : bmp) {
            const auto name = path / std::format("{}_{}", dirname.string(), index);
            write_png(name, std::get<Gempyre::Bitmap>(*frame));
            ++index;
        }
    } else {
        write_png(path, std::get<Gempyre::Bitmap>(**bmp.begin()));
    }
    return 0;
}