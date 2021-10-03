#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>

#include <actions.hpp>

Editor *Actions::editor = nullptr;

// Process an open file
void Actions::openFile() {
    QFileDialog dialog;
    dialog.setWindowTitle("Open File");
    
    if (!dialog.exec()) return;
    if (dialog.selectedFiles().size() == 0) return;
    
    QString selected = dialog.selectedFiles().at(0);
    QFile file(selected);
    QString content = "";
    
    if (file.open(QFile::ReadOnly)) {
        QTextStream reader(&file);
        while (!reader.atEnd())
            content += reader.readLine() + "\n";
    }
    
    editor->setText(content);
    editor->setPath(selected);
}

// The save-file handler
void Actions::saveFile() {
    if (editor->isUntitled()) {
        saveFileAs();
        return;
    }
    
    QString path = editor->getPath();
    QFile file(path);
    
    if (!file.exists()) {
        saveFileAs();
        return;
    }
    
    if (file.open(QFile::WriteOnly)) {
        QTextStream writer(&file);
        writer << editor->getText();
    }
}

// The save-file-as action handler
void Actions::saveFileAs() {
    QFileDialog dialog;
    dialog.setDirectory(QDir::homePath());
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setWindowTitle("Save File As");
    
    if (dialog.exec() != QFileDialog::Accepted) {
        return;
    }
    
    QString path = dialog.selectedFiles().at(0);
    editor->setPath(path);
    
    QFile(path).open(QFile::ReadWrite);
    saveFile();
}
