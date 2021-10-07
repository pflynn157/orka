#pragma once

#include <QMainWindow>
#include <QMenuBar>

#include <editor.hpp>
#include <toolbar.hpp>
#include <statusbar.hpp>
#include <menubar/file_menu.hpp>
#include <menubar/edit_menu.hpp>

class Window : public QMainWindow {
    Q_OBJECT
public:
    Window();
    ~Window();
private:
    Editor *editor;
    ToolBar *toolbar;
    QMenuBar *menubar;
    StatusBar *statusbar;
    
    FileMenu *fileMenu;
    EditMenu *editMenu;
};
