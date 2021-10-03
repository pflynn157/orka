#pragma once

#include <QPlainTextEdit>
#include <QString>

#include <highlighter.hpp>
#include <repository.hpp>

class Editor : public QPlainTextEdit {
    Q_OBJECT
public:
    Editor();
    ~Editor();
    void setText(QString text);
    
    void setPath(QString path) { this->path = path; }
    QString getPath() { return path; }
private:
    QString path = "";
    SyntaxHighlighter *highlight;
};
