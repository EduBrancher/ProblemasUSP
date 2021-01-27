#ifndef SymbolTable_H
#define SymbolTable_H


#include <string>

using namespace std;

class SymbolTable{


public:

	virtual void insere(wstring chave, long val) = 0;
	virtual long devolve(wstring chave) = 0;
	virtual void remove(wstring chave) = 0;
	virtual long rank(wstring chave) = 0;
	virtual wstring select(long rank) = 0;
	virtual void print() = 0;
	virtual long getSize() = 0;

	virtual ~SymbolTable(){
		
	}

};


#endif