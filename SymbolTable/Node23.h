#include <iostream>          
#include <wchar.h>
#include <string>


using namespace std;

class Node23 {

	wstring chaveEsq;
	wstring chaveDir;
	long valorEsq;
	long valorDir;
	long tree_size;
	long priority;
	long state; // 2 no ou 3 no
	Node23* left_child;
	Node23* mid_child;
	Node23* right_child;
	Node23* parent;

public:

	Node23(wstring chave, long valor){
		this->chaveEsq = chave;
		this->valorEsq = valor;
		this->state = 2;
		this->left_child = NULL;
		this->mid_child = NULL;
		this->right_child = NULL;
		this->parent = NULL;
		this->tree_size = 1;
	}

	Node23 (){
		this->chaveEsq = L"";
		this->valorEsq = 0;
		this->state = 2;
		this->left_child = NULL;
		this->right_child = NULL;
		this->parent = NULL;
		this->tree_size = 1;
	}

	~Node23(){

	}

	wstring getKeyEsq(){
		return this->chaveEsq;
	}

	wstring getKeyDir(){
		return this->chaveDir;
	}

	long getValEsq(){
		return this->valorEsq;
	}

	long getValDir(){
		return this->valorDir;
	}

	void setKeyEsq(wstring chave){
		this->chaveEsq = chave;
	}

	void setKeyDir(wstring chave){
		this->chaveDir = chave;
	}

	void setValEsq(long valor){
		this->valorEsq = valor;
	}

	void setValDir(long valor){
		this->valorDir = valor;
	}


	Node23* getLeft(){
		return this->left_child;
	}

	void setLeft(Node23* new_left){
		this->left_child = new_left;
	}

	Node23* getRight(){
		return this->right_child;
	}

	void setRight(Node23* new_right){
		this->right_child = new_right;
	}

	Node23* getMid(){
		return this->mid_child;
	}

	void setMid(Node23* node){
		this->mid_child = node;
	}

	Node23* getParent(){
		return this->parent;
	}

	void setParent(Node23* new_parent){
		this->parent = new_parent;
	}

	long getSize(){
		return this->tree_size;
	}

	void setSize(long num){
		this->tree_size = num;
	}

	void setState(long state){
		this->state = state;
	}

	long getState(){
		return (this->state);
	}

};