#pragma once

#include <QMainWindow>
#include <QMenuBar>

#include <editor.hpp>
#include <menubar/file_menu.hpp>
#include <menubar/edit_menu.hpp>

class Window : public QMainWindow {
    Q_OBJECT
public:
    Window();
    ~Window();
private:
    Editor *editor;
    QMenuBar *menubar;
    
    FileMenu *fileMenu;
    EditMenu *editMenu;
};
