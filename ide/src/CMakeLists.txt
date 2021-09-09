cmake_minimum_required(VERSION 3.0.0)
project(ide_src)

set(BASE_SRC
    editor.cpp
    main.cpp
    tabwidget.cpp
    window.cpp
)

set(SRC
    ${BASE_SRC}
)

add_executable(oride ${SRC})

target_link_libraries(oride
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
