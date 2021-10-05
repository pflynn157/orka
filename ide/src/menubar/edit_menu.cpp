#include <QPixmap>

#include <menubar/edit_menu.hpp>

EditMenu::EditMenu(Editor *editor) {
    this->editor = editor;
    this->setTitle("Edit");
    
    cut = new QAction(QPixmap(":/icons/edit-cut.svg"), "Cut");
    copy = new QAction(QPixmap(":/icons/edit-copy.svg"), "Copy");
    paste = new QAction(QPixmap(":/icons/edit-paste.svg"), "Paste");
    
    connect(cut, &QAction::triggered, this, &EditMenu::onCutClicked);
    connect(copy, &QAction::triggered, this, &EditMenu::onCopyClicked);
    connect(paste, &QAction::triggered, this, &EditMenu::onPasteClicked);
    
    this->addAction(cut);
    this->addAction(copy);
    this->addAction(paste);
}

EditMenu::~EditMenu() {
    delete cut;
    delete copy;
    delete paste;
}

void EditMenu::onCutClicked() {
    editor->cut();
}

void EditMenu::onCopyClicked() {
    editor->copy();
}

void EditMenu::onPasteClicked() {
    editor->paste();
}
