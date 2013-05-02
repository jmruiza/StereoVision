# -------------------------------------------------------
# Project HarrisDetector

set(InterestPoints_SRC
    harrisDetector.h
    interestPoints.cpp
)

set(InterestPoints_LIBS
    ${OpenCV_LIBS}
)

add_executable(InterestPoints ${InterestPoints_SRC})
target_link_libraries(InterestPoints ${InterestPoints_LIBS})

# -------------------------------------------------------
# Project Tracking

set(Tracking_SRC
    tracking.cpp
)

set(Tracking_LIBS
    ${OpenCV_LIBS}
)

add_executable(Tracking ${Tracking_SRC})
target_link_libraries(Tracking ${Tracking_LIBS})

# -------------------------------------------------------
# Project FeatureDescription
set(FeatureDescription_SRC
    FeatureDescription.cpp
)

set(FeatureDescription_LIBS
    ${OpenCV_LIBS}
)

add_executable(FeatureDescription ${FeatureDescription_SRC})
target_link_libraries(FeatureDescription ${FeatureDescription_LIBS})

# -------------------------------------------------------

# Project Fourier Transform
set(Fourier_SRC
    Fourier.cpp
)

set(Fourier_LIBS
    ${OpenCV_LIBS}
)

add_executable(Fourier ${Fourier_SRC})
target_link_libraries(Fourier ${Fourier_LIBS})

# -------------------------------------------------------

# Project StereoMatch
set(StereoMatch_SRC
    stereo_match.cpp
)

set(StereoMatch_LIBS
    ${OpenCV_LIBS}
)

add_executable(StereoMatch ${StereoMatch_SRC})
target_link_libraries(StereoMatch ${StereoMatch_LIBS})

# -------------------------------------------------------
