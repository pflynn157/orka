#include <window.hpp>

Window::Window() {
    this->setWindowTitle("Orka IDE");
    this->resize(800, 500);
    
    editor = new Editor;
    this->setCentralWidget(editor);
}

Window::~Window() {
    if (editor) delete editor;
}
