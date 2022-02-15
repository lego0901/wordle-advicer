include_directories(${WORDLE_SOURCE_DIR}/include)

add_library(common Dictionary.cpp State.cpp Utils.cpp)