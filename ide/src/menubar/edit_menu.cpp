#include <menubar/edit_menu.hpp>

EditMenu::EditMenu(Editor *editor) {
    this->editor = editor;
    this->setTitle("Edit");
    
    cut = new QAction("Cut");
    copy = new QAction("Copy");
    paste = new QAction("Paste");
    
    this->addAction(cut);
    this->addAction(copy);
    this->addAction(paste);
}

EditMenu::~EditMenu() {

}
