#include <menubar/file_menu.hpp>
#include <actions.hpp>

FileMenu::FileMenu() {
    this->setTitle("File");
    
    newWindow = new QAction("New Window");
    newFile = new QAction("New File");
    openFile = new QAction("Open File");
    saveFile = new QAction("Save File");
    saveFileAs = new QAction("Save File As");
    quit = new QAction("Quit");
    
    connect(openFile, &QAction::triggered, this, &FileMenu::onOpenFileClicked);
    
    this->addAction(newWindow);
    this->addAction(newFile);
    this->addAction(openFile);
    this->addSeparator();
    this->addAction(saveFile);
    this->addAction(saveFileAs);
    this->addSeparator();
    this->addAction(quit);
}

FileMenu::~FileMenu() {

}

void FileMenu::onOpenFileClicked() {
    Actions::openFile();
}