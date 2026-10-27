#pragma once
#include "arvore.hpp"

enum Cor { VERMELHO, PRETO };

struct NoRB {
    int chave;
    Cor cor;
    NoRB* esq;
    NoRB* dir;
    NoRB* pai;
};

class RB : public Arvore {
    NoRB* nil;
    NoRB* raiz;
    int n = 0;

    void pintar(NoRB* no, Cor c) {
        if (no != nil && no->cor != c) {
            no->cor = c;
            recoloracoes++;
        }
    }

    void rotEsq(NoRB* x) {
        NoRB* y = x->dir;
        x->dir = y->esq;
        if (y->esq != nil) y->esq->pai = x;
        y->pai = x->pai;
        if (x->pai == nil) raiz = y;
        else if (x == x->pai->esq) x->pai->esq = y;
        else x->pai->dir = y;
        y->esq = x;
        x->pai = y;
        rotacoes++;
    }

    void rotDir(NoRB* x) {
        NoRB* y = x->esq;
        x->esq = y->dir;
        if (y->dir != nil) y->dir->pai = x;
        y->pai = x->pai;
        if (x->pai == nil) raiz = y;
        else if (x == x->pai->dir) x->pai->dir = y;
        else x->pai->esq = y;
        y->dir = x;
        x->pai = y;
        rotacoes++;
    }

    void consertarInsercao(NoRB* z) {
        while (z->pai->cor == VERMELHO) {
            if (z->pai == z->pai->pai->esq) {
                NoRB* tio = z->pai->pai->dir;
                if (tio->cor == VERMELHO) {
                    pintar(z->pai, PRETO);
                    pintar(tio, PRETO);
                    pintar(z->pai->pai, VERMELHO);
                    z = z->pai->pai;
                } else {
                    if (z == z->pai->dir) {
                        z = z->pai;
                        rotEsq(z);
                    }
                    pintar(z->pai, PRETO);
                    pintar(z->pai->pai, VERMELHO);
                    rotDir(z->pai->pai);
                }
            } else {
                NoRB* tio = z->pai->pai->esq;
                if (tio->cor == VERMELHO) {
                    pintar(z->pai, PRETO);
                    pintar(tio, PRETO);
                    pintar(z->pai->pai, VERMELHO);
                    z = z->pai->pai;
                } else {
                    if (z == z->pai->esq) {
                        z = z->pai;
                        rotDir(z);
                    }
                    pintar(z->pai, PRETO);
                    pintar(z->pai->pai, VERMELHO);
                    rotEsq(z->pai->pai);
                }
            }
        }
        pintar(raiz, PRETO);
    }

    void transplantar(NoRB* u, NoRB* v) {
        if (u->pai == nil) raiz = v;
        else if (u == u->pai->esq) u->pai->esq = v;
        else u->pai->dir = v;
        v->pai = u->pai;
    }

    NoRB* minimo(NoRB* x) {
        while (x->esq != nil) x = x->esq;
        return x;
    }

    NoRB* buscarNo(int k) {
        NoRB* x = raiz;
        while (x != nil && x->chave != k) x = k < x->chave ? x->esq : x->dir;
        return x;
    }

    void consertarRemocao(NoRB* x) {
        while (x != raiz && x->cor == PRETO) {
            if (x == x->pai->esq) {
                NoRB* w = x->pai->dir;
                if (w->cor == VERMELHO) {
                    pintar(w, PRETO);
                    pintar(x->pai, VERMELHO);
                    rotEsq(x->pai);
                    w = x->pai->dir;
                }
                if (w->esq->cor == PRETO && w->dir->cor == PRETO) {
                    pintar(w, VERMELHO);
                    x = x->pai;
                } else {
                    if (w->dir->cor == PRETO) {
                        pintar(w->esq, PRETO);
                        pintar(w, VERMELHO);
                        rotDir(w);
                        w = x->pai->dir;
                    }
                    pintar(w, x->pai->cor);
                    pintar(x->pai, PRETO);
                    pintar(w->dir, PRETO);
                    rotEsq(x->pai);
                    x = raiz;
                }
            } else {
                NoRB* w = x->pai->esq;
                if (w->cor == VERMELHO) {
                    pintar(w, PRETO);
                    pintar(x->pai, VERMELHO);
                    rotDir(x->pai);
                    w = x->pai->esq;
                }
                if (w->dir->cor == PRETO && w->esq->cor == PRETO) {
                    pintar(w, VERMELHO);
                    x = x->pai;
                } else {
                    if (w->esq->cor == PRETO) {
                        pintar(w->dir, PRETO);
                        pintar(w, VERMELHO);
                        rotEsq(w);
                        w = x->pai->esq;
                    }
                    pintar(w, x->pai->cor);
                    pintar(x->pai, PRETO);
                    pintar(w->esq, PRETO);
                    rotDir(x->pai);
                    x = raiz;
                }
            }
        }
        pintar(x, PRETO);
    }

