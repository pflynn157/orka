#include <menubar/edit_menu.hpp>

EditMenu::EditMenu(Editor *editor) {
    this->editor = editor;
    this->setTitle("Edit");
    
    cut = new QAction("Cut");
    copy = new QAction("Copy");
    paste = new QAction("Paste");
    
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
