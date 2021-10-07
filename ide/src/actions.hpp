#pragma once

#include <QString>

#include <editor.hpp>
#include <statusbar.hpp>

class Actions {
public:
    static void newFile();
    static void openFile();
    static void saveFile();
    static void saveFileAs();
    
    static Editor *editor;
    static StatusBar *statusbar;
};
