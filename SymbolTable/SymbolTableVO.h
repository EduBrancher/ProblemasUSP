

#include "Node.h"
#include "SymbolTable.h"
#include <iostream>          
#include <wchar.h>
#include <string>

using namespace std;

class SymbolTableVO: public SymbolTable {

	long size;
	Node* array;
	long cur_size;

public:

	SymbolTableVO(long size){

		this->size = size;
		this->array = new Node[this->size];
		for (int i = 0; i < this->size; i++){
			this->array[i] = Node();
		}
		this->cur_size = 0;

	}

	~SymbolTableVO(){
		delete[] this->array;
	}

	long getSize(){
		return this->cur_size;
	}
	
	Node push(Node entry, long index){ //coloca o node entry na posição do vetor index. retorna o node que estava lá.

		Node temp(this->array[index].getKey(),this->array[index].getVal());

		this->array[index].setKey(entry.getKey());
		this->array[index].setVal(entry.getVal());
		return temp;
	}

	void insere (wstring chave, long valor){	

		long i = 0;
		Node temp;

		while (i < this->cur_size && this->array[i].getKey() <= chave){
			if (this->array[i].getKey() == chave){	//ja presente
				this->array[i].addVal(valor);
				return;
			}
			i = i + 1;
		}
		temp = this->push(Node(chave, valor), i);
		
		this->cur_size = this->cur_size + 1;

		for (int j = i + 1; j <= this->cur_size; j++){
			temp = this->push(temp, j);
			if (temp.getVal() == 0){
				return;
			}
		}
	} // pior caso linear

	long devolve(wstring chave){ //retorna -1 se nao encontrado //busca binaria

		long start = 0;
		long end = this->cur_size;
		long mid = (start + end)/2;
		
		while (start <= end){
			if (this->array[mid].getKey() == chave){
				return (this->array[mid].getVal());
			}
			else if (this->array[mid].getKey() < chave){
				start = mid + 1;
			}
			else if (this->array[mid].getKey() > chave){
				end = mid - 1;
			}
			mid = (start + end)/2;
		}
		//wcout << L"Chave pedida nao encontrada." << endl;
		return (-1);
	} // pior caso linear

	void remove(wstring chave){

		long start = 0;
		long end = this->cur_size;
		long mid = (start + end)/2;
		
		while (start <= end){
			if (this->array[mid].getKey() == chave){
				for (int j = mid; j < this->cur_size; j++){
					this->array[j].setKey(this->array[j+1].getKey());
					this->array[j].setVal(this->array[j+1].getVal());
				}
				this->cur_size = this->cur_size - 1;
				return;
			}
			else if (this->array[mid].getKey() < chave){
				start = mid + 1;
			}
			else if (this->array[mid].getKey() > chave){
				end = mid - 1;
			}
			mid = (start + end)/2;
		}
		//wcout << L"Chave pedida nao encontrada." << endl;
		return;

	}

	long rank(wstring chave){ //numero de chaves menores que chave. //busca binaria

		long start = 0;
		long end = this->cur_size;
		long mid = (start + end)/2;
		
		while (start <= end){
			if (this->array[mid].getKey() == chave){
				return (mid);
			}
			else if (this->array[mid].getKey() < chave){
				start = mid + 1;
			}
			else if (this->array[mid].getKey() > chave){
				end = mid - 1;
			}
			mid = (start + end)/2;
		}
		//wcout << L"Chave pedida nao encontrada." << endl;
		return (-1);

	}

	wstring select(long given_rank){ //chave tal que given_rank chaves sao menores que ela


		if (given_rank < 0 || given_rank >= cur_size){
			//wcout << L"Numero de chave pedida esta fora da tabela." << endl;
			return (L"ERROR");
		}
		else {
			return ((this->array[given_rank]).getKey());
		}
	}
	void print(){
		for (int i = 0; i < this->cur_size; i++){
			wcout << "Chave " << this->array[i].getKey() << " tem valor " << this->array[i].getVal() << ". ";
		}
		return;
	}


};


//EVAL: DONE