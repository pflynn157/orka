#pragma once

#include <QMenu>
#include <QAction>

class DosieroMenuo : public QMenu {
    Q_OBJECT
public:
    DosieroMenuo();
    ~DosieroMenuo();
private:
    QAction *novaFenestro, *novaDosiero, *malfermiDosiero;
    QAction *saviDosiero, *saviDosieroKiel;
    QAction *forlasi;
};