#include <window.hpp>
#include <actions.hpp>

Window::Window() {
    this->setWindowTitle("Orka IDE");
    this->resize(800, 500);
    
    editor = new Editor;
    this->setCentralWidget(editor);
    Actions::editor = editor;
    
    menubar = new QMenuBar;
    this->setMenuBar(menubar);
    
    // Add the file menu
    fileMenu = new FileMenu;
    menubar->addMenu(fileMenu);
    
    // Add the edit menu
    editMenu = new EditMenu(editor);
    menubar->addMenu(editMenu);
}

Window::~Window() {
    if (editor) delete editor;
    if (menubar) delete menubar;
}
