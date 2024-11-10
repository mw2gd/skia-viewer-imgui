# Replace the build file within the skia repository with 
# custom build
cp ./APPBUILD.gn ./ext/skia/BUILD.gn

# Run gn within the skia repository
cd ./ext/skia
gn gen ../../build/fullapp --args=' 
is_official_build=false
is_debug=false
skia_enable_tools=true
skia_enable_skottie=false
skia_enable_svg=false
skia_use_zlib=false
'

# Run ninja to output the application
cd ../..
ninja -C ./build/fullapp fullapp