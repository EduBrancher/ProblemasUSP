#include <iostream>          
#include <wchar.h>
#include <string>
#include <random>
#include <climits>

using namespace std;

class TreapNode {

	wstring chave;
	long valor;
	long tree_size; 
	long priority;
	TreapNode* left_child;
	TreapNode* right_child;
	TreapNode* parent;

public:

	TreapNode(wstring chave, long valor, long priority){
		this->chave = chave;
		this->valor = valor;
		this->priority = priority;
		this->left_child = NULL;
		this->right_child = NULL;
		this->parent = NULL;
		this->tree_size = 1;
	}

	TreapNode (){
		this->chave = L"";
		this->valor = 0;
		this->priority = 0;
		this->left_child = NULL;
		this->right_child = NULL;
		this->parent = NULL;
		this->tree_size = 1;
	}

	~TreapNode(){

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

	TreapNode* getLeft(){
		return this->left_child;
	}

	void setLeft(TreapNode* new_left){
		this->left_child = new_left;
	}

	TreapNode* getRight(){
		return this->right_child;
	}

	void setRight(TreapNode* new_right){
		this->right_child = new_right;
	}

	TreapNode* getParent(){
		return this->parent;
	}

	void setParent(TreapNode* new_parent){
		this->parent = new_parent;
	}

	long getSize(){
		return this->tree_size;
	}

	void setSize(long num){
		this->tree_size = num;
	}

	void setPriority(long priority){
		this->priority = priority;
	}

	long getPriority(){
		return (this->priority);
	}


};