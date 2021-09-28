#pragma once

#include <QMainWindow>
#include <QMenuBar>

#include <editor.hpp>
#include <menubar/dosiero_menuo.hpp>
#include <menubar/edito_menuo.hpp>

class Window : public QMainWindow {
    Q_OBJECT
public:
    Window();
    ~Window();
private:
    Editor *editor;
    QMenuBar *menubar;
    
    DosieroMenuo *dosieroMenuo;
    EditoMenuo *editoMenuo;
};
