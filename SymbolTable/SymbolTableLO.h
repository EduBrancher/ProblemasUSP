#include "ListNode.h"
#include "SymbolTable.h"
#include <iostream>          
#include <wchar.h>
#include <string>

using namespace std;

class SymbolTableLO: public SymbolTable	{

	long size;
	ListNode* start;

public:

	SymbolTableLO(){
		this->size = 0;
		start = NULL;
	}

	~SymbolTableLO(){

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

		ListNode* temp = NULL;
		ListNode* temp2 = NULL;

		if (start == NULL){
			temp = new ListNode(chave, valor);
			temp->setNext(NULL);
			this->start = temp;
			this->size = this->size + valor;
			return;
		}
		temp2 = start; //temp2 != NULL (start != NULL)
		
		while (temp2 != NULL){
			if (temp2->getKey() > chave){
				if (temp == NULL){  //inserção no começo
					temp = new ListNode(chave, valor);
					temp->setNext(temp2);
					this->start = temp;
					this->size++;
					return;
				}
				else { //temp != NULL
					ListNode* new_node = new ListNode(chave, valor);
					temp->setNext(new_node);
					new_node->setNext(temp2);
					this->size++;
					return;
				}
			}
			else if (temp2->getKey() == chave){
				temp2->setVal(temp2->getVal() + valor);
				return;
			}
			//else{}
			temp = temp2;
			temp2 = temp2->getNext();

		}
		//todas as chaves na lista eram menores que chave, temp2 vale NULL
		temp2 = new ListNode(chave, valor);
		temp2->setNext(NULL); //fim da lista
		temp->setNext(temp2);
		this->size++;
		return;

	}

	long devolve(wstring chave){

		ListNode* temp = start;

		while (temp != NULL && temp->getKey() <= chave){
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

		if (temp == NULL){
			//wcout << L"Chave nao encontrada, lista vazia" << endl;
			return;
		}

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

		while (temp != NULL && temp->getKey() <= chave){
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
		while (temp->getKey() < chave){
			count++;
			temp = temp->getNext();
		}
		return (count);
	}


	wstring select(long given_rank){

		ListNode* temp = start;
		long cur_rank = 0;
		while (temp != NULL && cur_rank <= given_rank){
			cur_rank = rank(temp->getKey());
			if (cur_rank == given_rank){
				return (temp->getKey());
			}
			temp = temp->getNext();
		}
		if (temp == NULL){
			//return (L"err");
		}
		return (L"err2");
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

///EVAL: DONE

