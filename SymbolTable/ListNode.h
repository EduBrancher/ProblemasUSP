


#include <iostream>          
#include <wchar.h>
#include <string>

#ifndef ListNode_H
#define ListNode_H

using namespace std;

class ListNode {

	wstring chave;
	long valor;
	ListNode* next;

public:

	ListNode(wstring chave, long valor){
		this->chave = chave;
		this->valor = valor;
		this->next = NULL;
	}

	ListNode (){
		this->chave = L"";
		this->valor = 0;
		this->next = NULL;
	}

	~ListNode(){

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

	ListNode* getNext(){
		return this->next;
	}

	void setNext(ListNode* next){
		this->next = next;
	}


};

#endif