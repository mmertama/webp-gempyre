### [WebP](http://https://en.wikipedia.org/wiki/WebP "WebP") for [Gempyre](http://https://github.com/mmertama/Gempyre "Gempyre")


####  class Webp

Gempyre supports PNG out from box, but for example animations are good reason to look another formats. Webp class has a simple API to convert GempyreBitmap as WeBP picture and add animation frames.

##### Gempyre::Bitmap

Gempyre::Bitmap is internal Gempyre image class. Drawing that on UI needs either done using HTML Canvas or bitmap has to be converted to image format that used browser supports.  Drawing a single image is magnitudes more efficient when browser renders that using a native image than blitted from individual pixels using HTML Canvas. Gempyre has APIs to use PNG images and let generate a PNG from Bitmap and then bind a URL to PNG so it can be accessed from HTML componets as any data.

 ##### Using Webp

 Here is a simplified example ( no error handling, if something goes wrong see Webp::get_error_string())

```cpp

void add_animation(Gempyre::Ui &ui, std::string_view id, const std::vector<Gempyre::Bitmap>& bmps) {
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

#### API

##### Webp(const Gempyre::Bitmap& bitmap); 
###### Construct with a single frame.
###### param bitmap.
##### Webp();
##### bool add(const Gempyre::Bitmap& bitmap, std::chrono::milliseconds frame_len);
###### Add frames into Webp.
###### param bitmap. 
###### param frame_len milliseconds to the next frame.
###### return success. 
##### std::optional&lt;std::vector&lt;uint8_t&gt;&gt; image() const;    
###### encode and get bytes. Can be called only once per Webp instance.
###### return bytes as WebP image (file). 
##### std::string get_error_string() const;    
###### Get encoder error information.
###### return error string.
   

#### What is Gempyre?

[Gempyre](http://https://github.com/mmertama/Gempyre "Gempyre") is a C++ UI library that uses browser as a UI backend. Gempyre supports at least (i.e. tested on) Linux, Windows, MacOS, Raspberry and Android. There are bindings for [Python](http://https://github.com/mmertama/Gempyre-Python "Python") - for Rust there is a concurrent native implementation [Gemgui-rs](http://https://github.com/mmertama/gemgui-rs "Gemgui-rs").
