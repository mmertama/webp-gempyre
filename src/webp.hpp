#pragma once

#include <chrono>
#include <vector>
#include <memory>
#include <optional>
#include <iterator>
#include <functional>
#include <span>
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
    /// @brief WebP class to create WebP images
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

    /// @brief Bitmap class to create Bitmap of WebP images
    class GEMPYRE_EX Bitmap {
        public:
            /// @brief 
            struct Info {
                uint32_t width;
                uint32_t height;
                uint32_t loops;
                uint32_t bgcolor;
                uint32_t frames;
            };
            /// @brief 
            using FrameBitmap = std::pair<Gempyre::Bitmap, std::chrono::milliseconds>;
            /// @brief 
            struct FrameIterator {
                using difference_type = std::ptrdiff_t;
                using element_type = std::optional<Bitmap::FrameBitmap>;
                using pointer = element_type*;
                using reference = element_type &;
                using GetNext = std::function<element_type ()>;
            public:
                FrameIterator() = default;
                FrameIterator(GetNext&& next) : m_next{next}, m_curr{m_next()} {}
                FrameIterator (const FrameIterator& other) = default;
                FrameIterator& operator=(const FrameIterator& other) = default;
                const auto& operator*() const { return m_curr; }
                FrameIterator& operator++() {
                     m_curr = m_next();
                     return *this; }
                FrameIterator operator++(int) { auto tmp = *this;++(*this);return tmp; }
                auto operator==(const FrameIterator& other) const {
                    if(!m_curr.has_value() && !other.m_curr.has_value()) return true;
                    if(!m_curr.has_value() || !other.m_curr.has_value()) return false;
                    return m_curr->first.const_data() == other.m_curr->first.const_data() && m_curr->second == other.m_curr->second;
                    }
            private:
                GetNext m_next;
                element_type m_curr;
            };
            /// @brief 
            /// @param webp_bytes 
            Bitmap(std::span<const uint8_t> webp_bytes);
            /// @brief 
            ~Bitmap();
            /// @brief 
            /// @return 
            Info info() const;
            /// @brief 
            /// @return 
            FrameIterator begin();
            /// @brief 
            /// @return 
            FrameIterator end();
            /// @brief Convenience function to return the 1st (or any) bitmap.
            /// @return 
            std::optional<Gempyre::Bitmap> bitmap();
        private:
            class Private;
            std::unique_ptr<Private> m_private;
    };

    static_assert(std::forward_iterator<Bitmap::FrameIterator>);

}
