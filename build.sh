cp ./APPBUILD.gn ./ext/skia/BUILD.gn
cd ./ext/skia
gn gen out/fullapp --args=' 
is_official_build=false
is_debug=false
skia_enable_tools=true
skia_enable_skottie=false
skia_enable_svg=false
skia_use_zlib=false
'
ninja -C ./out/fullapp