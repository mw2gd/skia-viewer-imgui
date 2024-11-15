# Replace the build file within the skia repository with 
# custom build
cp ./APPBUILD.gn ./ext/skia/BUILD.gn

# Move into skia directory (gn only works inside skia directory)
cd ./ext/skia

# Handle dependencies
#python3 tools/git-sync-deps
#python3 bin/fetch-ninja

# Produce libskia (offical build omits test libraries)
gn gen out/skia --args='
is_debug=false
is_official_build=true
skia_enable_optimize_size=true
skia_use_system_libjpeg_turbo=false
skia_use_libpng_encode=false
skia_use_libpng_decode=false
skia_use_system_libwebp=false
skia_use_system_zlib=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
skia_use_system_expat=false
'
ninja -C out/skia skia

# Produce sk_app
gn gen out/sk_app --args='
is_debug=false
skia_enable_tools=true
'
ninja -C out/sk_app sk_app

# Reduce application size by removing symbols
# strip out/sk_app/
