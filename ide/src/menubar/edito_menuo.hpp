#pragma once

#include <QMenu>
#include <QAction>

#include <editor.hpp>

class EditoMenuo : public QMenu {
    Q_OBJECT
public:
    EditoMenuo(Editor *redaktisto);
    ~EditoMenuo();
private:
    Editor *redaktisto;
    QAction *eltondi, *kopii, *enposxigi;
};
