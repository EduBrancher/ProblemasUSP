#include <iostream>          
#include <wchar.h>
#include <string>

using namespace std;

class TreeNode {

	wstring chave;
	long valor;
	long tree_size;
	TreeNode* left_child;
	TreeNode* right_child;
	TreeNode* parent;

public:

	TreeNode(wstring chave, long valor){
		this->chave = chave;
		this->valor = valor;
		this->left_child = NULL;
		this->right_child = NULL;
		this->parent = NULL;
		this->tree_size = 1;
	}

	TreeNode (){
		this->chave = L"";
		this->valor = 0;
		this->left_child = NULL;
		this->right_child = NULL;
		this->parent = NULL;
		this->tree_size = 1;
	}

	~TreeNode(){

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

	TreeNode* getLeft(){
		return this->left_child;
	}

	void setLeft(TreeNode* new_left){
		this->left_child = new_left;
	}

	TreeNode* getRight(){
		return this->right_child;
	}

	void setRight(TreeNode* new_right){
		this->right_child = new_right;
	}

	TreeNode* getParent(){
		return this->parent;
	}

	void setParent(TreeNode* new_parent){
		this->parent = new_parent;
	}

	long getSize(){
		return this->tree_size;
	}

	void setSize(long num){
		this->tree_size = num;
	}


};