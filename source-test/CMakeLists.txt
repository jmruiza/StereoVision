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
