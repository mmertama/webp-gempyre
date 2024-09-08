#include "test_resources.h"
#include "../src/webp.hpp"
#include <gempyre_bitmap.h>
#include <gempyre.h>
#include <gempyre_utils.h>
#include <iostream>

int main(int argc, char** argv) {
    gempyre_utils_assert_x(argc > 1, "webp image parameter missing");
    const auto source = argv[1];
    gempyre_utils_assert(GempyreUtils::file_exists(source));
    Gempyre::Ui ui{{{"/app.html", Testhtml}}, "app.html"};
    const auto url = ui.add_file(source);
    gempyre_utils_assert(url);
    gempyre_utils_assert(ui.resource(*url));
    Gempyre::Element(ui, "oname").set_html("original:" + *url);

    // 1 just show webp
    Gempyre::Element(ui, "original").set_attribute("src", *url);

    // 2 convert webp to bitmap and back to webp
    WebPGempyre::WebP webp;
    const auto bytes = GempyreUtils::slurp<uint8_t>(source);
    WebPGempyre::Bitmap bmp(bytes);
    for(const auto& frame : bmp) {
        gempyre_utils_assert(webp.add(frame->first, frame->second));
        std::cout << "." << std::flush;
    }

    const auto bitmap_url = "/image_from_bitmap.webp";
    const auto pic = webp.picture();
    gempyre_utils_assert(pic);
    gempyre_utils_assert(ui.add_data(bitmap_url, *pic));
    Gempyre::Element(ui, "produced").set_attribute("src", bitmap_url);
    std::cout << std::endl;

    // 3 show 1st frame as a PNG
    const auto png_url = "/image_png.png";
    const auto bitmap = bmp.bitmap();
    gempyre_utils_assert(ui.add_data(png_url, bitmap->png_image()));
    Gempyre::Element(ui, "still").set_attribute("src", png_url);

    ui.run();
    return 0;
}