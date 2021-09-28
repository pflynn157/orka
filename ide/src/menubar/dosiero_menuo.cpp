#include <menubar/dosiero_menuo.hpp>

DosieroMenuo::DosieroMenuo() {
    this->setTitle("Dosiero");
    
    novaFenestro = new QAction("Nova Fenestro");
    novaDosiero = new QAction("Nova Dosiero");
    malfermiDosiero = new QAction("Malfermi Dosiero");
    saviDosiero = new QAction("Savi Dosiero");
    saviDosieroKiel = new QAction("Savi Dosiero Kiel");
    forlasi = new QAction("Forlasi");
    
    this->addAction(novaFenestro);
    this->addAction(novaDosiero);
    this->addAction(malfermiDosiero);
    this->addSeparator();
    this->addAction(saviDosiero);
    this->addAction(saviDosieroKiel);
    this->addSeparator();
    this->addAction(forlasi);
}

DosieroMenuo::~DosieroMenuo() {

}
