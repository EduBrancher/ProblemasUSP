
#ifndef Node_H
#define Node_H

#include <iostream>          
#include <wchar.h>
#include <string>

using namespace std;

class Node {

	wstring chave;
	long valor;

public:

	Node(wstring chave, long valor){
		this->chave = chave;
		this->valor = valor;
	}

	Node (){
		this->chave = L"";
		this->valor = 0;
	}

	~Node(){

	}

	wstring getKey(){
		return this->chave;
	}

	long getVal(){
		return this->valor;
	}

	void setKey(wstring chave){
		this->chave = chave;
	}

	void setVal(long valor){
		this->valor = valor;
	}

	void addVal(long val){
		this->valor = this->valor + val;
	}


};

#endif