project(ZserioSonarCloudTest)

add_executable(${PROJECT_NAME}
    main.cpp
    misra2023.cpp
    misra2023.h
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
