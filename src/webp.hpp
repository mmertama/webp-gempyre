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

#if 0
    class GEMPYRE_EX Bitmap {
        public:
            struct FrameIterator {
                using value_type = std::optional<Gempyre::Bitmap>;
                using difference_type = std::size_t;
                using iterator_category = std::input_iterator_tag;
                FrameIterator() = default;
                FrameIterator(std::function<value_type(size_t)>&& f, std::size_t position) : m_f{std::move(f)}, m_pos{position} {}
                const auto& operator*() const { return m_curr; }
                FrameIterator& operator++() { m_curr = m_f(++m_pos); return *this; }
                FrameIterator operator++(int) {  m_curr = m_f(m_pos); ++m_pos; return *this; }
                bool operator==(const FrameIterator& other) const { return m_pos == other.m_pos && (!(m_curr && other.m_curr) || ((m_curr && other.m_curr) && m_curr->const_data() == other.m_curr->const_data())) ; }
            private:
                std::function<value_type (size_t)> m_f{};
                size_t m_pos{0};
                value_type m_curr{};
            };
            Bitmap(const std::vector<uint8_t>& webp_bytes);
            ~Bitmap();
            std::size_t frame_count() const;
            std::optional<Gempyre::Bitmap> bitmap(size_t index) const;
            auto begin() const {return FrameIterator{[this](std::size_t index){return bitmap(index);}, 0};}
            auto end() const {return FrameIterator{[this](std::size_t index) {return bitmap(index);}, frame_count()};}
        private:
            class Private;
            std::unique_ptr<Private> m_private;
    };
}

#endif

