#include "test_resources.h"
#include "../src/webp.hpp"
#include <gempyre_bitmap.h>
#include <gempyre.h>
#include <gempyre_utils.h>

int main(int argc, char** argv) {
    gempyre_utils_assert_x(argc > 1, "webp image parameter missing");
    Gempyre::set_debug();
    const auto source = argv[1];
    gempyre_utils_assert(GempyreUtils::file_exists(source));
    Gempyre::Ui ui{{{"/app.html", Testhtml}}, "app.html"};
    const auto url = ui.add_file(source);
    gempyre_utils_assert(url);
    gempyre_utils_assert(ui.resource(*url));
    Gempyre::Element(ui, "oname").set_html("original:" + *url);
    Gempyre::Element(ui, "original").set_attribute("src", *url);
    ui.run();
    return 0;
}