    void emOrdemRec(NoRB* no, vector<int>& v) {
        if (no == nil) return;
        emOrdemRec(no->esq, v);
        v.push_back(no->chave);
        emOrdemRec(no->dir, v);
    }

    void imprimirRec(NoRB* no, int nivel) {
        if (no == nil) return;
        imprimirRec(no->dir, nivel + 1);
        cout << string(nivel * 6, ' ') << no->chave << (no->cor == VERMELHO ? "(V)" : "(P)") << "\n";
        imprimirRec(no->esq, nivel + 1);
    }

    int alturaRec(NoRB* no) {
        if (no == nil) return 0;
        return 1 + max(alturaRec(no->esq), alturaRec(no->dir));
    }

    int validarRec(NoRB* no, long long lo, long long hi, bool& ok) {
        if (no == nil) return 1;
        if (no->chave <= lo || no->chave >= hi) ok = false;
        if (no->cor == VERMELHO && (no->esq->cor == VERMELHO || no->dir->cor == VERMELHO)) ok = false;
        int a = validarRec(no->esq, lo, no->chave, ok);
        int b = validarRec(no->dir, no->chave, hi, ok);
        if (a != b) ok = false;
        return a + (no->cor == PRETO ? 1 : 0);
    }

    void destruir(NoRB* no) {
        if (no == nil) return;
        destruir(no->esq);
        destruir(no->dir);
        delete no;
    }

public:
    RB() {
        nil = new NoRB;
        nil->chave = 0;
        nil->cor = PRETO;
        nil->esq = nil->dir = nil->pai = nil;
        raiz = nil;
    }

    ~RB() {
        destruir(raiz);
        delete nil;
    }

    bool inserir(int k) {
        NoRB* y = nil;
        NoRB* x = raiz;
        while (x != nil) {
            y = x;
            if (k < x->chave) x = x->esq;
            else if (k > x->chave) x = x->dir;
            else return false;
        }
        NoRB* z = new NoRB{k, VERMELHO, nil, nil, y};
        if (y == nil) raiz = z;
        else if (k < y->chave) y->esq = z;
        else y->dir = z;
        n++;
        consertarInsercao(z);
        return true;
    }

    bool remover(int k) {
        NoRB* z = buscarNo(k);
        if (z == nil) return false;
        NoRB* y = z;
        Cor corOriginal = y->cor;
        NoRB* x;
        if (z->esq == nil) {
            x = z->dir;
            transplantar(z, z->dir);
        } else if (z->dir == nil) {
            x = z->esq;
            transplantar(z, z->esq);
        } else {
            y = minimo(z->dir);
            corOriginal = y->cor;
            x = y->dir;
            if (y->pai == z) {
                x->pai = y;
            } else {
                transplantar(y, y->dir);
                y->dir = z->dir;
                y->dir->pai = y;
            }
            transplantar(z, y);
            y->esq = z->esq;
            y->esq->pai = y;
            pintar(y, z->cor);
        }
        delete z;
        n--;
        if (corOriginal == PRETO) consertarRemocao(x);
        return true;
    }

    bool buscar(int k) { return buscarNo(k) != nil; }

    void emOrdem(vector<int>& v) { emOrdemRec(raiz, v); }

    void imprimir() {
        if (raiz == nil) {
            cout << "(arvore vazia)\n";
            return;
        }
        imprimirRec(raiz, 0);
    }

    int altura() { return alturaRec(raiz); }

    int tamanho() { return n; }

    bool validar() {
        bool ok = true;
        if (raiz->cor != PRETO) ok = false;
        validarRec(raiz, -2147483648LL - 1, 2147483647LL + 1, ok);
        return ok;
    }

    const char* nome() { return "Rubro-Negra"; }
};
