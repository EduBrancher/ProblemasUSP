#include <iostream>          
#include <wchar.h>
#include <string>
#include "HashNode.h"

using namespace std;

class SymbolTableHA{

	long size;
	HashNode** HashVector;


public:
	SymbolTableHA(){

	}
	SymbolTableHA(long size){

		this->size = size;
		HashVector = new HashNode*[size];
		for (int i = 0; i < size; i++){
			HashVector[i] = NULL;
		}

	}
	~SymbolTableHA(){
		
	}
	void appendNode(HashNode* head_node, HashNode* new_node){
		//coloca o HashNode new no fim da lista encabeçada por node.
		HashNode* temp = head_node;
		HashNode* prev;
		if (temp == NULL){
			cout << "AppendNode() chamado em head NULL, erro" << endl;
			return;
		}

		else{
			while (temp != NULL){
				if (temp->getKey().compare(new_node->getKey()) == 0){
					temp->setVal(temp->getVal() +  new_node->getVal());
					return;
				}
				prev = temp;
				temp = temp->getNext();
			}
			prev->setNext(new_node);
			head_node->incrementSize();
		}

	}
	long getSize(){
		return this->size;
	}
	void reHash(HashNode* node){
		//faz o rehashing de todos os nodes na lista encabeçada por node, com node incluso (para redimensionamento da symbol table). 
		//Requer uma função de hashing, portanto deve ser da Symbol Table. Não implementado.
	}
	void insert(wstring chave, long valor){

		long hash = Hash(chave);

		if (this->HashVector[hash] == NULL){
			this->HashVector[hash] = new HashNode(chave, valor, NULL);
		}
		else if (this->HashVector[hash] != NULL){
			appendNode(this->HashVector[hash], new HashNode(chave, valor, NULL));
		}

	}
	void remove(wstring chave){
		
		long hash = Hash(chave);
		HashNode* temp;
		HashNode* prev;

		if (this->HashVector[hash] == NULL){
			cout << "remove chamado em hash NULL, erro!" << endl;
		}
		else if (this->HashVector[hash] != NULL){
			temp = this->HashVector[hash];
			while (temp != NULL){
				if (temp->getKey().compare(chave) == 0){
					prev->setNext(temp->getNext());
					delete temp;
					return;
				}
				else{
					prev = temp;
					temp = temp->getNext();
				}
			}
			cout << "remove nao encontrou a chave pedida." << endl;
		}

	}
	long devolve(wstring chave){

		long hash = Hash(chave);
		HashNode* temp;

		if (this->HashVector[hash] == NULL){
			cout << "devolve chamado em hash NULL, erro!" << endl;
		}
		temp = this->HashVector[hash];
		while (temp != NULL){
			if (temp->getKey().compare(chave) == 0){
				return temp->getVal();
			}
			else{
				temp = temp->getNext();
			}
		}
		cout << "Chave pedida em devolve nao encontrada." << endl;
		return -1;
	}

	long rank(wstring chave){
	
		long rank = 0;
		long val = devolve(chave);
		HashNode* temp;

		if (val == -1){
			cout << "rank pedido em chave que nao esta na lista." << endl;
			return -1;
		}
		for (int i = 0; i < this->size; i++){
			temp = HashVector[i];
			while (temp != NULL){
				if (temp->getKey().compare(chave) < 0){
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
		cout << "Nao ha chave com o rank pedido na tabela." << endl;
	}
	long Hash(wstring chave){

		//obtida do website do Prof. Paulo Feofiloff, https://www.ime.usp.br/~pf/estruturas-de-dados/aulas/st-hash.html

		long hash = 0;
		long M = this->getSize();

		for (int i = 0; i < chave.length(); i++){
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
					wcout << "Palavra na lista" << HashVector[i]->getKey() << L" tem valor " << HashVector[i]->getVal() << L". ";
				}
				wcout << L"Fim da lista. ";
			}
		}

	}

};