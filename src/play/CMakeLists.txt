include_directories(${WORDLE_SOURCE_DIR}/include)

link_directories(${WORDLE_SOURCE_DIR}/src/common)
link_directories(${WORDLE_SOURCE_DIR}/src/brain)

add_executable(Play Play.cpp)
add_executable(Advice Advice.cpp)

target_link_libraries(Play common brain)
target_link_libraries(Advice common brain)