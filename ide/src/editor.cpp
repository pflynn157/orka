#include <editor.hpp>

Editor::Editor() {
    highlight = new SyntaxHighlighter(this->document());
    highlight->setLanguage("orka");
}

Editor::~Editor() {}

void Editor::setText(QString text) {
    this->setPlainText(text);
}
