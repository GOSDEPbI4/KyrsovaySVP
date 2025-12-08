project(tests LANGUAGES CXX)

enable_testing(true)

find_package(Qt6 REQUIRED COMPONENTS Test)

include_directories(${CMAKE_SOURCE_DIR}/src)

# Тесты для Note
add_executable(note_test
    note_test_src/main.cpp
    note_test_src/note_test.hpp
    note_test_src/note_test.cpp
    ${CMAKE_SOURCE_DIR}/src/note.cpp
    ${CMAKE_SOURCE_DIR}/src/note.hpp
)

add_test(NAME note_test COMMAND note_test)
target_link_libraries(note_test PRIVATE Qt6::Test Qt6::Core)

add_executable(notemanager_test
    notemanager_test_src/main.cpp
    notemanager_test_src/notemanager_test.hpp
    notemanager_test_src/notemanager_test.cpp
    ${CMAKE_SOURCE_DIR}/src/note.cpp
    ${CMAKE_SOURCE_DIR}/src/note.hpp
    ${CMAKE_SOURCE_DIR}/src/notemanager.cpp
    ${CMAKE_SOURCE_DIR}/src/notemanager.hpp
)

add_test(NAME notemanager_test COMMAND notemanager_test)
target_link_libraries(notemanager_test PRIVATE Qt6::Test Qt6::Core)
