#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Arvore {
public:
    long long rotacoes = 0;
    long long recoloracoes = 0;
    virtual ~Arvore() {}
    virtual bool inserir(int x) = 0;
    virtual bool remover(int x) = 0;
    virtual bool buscar(int x) = 0;
    virtual void emOrdem(vector<int>& v) = 0;
    virtual void imprimir() = 0;
    virtual int altura() = 0;
    virtual int tamanho() = 0;
    virtual bool validar() = 0;
    virtual const char* nome() = 0;
};
