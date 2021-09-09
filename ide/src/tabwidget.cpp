#include <tabwidget.hpp>
#include <editor.hpp>

TabWidget::TabWidget() {
    this->addTab(new Editor, "untitled");
}