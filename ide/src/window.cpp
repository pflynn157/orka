#include <window.hpp>

// La konstruo por la fenestro
Window::Window() {
    this->setWindowTitle("Orka IDE");
    this->resize(800, 500);
    
    editor = new Editor;
    this->setCentralWidget(editor);
    
    menubar = new QMenuBar;
    this->setMenuBar(menubar);
    
    // Aldoni la dosiero menuo
    dosieroMenuo = new DosieroMenuo;
    menubar->addMenu(dosieroMenuo);
    
    // la edito menuo
    editoMenuo = new EditoMenuo(editor);
    menubar->addMenu(editoMenuo);
}

Window::~Window() {
    if (editor) delete editor;
    if (menubar) delete menubar;
}
