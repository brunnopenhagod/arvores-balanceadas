#include "avl.hpp"
#include "rb.hpp"

int main() {
    cout << "Escolha a arvore:\n1 - AVL\n2 - Rubro-Negra\n> ";
    int tipo;
    cin >> tipo;
    Arvore* a;
    if (tipo == 1) a = new AVL();
    else if (tipo == 2) a = new RB();
    else {
        cout << "Opcao invalida\n";
        return 1;
    }
    cout << "Arvore " << a->nome() << " criada\n";

    int op = -1;
    while (op != 0) {
        cout << "\n1 - Inserir\n2 - Remover\n3 - Buscar\n4 - Percurso em ordem\n5 - Imprimir arvore\n6 - Altura e estatisticas\n7 - Validar propriedades\n0 - Sair\n> ";
        if (!(cin >> op)) break;
        int x;
        if (op == 1) {
            cout << "Valor: ";
            cin >> x;
            if (a->inserir(x)) cout << x << " inserido\n";
            else cout << x << " ja existe, insercao ignorada\n";
        } else if (op == 2) {
            cout << "Valor: ";
            cin >> x;
            if (a->remover(x)) cout << x << " removido\n";
            else cout << x << " nao existe, nada removido\n";
        } else if (op == 3) {
            cout << "Valor: ";
            cin >> x;
            cout << x << (a->buscar(x) ? " encontrado\n" : " nao encontrado\n");
        } else if (op == 4) {
            vector<int> v;
            a->emOrdem(v);
            if (v.empty()) cout << "(arvore vazia)";
            for (int e : v) cout << e << " ";
            cout << "\n";
        } else if (op == 5) {
            a->imprimir();
        } else if (op == 6) {
            cout << "Elementos: " << a->tamanho() << "\nAltura: " << a->altura() << "\nRotacoes: " << a->rotacoes << "\nRecoloracoes: " << a->recoloracoes << "\n";
        } else if (op == 7) {
            cout << (a->validar() ? "Propriedades OK\n" : "Propriedades VIOLADAS\n");
        } else if (op != 0) {
            cout << "Opcao invalida\n";
        }
    }
    delete a;
    return 0;
}
