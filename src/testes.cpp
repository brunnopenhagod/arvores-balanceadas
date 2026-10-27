#include "avl.hpp"
#include "rb.hpp"
#include <set>
#include <random>
#include <chrono>
#include <iomanip>

int total = 0;
int falhas = 0;

void checar(bool cond, const string& desc) {
    total++;
    if (!cond) {
        falhas++;
        cout << "  [FALHOU] " << desc << "\n";
    } else {
        cout << "  [OK] " << desc << "\n";
    }
}

bool igual(Arvore& a, set<int>& ref) {
    vector<int> v;
    a.emOrdem(v);
    return v == vector<int>(ref.begin(), ref.end());
}

struct NoBST {
    int c;
    NoBST* e;
    NoBST* d;
};

int alturaBST(const vector<int>& seq) {
    NoBST* raiz = nullptr;
    int maior = 0;
    vector<NoBST*> todos;
    for (int x : seq) {
        NoBST* novo = new NoBST{x, nullptr, nullptr};
        todos.push_back(novo);
        int prof = 1;
        if (!raiz) raiz = novo;
        else {
            NoBST* p = raiz;
            while (true) {
                prof++;
                if (x < p->c) {
                    if (!p->e) { p->e = novo; break; }
                    p = p->e;
                } else {
                    if (!p->d) { p->d = novo; break; }
                    p = p->d;
                }
            }
        }
        maior = max(maior, prof);
    }
    for (NoBST* p : todos) delete p;
    return maior;
}

vector<int> crescente(int n) {
    vector<int> v;
    for (int i = 1; i <= n; i++) v.push_back(i);
    return v;
}

vector<int> decrescente(int n) {
    vector<int> v;
    for (int i = n; i >= 1; i--) v.push_back(i);
    return v;
}

vector<int> aleatoria(int n, int semente) {
    vector<int> v = crescente(n);
    mt19937 g(semente);
    shuffle(v.begin(), v.end(), g);
    return v;
}

void testeVazia(Arvore& a) {
    cout << "\n[" << a.nome() << "] arvore vazia\n";
    vector<int> v;
    a.emOrdem(v);
    checar(!a.buscar(10), "buscar em arvore vazia retorna falso");
    checar(!a.remover(10), "remover em arvore vazia retorna falso");
    checar(v.empty(), "percurso em ordem vazio");
    checar(a.altura() == 0, "altura 0");
    checar(a.validar(), "propriedades validas");
}

void testeUmElemento(Arvore& a) {
    cout << "\n[" << a.nome() << "] um elemento\n";
    checar(a.inserir(10), "inserir 10");
    checar(a.buscar(10), "buscar 10");
    checar(a.altura() == 1, "altura 1");
    checar(a.validar(), "propriedades validas");
    checar(a.remover(10), "remover 10");
    checar(a.tamanho() == 0 && a.altura() == 0, "arvore volta a ficar vazia");
    checar(a.validar(), "propriedades validas apos remover");
}

void testeDuplicadoEInexistente(Arvore& a) {
    cout << "\n[" << a.nome() << "] duplicados e inexistentes\n";
    a.inserir(5);
    a.inserir(3);
    a.inserir(8);
    checar(!a.inserir(5), "inserir 5 repetido e recusado");
    checar(a.tamanho() == 3, "tamanho continua 3");
    checar(!a.buscar(99), "buscar 99 inexistente retorna falso");
    checar(!a.remover(99), "remover 99 inexistente retorna falso");
    checar(a.tamanho() == 3 && a.validar(), "arvore intacta");
}

void testeRemocaoFilhos(Arvore& a) {
    cout << "\n[" << a.nome() << "] remocao com 0, 1 e 2 filhos\n";
    set<int> ref;
    int base[] = {50, 30, 70, 20, 40, 60, 80, 35, 85};
    for (int x : base) {
        a.inserir(x);
        ref.insert(x);
    }
    a.remover(20);
    ref.erase(20);
    checar(igual(a, ref) && a.validar(), "remover 20 (folha)");
    a.remover(40);
    ref.erase(40);
    checar(igual(a, ref) && a.validar(), "remover 40 (um filho)");
    a.remover(30);
    ref.erase(30);
    checar(igual(a, ref) && a.validar(), "remover 30 (dois filhos)");
    a.remover(50);
    ref.erase(50);
    checar(igual(a, ref) && a.validar(), "remover 50 (raiz)");
}

void testeOrdens(Arvore& a, const string& nomeOrdem, const vector<int>& seq) {
    set<int> ref;
    for (int x : seq) {
        a.inserir(x);
        ref.insert(x);
    }
    checar(igual(a, ref) && a.validar(), "insercao " + nomeOrdem + " de " + to_string(seq.size()) + " elementos");
}

