#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>

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
