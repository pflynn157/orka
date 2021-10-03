#include <editor.hpp>

Editor::Editor() {

}

Editor::~Editor() {}

void Editor::setText(QString text) {
    this->setPlainText(text);
}
