include_directories(${WORDLE_SOURCE_DIR}/include)

link_directories(${WORDLE_SOURCE_DIR}/src/common)

add_library(brain ComputationalState.cpp Guess.cpp)

target_link_libraries(brain common)