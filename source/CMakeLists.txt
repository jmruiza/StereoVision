# -------------------------------------------------------
# Project without GUI

set(ClassCam_SRC
    camera_test.cpp
    camera.cpp
)

set(ClassCam_LIBS
    ${OpenCV_LIBS}
)

add_executable(ClassCam ${ClassCam_SRC})
target_link_libraries(ClassCam ${ClassCam_LIBS})

# -------------------------------------------------------
# Project StereoVision

set(Stereo_SRC
    main.cpp
    camera.cpp
    stereovision.cpp
)

set(Stereo_LIBS
    ${OpenCV_LIBS}
)

add_executable(StereoVision ${Stereo_SRC})
target_link_libraries(StereoVision ${Stereo_LIBS})

# -------------------------------------------------------
