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
    Gempyre::Element(ui, "original").set_attribute("src", *url);

    const auto bytes = GempyreUtils::slurp<uint8_t>(source);
    WebPGempyre::Bitmap bmp(bytes);
    WebPGempyre::WebP webp;
    for(const auto& frame : bmp) {
        gempyre_utils_assert(webp.add(frame->first, frame->second));
        std::cout << "." << std::flush;
    }
    const auto c_url = "/image_url.webp";
    const auto pic = webp.picture();
    gempyre_utils_assert(pic);
    gempyre_utils_assert(ui.add_data(c_url, *pic));
    Gempyre::Element(ui, "produced").set_attribute("src", *url);
    std::cout << std::endl;
    ui.run();
    return 0;
}