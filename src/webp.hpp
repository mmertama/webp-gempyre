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
}

class GEMPYRE_EX Webp {
public:
    Webp(const Gempyre::Bitmap& bitmap);
    Webp();
    bool add(const Gempyre::Bitmap& bitmap, std::chrono::milliseconds frame_len);
    std::optional<std::vector<uint8_t>> image() const;
    std::string get_error_string() const;
    ~Webp();
private:    
    class Private;
    std::unique_ptr<Private> m_private;
};

