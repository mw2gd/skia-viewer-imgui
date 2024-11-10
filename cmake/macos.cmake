# Everything in this file is copied from the skia ninja files
# of the 'viewer' sample application. Do not modify unless 
# there is need for optimizing output size

add_definitions(
    -DNDEBUG
    -DSK_TRIVIAL_ABI=[[clang::trivial_abi]]
    -DSK_GAMMA_APPLY_TO_A8
    -DSK_ALLOW_STATIC_GLOBAL_INITIALIZERS=1
    -DGPU_TEST_UTILS=1
    -DSK_ENABLE_AVX512_OPTS
    -DSK_USE_INTERNAL_VULKAN_HEADERS
    -DSK_TYPEFACE_FACTORY_CORETEXT
    -DSK_FONTMGR_CORETEXT_AVAILABLE
    -DSK_GL
    -DSK_SUPPORT_PDF
    -DSK_CODEC_DECODES_JPEG
    -DSK_CODEC_DECODES_JPEG_GAINMAPS
    -DSK_XML
    -DSK_ENABLE_ANDROID_UTILS
    -DSK_HAS_HEIF_LIBRARY
    -DSK_CODEC_DECODES_ICO
    -DSK_CODEC_DECODES_PNG
    -DSK_CODEC_DECODES_RAW
    -DSK_CODEC_DECODES_WEBP
    -DSK_HAS_WUFFS_LIBRARY
    -DSK_CODEC_DECODES_GIF
    -DSK_CODEC_DECODES_BMP
    -DSK_CODEC_DECODES_WBMP
    -DSK_ENABLE_PRECOMPILE
    -DSK_ASSUME_GL=1
    -DSK_GANESH
    -DSK_USE_PERFETTO
    -DSK_ENABLE_API_AVAILABLE
    -DSK_SHAPER_PRIMITIVE_AVAILABLE
    -DSK_SHAPER_CORETEXT_AVAILABLE
    -DSK_SHAPER_HARFBUZZ_AVAILABLE
    -DSK_SHAPER_UNICODE_AVAILABLE
    -DSK_UNICODE_AVAILABLE
    -DSK_UNICODE_ICU_IMPLEMENTATION
    -DIMGUI_ENABLE_OSX_DEFAULT_CLIPBOARD_FUNCTIONS
)

set(PLATLIBS ${PLATLIBS}
    "-framework ApplicationServices"
    "-framework OpenGL"
    "-framework AppKit"
    "-framework QuartzCore"
    "-framework Cocoa"
    "-framework Foundation"
)

set(OBJECTS ${OBJECTS}
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/sk_app/sk_app.CommandSet.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/sk_app/sk_app.Window.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/sk_app/mac/sk_app.Window_mac.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/sk_app/mac/sk_app.main_mac.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/library/load_dynamic_library.LoadDynamicLibrary_posix.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.AndroidSkDebugToStdOut.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.DDLPromiseImageHelper.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.DDLTileHelper.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.DecodeUtils.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.EncodeUtils.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.GpuToolUtils.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.LsanSuppressions.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.MSKPPlayer.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.ProcStats.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.Resources.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.RuntimeBlendUtils.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.SkMetaData.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.SkSharingProc.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.TestFontDataProvider.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.ToolUtils.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.TsanSuppressions.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.UrlDataManager.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/debugger/tool_utils.DebugCanvas.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/debugger/tool_utils.DebugLayerManager.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/debugger/tool_utils.DrawCommand.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/debugger/tool_utils.JsonWriteBuffer.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/fonts/tool_utils.FontToolUtils.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/fonts/tool_utils.RandomScalerContext.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/fonts/tool_utils.TestFontMgr.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/fonts/tool_utils.TestSVGTypeface.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/fonts/tool_utils.TestTypeface.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/text/tool_utils.SkTextBlobTrace.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/timer/tool_utils.Timer.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.SvgPathExtractor.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.CrashHandler.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/tool_utils.AutoreleasePool.o
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/skia/out/Release/obj/tools/flags/flags.CommandLineFlags.o
)
