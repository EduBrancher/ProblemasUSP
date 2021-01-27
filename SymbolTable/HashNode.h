#include <iostream>          
#include <wchar.h>
#include <string>

using namespace std;

class HashNode{

	wstring key;
	long value;
	HashNode* next;
	long list_size;

public:

	HashNode(){
		this->key = L"";
		this->value = 0;
		this->next = NULL;
		this->list_size = 1;
	}
	HashNode(wstring chave, long val, HashNode* next){
		this->key = chave;
		this->value = val;
		this->next = next;
		this->list_size = 1;
	}
	~HashNode(){

	}
	void setKey(wstring chave){
		this->key = chave;
	}
	void setVal(long val){
		this->value = val;
	}
	void setNext(HashNode* next){
		this->next = next;
	}
	wstring getKey(){
		return this->key;
	}
	long getVal(){
		return this->value;
	}
	HashNode* getNext(){
		//devolve o próximo HashNode na lista.
		return this->next;
	}
	long getSize(){
		//devolve o tamanho da lista encabeçada por node (minimo 1)
		return this->list_size;
	}
	void setSize(long size){
		this->list_size = size;
	}
	void incrementSize(){
		this->setSize(this->getSize() + 1);
	}


};