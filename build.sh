# Replace the build file within the skia repository with 
# custom build
cp ./APPBUILD.gn ./ext/skia/BUILD.gn

# Run gn within the skia repository
cd ./ext/skia
gn gen ../../test/ --args='
is_debug=false
skia_enable_optimize_size=true

skia_enable_tools=true
skia_use_gl=true

skia_enable_svg=false

skia_use_system_libjpeg_turbo=false
skia_use_system_libpng=false 
skia_use_system_libwebp=false
skia_use_system_zlib=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
skia_use_system_expat=false

skia_compile_modules=true
'

# Run ninja to output the application
cd ../..
ninja -C ./test/ fullapp

# Reduce application size by removing symbols
strip ./test/fullapp