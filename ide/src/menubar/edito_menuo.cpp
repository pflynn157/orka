#include <menubar/edito_menuo.hpp>

EditoMenuo::EditoMenuo(Editor *redaktisto) {
    this->redaktisto = redaktisto;
    this->setTitle("Edito");
    
    eltondi = new QAction("Eltondi");
    kopii = new QAction("Kopii");
    enposxigi = new QAction("Enpoŝigi");
    
    this->addAction(eltondi);
    this->addAction(kopii);
    this->addAction(enposxigi);
}

EditoMenuo::~EditoMenuo() {

}