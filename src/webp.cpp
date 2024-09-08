#include <chrono>
#include "webp.hpp"
extern "C" {
    #include "webp/encode.h"
    #include "webp/decode.h"
    #include "webp/mux.h"
    #include "webp/demux.h"
}

using namespace std::chrono_literals; 

class WebPGempyre::WebP::Private {
public:
    
    Private(const PictureConfig& config) : m_config{config}, m_enc{nullptr, WebPAnimEncoderDelete} {
        WebPAnimEncoderOptionsInit(&m_enc_options);
        m_enc_options.minimize_size = config.min_size; 
        m_enc_options.kmax = config.kmax;
        m_enc_options.allow_mixed = config.allow_mixed;  
    }

    Private(const Gempyre::Bitmap& bitmap, const PictureConfig& picture_config, const FrameConfig frame_config) : Private{picture_config} {  
        m_enc.reset(WebPAnimEncoderNew(bitmap.width(), bitmap.height(), &m_enc_options));
        add(bitmap, 0ms, frame_config);
    }

    bool add(const Gempyre::Bitmap& bitmap, std::chrono::milliseconds ms, const FrameConfig frame_config) {

        if (!m_enc) {
            m_enc.reset(WebPAnimEncoderNew(bitmap.width(), bitmap.height(), &m_enc_options));
        }

        if (!m_enc)
            return false;

        WebPConfig config;
        
        if (!WebPConfigInit(&config))
            return false;
        config.lossless = frame_config.lossless;
        config.quality = frame_config.quality;
        config.method = frame_config.compression_method;
        WebPPicture picture;
        
        if (!WebPPictureInit(&picture)) {
            return false;
        }

        picture.use_argb = true;
        picture.width = bitmap.width();
        picture.height = bitmap.height();
        
        if (!WebPPictureImportRGBA(&picture, bitmap.const_data(), bitmap.width() * sizeof(Gempyre::dataT)))
            return false;

        if (!WebPAnimEncoderAdd(m_enc.get(), &picture, m_time_stamp, &config))
            return false;
        
        m_time_stamp += ms.count();
        return true;
    }

    std::string get_error_string() const {
        const auto chr = WebPAnimEncoderGetError(m_enc.get());
        return {chr};
    }
 
    std::optional<std::vector<uint8_t>> encode() {
        if (!WebPAnimEncoderAdd(m_enc.get(), nullptr, m_time_stamp, nullptr))
            return std::nullopt;

        WebPData wep_p;
        WebPDataInit(&wep_p);

        if (!WebPAnimEncoderAssemble(m_enc.get(), &wep_p))
            return std::nullopt;

        std::vector<uint8_t> bytes {wep_p.bytes,  wep_p.bytes + wep_p.size};
        WebPDataClear(&wep_p);
        return bytes;
    }

private:
    const PictureConfig m_config;
    int m_time_stamp{0};
    WebPAnimEncoderOptions m_enc_options;
    std::unique_ptr<WebPAnimEncoder, std::function<void(WebPAnimEncoder*)>> m_enc;
} ;

WebPGempyre::WebP::WebP(const Gempyre::Bitmap& bitmap, const PictureConfig& picture_config, const FrameConfig frame_config) : 
m_private{std::make_unique<Private>(bitmap, picture_config, frame_config)} {
}

WebPGempyre::WebP::WebP(const PictureConfig& config) : m_private{std::make_unique<Private>(config)} {
}

bool WebPGempyre::WebP::add(const Gempyre::Bitmap& bitmap, std::chrono::milliseconds ms, const FrameConfig config) {
    return m_private->add(bitmap, ms, config);
}

std::optional<std::vector<uint8_t>> WebPGempyre::WebP::picture() const {
    return m_private->encode();
}

WebPGempyre::WebP::~WebP() {}

std::string WebPGempyre::WebP::get_error_string() const {
    return m_private->get_error_string();
}

class WebPGempyre::Bitmap::Private {
    public:
    Private(std::span<const uint8_t> bytes) :  m_dec{nullptr, WebPAnimDecoderDelete}  {
        WebPData webp_data{bytes.data(), bytes.size()};
        WebPAnimDecoderOptions dec_options;
        WebPAnimDecoderOptionsInit(&dec_options);
        dec_options.color_mode = MODE_RGBA  ;
        dec_options.use_threads = false;
        m_dec.reset(WebPAnimDecoderNew(&webp_data, &dec_options));
        WebPAnimDecoderGetInfo(m_dec.get(), &m_anim_info); //  anim_info.loop_count;
    }

    Info info() const {
        return {
            m_anim_info.canvas_width,
            m_anim_info.canvas_height,
            m_anim_info.loop_count,
            m_anim_info.bgcolor,
            m_anim_info.frame_count
        };
    }

    std::optional<FrameBitmap> next() { 
        
        if (!WebPAnimDecoderHasMoreFrames(m_dec.get()))
            return std::nullopt;

        const auto sz = m_anim_info.canvas_width * m_anim_info.canvas_height;   
        uint8_t* buf;
        int timestamp;

        if (!WebPAnimDecoderGetNext(m_dec.get(), &buf, &timestamp))
            return std::nullopt;

        if (reinterpret_cast<uintptr_t>(buf) & (sizeof(Gempyre::Color::type) - 1)) // must be aligned data
            return std::nullopt;    

        Gempyre::Bitmap bmp{
            static_cast<int>(m_anim_info.canvas_width),
            static_cast<int>(m_anim_info.canvas_height),
            m_anim_info.bgcolor};
        const auto ptr = reinterpret_cast<const Gempyre::Color::type*>(buf);
        const auto span = std::span<const Gempyre::Color::type>(
                ptr,
                sz);
        bmp.set_data(span);
        const auto time_gap = timestamp - m_current_time;
        m_current_time = timestamp;
        return FrameBitmap{std::move(bmp), std::chrono::milliseconds{time_gap}};
        //const WebPDemuxer* demuxer = WebPAnimDecoderGetDemuxer(dec);
        // ... (Do something using 'demuxer'; e.g. get EXIF/XMP/ICC data).
    }

    void reset() {
        WebPAnimDecoderReset(m_dec.get());
    }

private:
    WebPAnimInfo m_anim_info;
    std::unique_ptr<WebPAnimDecoder, std::function<void(WebPAnimDecoder*)>> m_dec;
    int m_current_time = 0;
};

WebPGempyre::Bitmap::Bitmap(std::span<const uint8_t> webp_bytes) :
    m_private{std::make_unique<Private>(webp_bytes)} {
    }

WebPGempyre::Bitmap::~Bitmap() {}

WebPGempyre::Bitmap::Info WebPGempyre::Bitmap::info() const {
    return m_private->info();
}

WebPGempyre::Bitmap::FrameIterator WebPGempyre::Bitmap::begin() {
    m_private->reset();
    return FrameIterator{[this](){return m_private->next();}};
    }
WebPGempyre::Bitmap::FrameIterator WebPGempyre::Bitmap::end() {
    return FrameIterator([]() -> std::optional<FrameBitmap> {return std::nullopt;});
    }

std::optional<Gempyre::Bitmap> WebPGempyre::Bitmap::bitmap() {
    const auto b = begin();
    return (b != end() && *b) ? std::make_optional((*b)->first) : std::nullopt;
}    
