#include <window.hpp>

Window::Window() {
    this->setWindowTitle("Orka IDE");
    this->resize(800, 500);
    
    tabs = new TabWidget;
    this->setCentralWidget(tabs);
}

Window::~Window() {
    if (tabs) delete tabs;
}
