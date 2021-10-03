#include <editor.hpp>

Editor::Editor() {
    highlight = new SyntaxHighlighter(this->document());
    highlight->setLanguage("orka");
    
    QFont font("Liberation Mono", 12);
    this->setFont(font);
}

Editor::~Editor() {}

void Editor::setText(QString text) {
    this->setPlainText(text);
}

QString Editor::getText() {
    return this->toPlainText();
}
