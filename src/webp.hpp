#pragma once

#include <chrono>
#include <vector>
#include <memory>
#include <optional>

/// @cond INTERNAL
#ifdef WINDOWS_EXPORT
#define GEMPYRE_EX __declspec( dllexport )
#else
#define GEMPYRE_EX
#endif
/// @endcond

namespace Gempyre {
    class Bitmap;
    /// @brief Webp class to easy use of WebP images
    class GEMPYRE_EX Webp {
    public:
        /// @brief Construct with a single frame
        /// @param bitmap
        Webp(const Gempyre::Bitmap& bitmap);
        /// @brief 
        Webp();
        /// @brief Add frames into Webp
        /// @param bitmap 
        /// @param frame_len milliseconds to the next frame
        /// @return 
        bool add(const Gempyre::Bitmap& bitmap, std::chrono::milliseconds frame_len);
        /// @brief encode and get bytes. Can be called only once per Webp instance.
        /// @return bytes as WebP image (file) 
        std::optional<std::vector<uint8_t>> image() const;
        /// @brief Get encoder error information
        /// @return 
        std::string get_error_string() const;
        ~Webp();
    private:    
        class Private;
        std::unique_ptr<Private> m_private;
    };
}

