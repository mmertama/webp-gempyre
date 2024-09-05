### [WebP](http://https://en.wikipedia.org/wiki/WebP "WebP") for [Gempyre](http://https://github.com/mmertama/Gempyre "Gempyre")

 

###### What is Gempyre?

[Gempyre](http://https://github.com/mmertama/Gempyre "Gempyre") is a C++ UI library that uses browser as a UI backend and supports at least (i.e. tested on) Linux, Windows, MacOS, Raspberry and Android. There are bindings for [Python](http://https://github.com/mmertama/Gempyre-Python "Python")  - for Rust there are a concurrent native implementation [Gemgui-rs](http://https://github.com/mmertama/gemgui-rs "Gemgui-rs").

 

##### Gempyre::Bitmap

Gempyre::Bitmap is internal Gempyre image format, but drawing that on UI needs either done using HTML Canvas or converted to image format that the browser recognizes. Gempyre has APIs to load and store Bitmaps to and from PNG images. Drawing a single image is magnitudes more efficient when browser renders that using PNG image than blitted from individual pixels using HTML Canvas. Gempyre offers APIs to generate a PNG from Bitmap and then bind a URL to PNG so it can be accessed from HTML componets as any data.

 

#####  class Webp

 

Gempyre supports PNG out from box, but for example animations are good reason to look another formats. Webp class has a simple API to convert GempyreBitmap as WeBP picture and add animation frames.

 Here is a simplified example ( no error handling, if something goes wrong see Webp::get_error_string())

```cpp

void add_animation(std::string_view id,  const std::vector<Gempyre::Bitmap>& bmps) {
  const auto image_key = std::format("/{}.webp", id);
  Webp webp;
  for(const auto& bmp : bmps) {
    webp.add(bmp, 400ms);
  }
  const auto data = webp.image();
  ui.add_data(image_key, *data);
  Gempyre::Element(ui, id).set_attribute("src", image_key);
}

```
Note that Webp expects that all frames are the same size. One solution is to create another bitmap and tile on that - for example tile centrified:

```cpp
Gempyre::Bitmap new_bmp(max_w, max_h, Gempyre::Color::Transparent);
new_bmp.tile((max_x - bmp.width()) / 2, (max_y  - bmp.height()) / 2, bmp);
```
