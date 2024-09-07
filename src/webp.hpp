#pragma once

#include <chrono>
#include <vector>
#include <memory>
#include <optional>
#include <iterator>
#include <functional>
#include <gempyre_bitmap.h>

/// @cond INTERNAL
#ifdef WINDOWS_EXPORT
#define WEBP_GEMPYRE_EX __declspec( dllexport )
#else
#define WEBP_GEMPYRE_EX
#endif
/// @endcond


namespace WebPGempyre {
    struct PictureConfig {
        int kmax = 1;
        bool min_size = false;
        bool allow_mixed = false;
    };
    struct FrameConfig {
        bool lossless = 1;
        int quality = 75; // 0 - 100
        int compression_method = 4; // 0-6
    };
    /// @brief WebP class to easy use of WebP images
    class WEBP_GEMPYRE_EX WebP {
    public:
        /// @brief Construct with a single frame
        /// @param bitmap
        WebP(const Gempyre::Bitmap& bitmap, const PictureConfig& picture_config = PictureConfig{}, const FrameConfig frame_config = FrameConfig{});
        /// @brief 
        WebP(const PictureConfig& config = PictureConfig{});
        /// @brief Add frames into WebP
        /// @param bitmap 
        /// @param frame_len milliseconds to the next frame
        /// @return 
        bool add(const Gempyre::Bitmap& bitmap, std::chrono::milliseconds frame_len, const FrameConfig config = FrameConfig{});
        /// @brief encode and get bytes. Can be called only once per WebP instance.
        /// @return bytes as WebP image (file) 
        std::optional<std::vector<uint8_t>> picture() const;
        /// @brief Get encoder error information
        /// @return 
        std::string get_error_string() const;
        ~WebP();
    private:    
        class Private;
        std::unique_ptr<Private> m_private;
    };
}


