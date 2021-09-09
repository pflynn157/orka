#pragma once

#include <QMainWindow>

#include <tabwidget.hpp>

class Window : public QMainWindow {
    Q_OBJECT
public:
    Window();
    ~Window();
private:
    TabWidget *tabs;
};
