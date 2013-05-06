# -------------------------------------------------------
# Project StereoVision

set(Stereo_SRC
    main.cpp
    pdi.cpp
    fourier.cpp
    harrisDetector.h

    camera.cpp
    cameracalibrator.cpp

    stereovision.cpp
    stereocontroller.cpp
    stereocalibrator.cpp
)

set(Stereo_LIBS
    ${OpenCV_LIBS}
)

add_executable(StereoVision ${Stereo_SRC})
target_link_libraries(StereoVision ${Stereo_LIBS})

# -------------------------------------------------------

