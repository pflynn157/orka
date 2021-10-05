#pragma once

#include <QMenu>
#include <QAction>

#include <editor.hpp>

class EditMenu : public QMenu {
    Q_OBJECT
public:
    EditMenu(Editor *editor);
    ~EditMenu();
private:
    Editor *editor;
    QAction *cut, *copy, *paste;
private slots:
    void onCutClicked();
    void onCopyClicked();
    void onPasteClicked();
};
