#include <QApplication>

#include <menubar/file_menu.hpp>
#include <actions.hpp>

FileMenu::FileMenu() {
    this->setTitle("File");
    
    newFile = new QAction("New");
    openFile = new QAction("Open");
    saveFile = new QAction("Save");
    saveFileAs = new QAction("Save As");
    quit = new QAction("Quit");
    
    connect(newFile, &QAction::triggered, this, &FileMenu::onNewFileClicked);
    connect(openFile, &QAction::triggered, this, &FileMenu::onOpenFileClicked);
    connect(saveFile, &QAction::triggered, this, &FileMenu::onSaveFileClicked);
    connect(saveFileAs, &QAction::triggered, this, &FileMenu::onSaveFileAsClicked);
    connect(quit, &QAction::triggered, qApp, &QApplication::quit);
    
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

void FileMenu::onNewFileClicked() {
    Actions::newFile();
}

void FileMenu::onOpenFileClicked() {
    Actions::openFile();
}

void FileMenu::onSaveFileClicked() {
    Actions::saveFile();
}

void FileMenu::onSaveFileAsClicked() {
    Actions::saveFileAs();
}
