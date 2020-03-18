add_library(heatmap renderer.cc renderer_exceptions.cc heatmap.cc)

target_link_libraries(heatmap SDL2)
