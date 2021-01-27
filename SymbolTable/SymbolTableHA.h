#include <iostream>          
#include <wchar.h>
#include <string>
#include "HashNode.h"
#include "SymbolTable.h"

using namespace std;

class SymbolTableHA: public SymbolTable{

	long size;
	long cur_size;
	HashNode** HashVector;


public:
	SymbolTableHA(){

	}
	SymbolTableHA(long size){

		this->size = size;
		this->cur_size = 0;
		HashVector = new HashNode*[size];
		for (long i = 0; i < size; i++){
			HashVector[i] = NULL;
		}

	}
	~SymbolTableHA(){

		HashNode* temp;
		HashNode* temp2;
		for (int i = 0; i < size; i++){
			temp = this->HashVector[i];
			while (temp != NULL){
				temp2 = temp;
				temp = temp->getNext();
				delete temp2;
			}
		}
		delete[] this->HashVector;
		return;
		
	}
	void appendNode(HashNode* head_node, HashNode* new_node){
		//coloca o HashNode new no fim da lista encabeçada por node.
		HashNode* temp = head_node;
		HashNode* prev;

		while (temp != NULL){
			if (temp->getKey() == new_node->getKey()){
				temp->setVal(temp->getVal() +  new_node->getVal());
				delete new_node;
				return;
			}
			prev = temp;
			temp = temp->getNext();
		}
		prev->setNext(new_node);
		this->cur_size = this->cur_size + 1;
		head_node->incrementSize();
		}

	
	long getSize(){
		return this->cur_size;
	}
	void insere(wstring chave, long valor){

		long hash = Hash(chave);

		if (this->HashVector[hash] == NULL){
			this->HashVector[hash](new HashNode(chave, valor, NULL));
			this->cur_size = this->cur_size + 1;
		}
		else if (this->HashVector[hash] != NULL){
			appendNode(this->HashVector[hash], new HashNode(chave, valor, NULL));
		}

	}
	void remove(wstring chave){
		
		long hash = Hash(chave);
		HashNode* temp;
		HashNode* prev;	

		temp = this->HashVector[hash];
		if (temp->getNext() == NULL){
			wcout << L"Palavra não encontrada." << endl;
			return;
		}
		if (temp->getKey() == chave){
			HashVector[hash] = temp->getNext();
			delete temp;
			this->cur_size = this->cur_size - 1;
			return;
		}
		while (temp != NULL){
			prev = temp;
			temp = temp->getNext();
			if (temp->getKey() == chave){
				prev->setNext(temp->getNext());
				delete temp;
				this->cur_size = this->cur_size - 1;
				return;
			}
		}
		wcout << L"remove nao encontrou a chave pedida." << endl;
	
	}
	long devolve(wstring chave){

		long hash = Hash(chave);
		HashNode* temp;

		if (this->HashVector[hash] == NULL){
			wcout << L"devolve chamado em hash NULL, erro!" << endl;
		}
		temp = this->HashVector[hash];
		while (temp != NULL){
			if (temp->getKey() == chave){
				return temp->getVal();
			}
			else{
				temp = temp->getNext();
			}
		}
		wcout << L"Chave pedida em devolve nao encontrada." << endl;
		return -1;
	}

	long rank(wstring chave){
	
		long rank = 0;
		long val = devolve(chave);
		HashNode* temp;

		if (val == -1){
			wcout << L"rank pedido em chave que nao esta na lista." << endl;
			return -1;
		}
		for (int i = 0; i < this->size; i++){
			temp = HashVector[i];
			while (temp != NULL){
				if (temp->getKey() < chave){
					rank = rank + 1;
				}
				temp = temp->getNext();
			}
		}
		return rank;
	}

	wstring select(long given_rank){

		HashNode* temp;
		long cur_rank;
		
		for (int i = 0; i < this->size; i++){
			temp = HashVector[i];
			while (temp != NULL){
				cur_rank = rank(temp->getKey());
				if (cur_rank == given_rank){
					return temp->getKey();
				}
				temp = temp->getNext();
			}
		}
		wcout << L"Nao ha chave com o rank pedido na tabela." << endl;
		return L"";
	}
	long Hash(wstring chave){

		//obtida do website do Prof. Paulo Feofiloff, https://www.ime.usp.br/~pf/estruturas-de-dados/aulas/st-hash.html

		long hash = 0;
		long M = this->size;

		for (unsigned int i = 0; i < chave.length(); i++){
			hash = (31 * hash + chave.at(i)) % M;
		}

		return hash;

	}
	void print(){

		for (int i = 0; i < this->size; i++){
			if (HashVector[i] == NULL){
				wcout << L" [] ";
			}
			else{
				wcout << "Palavra " << HashVector[i]->getKey() << L" tem valor " << HashVector[i]->getVal() << L". ";
				HashNode* HashPointer = HashVector[i]->getNext();
				while (HashPointer != NULL){
					wcout << "Palavra na lista " << HashPointer->getKey() << L" tem valor " << HashPointer->getVal() << L". ";
					HashPointer = HashPointer->getNext();
				}
				wcout << L"Fim da lista. ";
			}
		}

	}

};