#include <chrono>
#include "gempyre_graphics.h"
#include "webp.hpp"
extern "C" {
    #include "webp/encode.h"
    #include "webp/mux.h"
}

using namespace std::chrono_literals; 

class Gempyre::Webp::Private {
public:
    
    Private() : m_enc(nullptr, WebPAnimEncoderDelete) {
        WebPAnimEncoderOptionsInit(&m_enc_options);
    }

    Private(const Gempyre::Bitmap& bitmap) : m_enc(nullptr, WebPAnimEncoderDelete) {
        WebPAnimEncoderOptionsInit(&m_enc_options);
        m_enc_options.minimize_size = false; 
        m_enc_options.kmax = 1;
        m_enc_options.allow_mixed = false;  
        m_enc.reset(WebPAnimEncoderNew(bitmap.width(), bitmap.height(), &m_enc_options));
        add(bitmap, 0ms);
    }

    bool add(const Gempyre::Bitmap& bitmap, std::chrono::milliseconds ms ) {

        if (!m_enc) {
            m_enc.reset(WebPAnimEncoderNew(bitmap.width(), bitmap.height(), &m_enc_options));
        }

        if (!m_enc)
            return false;

        WebPConfig config;
        
        if (!WebPConfigInit(&config))
            return false;
        config.lossless = true;
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
    int m_time_stamp = 0;
    WebPAnimEncoderOptions m_enc_options;
    std::unique_ptr<WebPAnimEncoder, std::function<void(WebPAnimEncoder*)>> m_enc;
} ;

Gempyre::Webp::Webp(const Gempyre::Bitmap& bitmap) : m_private{std::make_unique<Private>(bitmap)} {}
Gempyre::Webp::Webp() : m_private{std::make_unique<Private>()} {}

bool Gempyre::Webp::add(const Gempyre::Bitmap& bitmap, std::chrono::milliseconds ms) {
    return m_private->add(bitmap, ms);
}

std::optional<std::vector<uint8_t>> Gempyre::Webp::image() const {
    return m_private->encode();
}

Gempyre::Webp::~Webp() {}

std::string Gempyre::Webp::get_error_string() const {
    return m_private->get_error_string();
}
