#pragma once

#include <QMainWindow>

#include <editor.hpp>

class Window : public QMainWindow {
    Q_OBJECT
public:
    Window();
    ~Window();
private:
    Editor *editor;
};
