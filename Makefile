all: arvores testes

arvores: src/main.cpp src/avl.hpp src/rb.hpp src/arvore.hpp
	g++ -O2 -std=c++17 -o arvores src/main.cpp

testes: src/testes.cpp src/avl.hpp src/rb.hpp src/arvore.hpp
	g++ -O2 -std=c++17 -o testes src/testes.cpp