void testeEstresse(Arvore& a) {
    cout << "\n[" << a.nome() << "] estresse: 20000 operacoes aleatorias validando a cada uma\n";
    mt19937 g(7);
    set<int> ref;
    bool tudoOk = true;
    for (int i = 0; i < 20000; i++) {
        int x = g() % 500;
        if (g() % 2) {
            bool r = a.inserir(x);
            if (r != ref.insert(x).second) tudoOk = false;
        } else {
            bool r = a.remover(x);
            if (r != (ref.erase(x) > 0)) tudoOk = false;
        }
        if (!a.validar()) tudoOk = false;
    }
    checar(tudoOk && igual(a, ref), "retornos, conteudo e propriedades corretos em todas as operacoes");
}

void comparar(const string& nomeCenario, const vector<int>& ins, const vector<int>& rem) {
    AVL avl;
    RB rb;
    set<int> ref;
    for (int x : ins) {
        avl.inserir(x);
        rb.inserir(x);
    }
    long long rotAvlIns = avl.rotacoes, rotRbIns = rb.rotacoes, recIns = rb.recoloracoes;
    int hAvlIns = avl.altura(), hRbIns = rb.altura();
    for (int x : rem) {
        avl.remover(x);
        rb.remover(x);
    }
    cout << left << setw(26) << nomeCenario
         << " | AVL ins: h=" << setw(3) << hAvlIns << " rot=" << setw(6) << rotAvlIns
         << " | RB ins: h=" << setw(3) << hRbIns << " rot=" << setw(6) << rotRbIns << " rec=" << setw(6) << recIns
         << " | AVL rem: h=" << setw(3) << avl.altura() << " rot=" << setw(6) << avl.rotacoes - rotAvlIns
         << " | RB rem: h=" << setw(3) << rb.altura() << " rot=" << setw(6) << rb.rotacoes - rotRbIns << " rec=" << setw(6) << rb.recoloracoes - recIns
         << " | validas: " << (avl.validar() && rb.validar() ? "sim" : "NAO") << "\n";
}

double tempo(Arvore& a, const vector<int>& ins, const vector<int>& buscas, long long& achados) {
    auto t0 = chrono::steady_clock::now();
    for (int x : ins) a.inserir(x);
    achados = 0;
    for (int x : buscas) achados += a.buscar(x);
    auto t1 = chrono::steady_clock::now();
    return chrono::duration<double, milli>(t1 - t0).count();
}

int main() {
    cout << "===== TESTES FUNCIONAIS =====\n";
    for (int t = 0; t < 2; t++) {
        Arvore* a;
        if (t == 0) a = new AVL(); else a = new RB();
        testeVazia(*a);
        delete a;
        if (t == 0) a = new AVL(); else a = new RB();
        testeUmElemento(*a);
        delete a;
        if (t == 0) a = new AVL(); else a = new RB();
        testeDuplicadoEInexistente(*a);
        delete a;
        if (t == 0) a = new AVL(); else a = new RB();
        testeRemocaoFilhos(*a);
        delete a;
        if (t == 0) a = new AVL(); else a = new RB();
        cout << "\n[" << a->nome() << "] ordens de insercao\n";
        testeOrdens(*a, "crescente", crescente(1000));
        delete a;
        if (t == 0) a = new AVL(); else a = new RB();
        testeOrdens(*a, "decrescente", decrescente(1000));
        delete a;
        if (t == 0) a = new AVL(); else a = new RB();
        testeOrdens(*a, "aleatoria", aleatoria(1000, 42));
        delete a;
        if (t == 0) a = new AVL(); else a = new RB();
        testeEstresse(*a);
        delete a;
    }
    cout << "\nTotal: " << total << " verificacoes, " << falhas << " falhas\n";

    cout << "\n===== ALTURA DA BST SEM BALANCEAMENTO (N=1000) =====\n";
    cout << "crescente: " << alturaBST(crescente(1000)) << "\n";
    cout << "decrescente: " << alturaBST(decrescente(1000)) << "\n";
    cout << "aleatoria: " << alturaBST(aleatoria(1000, 42)) << "\n";

    cout << "\n===== COMPARACAO (mesmas sequencias nas duas arvores) =====\n";
    for (int n : {1000, 100000}) {
        vector<int> asc = crescente(n), desc = decrescente(n), rnd = aleatoria(n, 42);
        vector<int> metade;
        for (int i = 0; i < n; i += 2) metade.push_back(rnd[i]);
        cout << "\nN = " << n << " (remocao de " << metade.size() << " chaves)\n";
        comparar("crescente", asc, metade);
        comparar("decrescente", desc, metade);
        comparar("aleatoria", rnd, metade);
    }

    cout << "\n===== TEMPO (N=1000000, insercao aleatoria + 1000000 buscas) =====\n";
    {
        int n = 1000000;
        vector<int> ins = aleatoria(n, 1), buscas = aleatoria(n, 2);
        AVL avl;
        RB rb;
        long long a1, a2;
        double ta = tempo(avl, ins, buscas, a1);
        double tr = tempo(rb, ins, buscas, a2);
        cout << "AVL: " << fixed << setprecision(0) << ta << " ms, altura " << avl.altura() << ", rotacoes " << avl.rotacoes << "\n";
        cout << "RB: " << tr << " ms, altura " << rb.altura() << ", rotacoes " << rb.rotacoes << ", recoloracoes " << rb.recoloracoes << "\n";
    }
    return falhas ? 1 : 0;
}
