add_library(heatmap renderer.cc)

link_directories(/usr/lib)
target_link_libraries(heatmap SDL2)
