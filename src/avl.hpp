#pragma once
#include "arvore.hpp"

struct NoAVL {
    int chave;
    int h;
    NoAVL* esq;
    NoAVL* dir;
    NoAVL(int c) : chave(c), h(1), esq(nullptr), dir(nullptr) {}
};

class AVL : public Arvore {
    NoAVL* raiz = nullptr;
    int n = 0;

    int alt(NoAVL* no) { return no ? no->h : 0; }

    void atualizar(NoAVL* no) { no->h = 1 + max(alt(no->esq), alt(no->dir)); }

    int fator(NoAVL* no) { return alt(no->esq) - alt(no->dir); }

    NoAVL* rotDir(NoAVL* y) {
        NoAVL* x = y->esq;
        y->esq = x->dir;
        x->dir = y;
        atualizar(y);
        atualizar(x);
        rotacoes++;
        return x;
    }

    NoAVL* rotEsq(NoAVL* x) {
        NoAVL* y = x->dir;
        x->dir = y->esq;
        y->esq = x;
        atualizar(x);
        atualizar(y);
        rotacoes++;
        return y;
    }

    NoAVL* balancear(NoAVL* no) {
        atualizar(no);
        int f = fator(no);
        if (f > 1) {
            if (fator(no->esq) < 0) no->esq = rotEsq(no->esq);
            return rotDir(no);
        }
        if (f < -1) {
            if (fator(no->dir) > 0) no->dir = rotDir(no->dir);
            return rotEsq(no);
        }
        return no;
    }

    NoAVL* inserirRec(NoAVL* no, int x, bool& ok) {
        if (!no) {
            ok = true;
            return new NoAVL(x);
        }
        if (x < no->chave) no->esq = inserirRec(no->esq, x, ok);
        else if (x > no->chave) no->dir = inserirRec(no->dir, x, ok);
        else {
            ok = false;
            return no;
        }
        return balancear(no);
    }

    NoAVL* menor(NoAVL* no) {
        while (no->esq) no = no->esq;
        return no;
    }

    NoAVL* removerRec(NoAVL* no, int x, bool& ok) {
        if (!no) {
            ok = false;
            return nullptr;
        }
        if (x < no->chave) no->esq = removerRec(no->esq, x, ok);
        else if (x > no->chave) no->dir = removerRec(no->dir, x, ok);
        else {
            ok = true;
            if (!no->esq || !no->dir) {
                NoAVL* filho = no->esq ? no->esq : no->dir;
                delete no;
                return filho;
            }
            NoAVL* s = menor(no->dir);
            no->chave = s->chave;
            bool tmp;
            no->dir = removerRec(no->dir, s->chave, tmp);
        }
        return balancear(no);
    }

    void emOrdemRec(NoAVL* no, vector<int>& v) {
        if (!no) return;
        emOrdemRec(no->esq, v);
        v.push_back(no->chave);
        emOrdemRec(no->dir, v);
    }

    void imprimirRec(NoAVL* no, int nivel) {
        if (!no) return;
        imprimirRec(no->dir, nivel + 1);
        cout << string(nivel * 6, ' ') << no->chave << " (fb=" << fator(no) << ")\n";
        imprimirRec(no->esq, nivel + 1);
    }

    int validarRec(NoAVL* no, long long lo, long long hi, bool& ok) {
        if (!no) return 0;
        if (no->chave <= lo || no->chave >= hi) ok = false;
        int a = validarRec(no->esq, lo, no->chave, ok);
        int b = validarRec(no->dir, no->chave, hi, ok);
        if (abs(a - b) > 1) ok = false;
        int h = 1 + max(a, b);
        if (h != no->h) ok = false;
        return h;
    }

    void destruir(NoAVL* no) {
        if (!no) return;
        destruir(no->esq);
        destruir(no->dir);
        delete no;
    }

public:
    ~AVL() { destruir(raiz); }

    bool inserir(int x) {
        bool ok;
        raiz = inserirRec(raiz, x, ok);
        if (ok) n++;
        return ok;
    }

    bool remover(int x) {
        bool ok;
        raiz = removerRec(raiz, x, ok);
        if (ok) n--;
        return ok;
    }

    bool buscar(int x) {
        NoAVL* no = raiz;
        while (no) {
            if (x == no->chave) return true;
            no = x < no->chave ? no->esq : no->dir;
        }
        return false;
    }

    void emOrdem(vector<int>& v) { emOrdemRec(raiz, v); }

    void imprimir() {
        if (!raiz) {
            cout << "(arvore vazia)\n";
            return;
        }
        imprimirRec(raiz, 0);
    }

    int altura() { return alt(raiz); }

    int tamanho() { return n; }

    bool validar() {
        bool ok = true;
        validarRec(raiz, -2147483648LL - 1, 2147483647LL + 1, ok);
        return ok;
    }

    const char* nome() { return "AVL"; }
};
