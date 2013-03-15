# -------------------------------------------------------
# Project without GUI

set(Stereo_SRC
    mainTest.cpp
    camera.cpp
)

set(Stereo_LIBS
    ${OpenCV_LIBS}
)

add_executable(StereoVision ${Stereo_SRC})
target_link_libraries(StereoVision ${Stereo_LIBS})

# -------------------------------------------------------

