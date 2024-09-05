find_library(webpg_path webp_gempyre REQUIRED)
find_package(WebP CONFIG REQUIRED) #  brew install webp
set(WEBP_LIB WebP::webp WebP::libwebpmux)
# Expose a target
add_library(webp_gempyre::webp_gempyre UNKNOWN IMPORTED)
# Set the imported library file 
set_target_properties(webp_gempyre::webp_gempyre PROPERTIES
    IMPORTED_LOCATION ${webpg_path}
)
# Link dependencies automatically
target_link_libraries(webp_gempyre::webp_gempyre INTERFACE ${WEBP_LIB})
