

#include "ListNode.h"
#include "SymbolTable.h"
#include <iostream>          
#include <wchar.h>
#include <string>

using namespace std;

class SymbolTableLD: public SymbolTable	{

	long size;
	ListNode* start;

public:

	SymbolTableLD(){
		this->size = 0;
		start = NULL;
	}

	~SymbolTableLD(){

		ListNode* temp;
		while (start != NULL){
			temp = start;
			start = start->getNext();
			delete temp;
		}

	}

	long getSize(){
		return this->size;
	}

	void insere (wstring chave, long valor){

		ListNode* temp = start;
		while (temp != NULL){
			if (temp->getKey() == chave){
				temp->setVal(temp->getVal() + valor);
				return;
			}
			temp = temp->getNext();
		}

		temp = new ListNode(chave, valor);
		temp->setNext(start);
		this->start = temp;
		this->size = this->size + 1;
		return;
	}

	long devolve(wstring chave){

		ListNode* temp = start;

		while (temp != NULL){
			if (temp->getKey() == chave){
				return (temp->getVal());
			}
			temp = temp->getNext();
		}
		//wcout << L"Chave nao encontrada." << endl;
		return(-1);

	}

	void remove(wstring chave){

		ListNode* temp = this->start;
		ListNode* temp2;

		if (this->start->getKey() == chave){
			start = start->getNext();
			delete temp;
			this->size = this->size - 1;
			return;
		}

		if (start->getNext() == NULL){
			//wcout << L"Chave nao encontrada" << endl;
			return;
		}

		temp2 = temp;
		temp = temp->getNext();

		while (temp != NULL){
			if ((temp->getKey()) == chave){
				temp2->setNext(temp->getNext());
				delete temp;
				this->size = this->size - 1;
				return;
			}
			temp2 = temp;
			temp = temp->getNext();
		}

		//wcout << L"Chave nao encontrada" << endl;
		return;
	}

	long rank(wstring chave){

		ListNode* temp = start;
		long count = 0;
		while (temp != NULL){
			if (temp->getKey() < chave){
				count++;
			}
			temp = temp->getNext();
		}
		return (count);
	}


	wstring select(long given_rank){

		ListNode* temp = start;
		long cur_rank;
		while (temp != NULL){
			cur_rank = rank(temp->getKey());
			if (cur_rank == given_rank){
				return (temp->getKey());
			}
			temp = temp->getNext();
		}
		return (L"");
	}
	void print(){

		ListNode* temp = this->start;

		while (temp != NULL){
			wcout << "Chave " << temp->getKey() << " tem valor " << temp->getVal() << ". ";
			temp = temp->getNext();
		}
		return;
	}
};


//EVAL: DONE