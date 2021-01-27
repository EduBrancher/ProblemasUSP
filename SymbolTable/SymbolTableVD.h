

#include "Node.h"
#include "SymbolTable.h"
#include <iostream>          
#include <wchar.h>
#include <string>

using namespace std;

class SymbolTableVD: public SymbolTable {

	long size;
	Node* array;
	long cur_size;

public:

	SymbolTableVD(long size){

		this->size = size;
		this->array = new Node[this->size];
		this->cur_size = 0;

	}

	~SymbolTableVD(){
		delete[] this->array;
	}
	void incSize(){
		this->cur_size = this->cur_size + 1;
	}
	long getCurSize(){
		return this->cur_size;
	}
	long getSize(){
		return this->cur_size;
	}

	void setTable(Node* table){
		this->array = table;
	}

	void resize(){

		Node* new_table = new Node[this->size*2];
		Node* old_table = this->array;
		for (int i = 0; i < this->size; i++){
			new_table[i] = this->array[i];
		}
		this->setTable(new_table);
		delete[] old_table; 
	}

	void insere (wstring chave, long valor){

		for (int i = 0; i < this->cur_size; i++){
			if (this->array[i].getKey() == chave){	//ja presente
				this->array[i].addVal(valor);
				return;
			}
		}
		this->array[this->cur_size] = Node(chave, valor);
		incSize();
		if (this->getCurSize() == this->getSize()){
			this->resize();
		}
 	
	} // pior caso linear

	long devolve(wstring chave){ //retorna -1 se nao encontrado

		for (int i = 0; i < this->cur_size; i++){
			if (this->array[i].getKey() == chave){
				return (this->array[i].getVal());
			}
		}
		return (-1);
	} // pior caso linear

	void remove(wstring chave){

		wstring nula = L"";
		for (int i = 0; i < this->cur_size; i++){
			if (this->array[i].getKey() == chave){
				this->array[i].setKey(nula);
				this->array[i].setVal(0);
				return;
			}
		}
		//wcout << L"O objeto a ser removido não foi encontrado." << endl;
		return;
	} // pior caso linear

	long rank(wstring chave){ //numero de chaves menores que chave.

		long count = 0;

		for (int i = 0; i < this->cur_size; i++){
			if (this->array[i].getKey() < chave){
				count++;
			}
		}
		return count;
	}

	wstring select(long given_rank){

		for (int i = 0; i < this->cur_size; i++){
			long cur_rank = rank(this->array[i].getKey());
			if (cur_rank == given_rank){
				return(this->array[i].getKey());
			}
		}
		//wcout << L"A key with the requested rank was not found." << endl;
		return (L"");
	}

	void print(){
		for (int i = 0; i < this->size; i++){
			if (this->array[i].getKey() == L""){
				continue;
			}
			wcout << "Chave " << this->array[i].getKey() << " tem valor " << this->array[i].getVal() << ". ";
		}
		return;
	}

};

///EVAL: DONE