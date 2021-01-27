#include <iostream>          
#include <wchar.h>
#include <string>


using namespace std;

class RedBlackNode {

	wstring chave;
	long valor;
	long tree_size;
	long priority;
	long colour; // 0 == preto, 1 == vermelho, no vermelho tem filhos pretos
	RedBlackNode* left_child;
	RedBlackNode* right_child;
	RedBlackNode* parent;

public:

	RedBlackNode(wstring chave, long valor, long colour){
		this->chave = chave;
		this->valor = valor;
		this->colour = colour;
		this->left_child = NULL;
		this->right_child = NULL;
		this->parent = NULL;
		this->tree_size = 1;
	}

	RedBlackNode (){
		this->chave = L"";
		this->valor = 0;
		this->colour = 1;
		this->left_child = NULL;
		this->right_child = NULL;
		this->parent = NULL;
		this->tree_size = 1;
	}

	~RedBlackNode(){
		delete this->chave;
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

	RedBlackNode* getLeft(){
		return this->left_child;
	}

	void setLeft(RedBlackNode* new_left){
		this->left_child = new_left;
	}

	RedBlackNode* getRight(){
		return this->right_child;
	}

	void setRight(RedBlackNode* new_right){
		this->right_child = new_right;
	}

	RedBlackNode* getParent(){
		return this->parent;
	}

	void setParent(RedBlackNode* new_parent){
		this->parent = new_parent;
	}

	long getSize(){
		return this->tree_size;
	}

	void setSize(long num){
		this->tree_size = num;
	}

	void setColour(long colour){
		this->colour = colour;
	}

	long getColour(){
		return (this->colour);
	}


};