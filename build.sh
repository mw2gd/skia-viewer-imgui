# Move into skia directory (gn only works inside skia directory)
cd ./ext/skia

# Produce libska.a (offical build omits test libraries (i.e., libimgui.a))
# gn gen out/SKIA --args='
# is_debug=false
# is_official_build=true
# skia_enable_optimize_size=true
# skia_use_system_libjpeg_turbo=false
# skia_use_libpng_encode=false
# skia_use_libpng_decode=false
# skia_use_system_libwebp=false
# skia_use_system_zlib=false
# skia_use_system_icu=false
# skia_use_system_harfbuzz=false
# skia_use_system_expat=false
# '
# ninja -C out/SKIA skia

# Produce libimgui.a
cd ./third_party/imgui
gn gen ../../out/TOOLS --args='
is_debug=false
is_official_build=false
skia_enable_optimize_size=true
'
ninja -C ../../out/TOOLS imgui
ninja -C ../../out/TOOLS sk_app

