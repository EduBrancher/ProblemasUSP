#include "TreapNode.h"
#include "SymbolTable.h"
#include <iostream>          
#include <wchar.h>
#include <string>
#include <random>
#include <climits>


//Foi usada a referencia https://www.geeksforgeeks.org/treap-a-randomized-binary-search-tree/ e suas dependências para estudar o treap, 
//que não consegui fazer de memória. Os códigos dessa seção serão, portanto, parecidos com os da referência, que consultei quando não consegui seguir.

using namespace std;

class TreapQueueNode{

	TreapNode* node;
	TreapQueueNode* next;

public:
	TreapQueueNode(TreapNode* node){
		this->node = node;
		this->next = NULL;
	}
	~TreapQueueNode(){

	}
	void setNext(TreapQueueNode* queue_node){
		this->next = queue_node;
	}
	TreapQueueNode* getNext(){
		return this->next;
	}
	TreapNode* getNode(){
		return this->node;
	}

};

class TreapQueue {

	TreapQueueNode* start;
	TreapQueueNode* end;

public:
	TreapQueue(){
		start = NULL;
		end = NULL;
	}
	~TreapQueue(){

	}
	void Enqueue(TreapNode* recv_node){

		TreapQueueNode* new_node = new TreapQueueNode(recv_node);
		if (QueueEmpty() == 1){
			this->start = new_node;
			this->end = new_node;
		}
		else{
			this->end->setNext(new_node);
			end = new_node;
		}
		return;
	}
	TreapNode* Dequeue(){

		TreapQueueNode* temp = this->start;

		if (QueueEmpty() == 1){
			return NULL;
		}
		if (start == end){
			start = NULL;
			end = NULL;
		}
		else{
			this->start = start->getNext();
		}
		TreapNode* ret = temp->getNode();
		delete temp;
		return ret;

	}
	long QueueEmpty(){
		if (start == NULL && end == NULL){
			return 1;
		}
		else{
			return 0;
		}
	}

};

class SymbolTableTR: public SymbolTable	{

	TreapNode* root; //árvore
	minstd_rand0 g2;
	minstd_rand0 g3;

public:

	SymbolTableTR(string seed){

		this->root = NULL;
		//int a = 4132;
  		//string str(to_string(a));
        //seed_seq seed2 (str.begin(),str.end());
  		string str2 = seed;
  		seed_seq seed3(str2.begin(), str2.end());

  		//this->g2 = minstd_rand0(seed2);
  		this->g3 = minstd_rand0(seed3);


	}
	void DeleteTable(TreapNode* root){

		if (root == NULL){
			return;
		}

		DeleteTable(root->getLeft());
		DeleteTable(root->getRight());
		delete root;
		return;
	}

	~SymbolTableTR(){
		DeleteTable(this->root);
	}

	TreapNode* getRoot(){
		return this->root;
	}

	TreapNode* setRoot(TreapNode* new_root){
		this->root = new_root;
		return (this->root);
	}

	long SubtreeSize(TreapNode* node){
		if (node == NULL){
			return 0;
		}
		else{
			return node->getSize();
		}
	}
	void FixRight(TreapNode* broken, TreapNode* right){
		broken->setRight(right);
		if (right != NULL){
			right->setParent(broken);
		}
	}

	void FixLeft(TreapNode* broken, TreapNode* left){
		broken->setLeft(left);
		if (left != NULL){
			left->setParent(broken);
		}
	}

	void FixSize(TreapNode* node){

		if (node == NULL){
			return;
		}
		node->setSize(SubtreeSize(node->getLeft()) + SubtreeSize(node->getRight()) + 1);

	}

	long GetPrio(TreapNode* node){
		if (node == NULL){
			return -1;
		}
		else{
			return node->getPriority();
		}
	}

	TreapNode* setRoot(wstring chave, long valor){
		long priority = (long)g3();
		this->root = new TreapNode(chave, valor, priority);
		return root;
	}

	long getSize(){
		return SubtreeSize(this->getRoot());
	}

	TreapNode* rotateRight (TreapNode* node){

		if (node == NULL){
			wcout << L"rotateRight() chamada em NULL" << endl;
			return(NULL);
		}

		
		TreapNode* child;
		TreapNode* second_child;

		child = node->getLeft();
		if (child == NULL){
			return node;
		}
		second_child = child->getRight();
		child->setParent(node->getParent());
		child->setRight(node);
		node->setParent(child);
		node->setLeft(second_child);
		node->setSize(SubtreeSize(node->getLeft()) + SubtreeSize(node->getRight()) + 1);
		child->setSize(SubtreeSize(child->getLeft()) + SubtreeSize(child->getRight()) + 1);
		if (second_child != NULL){
			second_child->setParent(node);
		}
		return (child);


	}

	TreapNode* rotateLeft (TreapNode* node){ //Essas funções não acertam o ultimo link, a saber, retornam uma raiz que sabe quem é o pai, mas o pai nao sabe que é sua filha.
											 //Logo, devemos lembrar, no escopo que chama a função, de corrigir este último link (isso permite o uso recursivo dessas funções).
		if (node == NULL){
			wcout << L"rotateLeft() chamada em NULL" << endl;
			return(NULL);
		}

		TreapNode* child;
		TreapNode* second_child;

		child = node->getRight();
		if (child == NULL){
			return node;
		}
		second_child = child->getLeft();
		child->setParent(node->getParent());
		child->setLeft(node);
		node->setParent(child);
		node->setRight(second_child);
		node->setSize(SubtreeSize(node->getLeft()) + SubtreeSize(node->getRight()) + 1);
		child->setSize(SubtreeSize(child->getLeft()) + SubtreeSize(child->getRight()) + 1);
		if (second_child != NULL){
			second_child->setParent(node);
		}
		return (child);

	}

	TreapNode* insereNo(TreapNode* root, wstring chave, long valor){

		if (root == NULL){
			long priority = (long)g3();
			//wcout << L"Param: " << priority << endl;
			root = new TreapNode(chave, valor, priority);
			FixSize(root);
			return root;
		}
		//root != NULL
		else if (root->getKey() == chave){
			root->setVal(root->getVal() + valor);
		}
		else if (root->getKey() < chave){
			FixRight(root,insereNo(root->getRight(), chave, valor)); //chamando insere no nó direito, corrigimos o ultimo link da rotação

			if (GetPrio(root->getRight()) > GetPrio(root)){
				root = rotateLeft(root);
			}
		}
		else if (root->getKey() > chave){

			FixLeft(root, insereNo(root->getLeft(), chave, valor)); //chamando insere no nó esquerdo, corrigimos o ultimo link da rotação.

			if(GetPrio(root->getLeft()) > GetPrio(root)){
				root = rotateRight(root);
			}
		}
		FixSize(root);
		return (root);
	}



	void insere (wstring chave, long valor){

		TreapNode* root = this->getRoot();
		long subtree_size = SubtreeSize(root);
		TreapNode* new_root = insereNo(root, chave, valor);
		if (SubtreeSize(new_root) > subtree_size){
			this->setRoot(new_root);
		}
	}
	

	long devolve(wstring chave){

		TreapNode* temp = this->getRoot();

		while (temp != NULL){
			if (temp->getKey() == chave){
				return temp->getVal();
			}
			else if (temp->getKey() < chave){
				temp = temp->getRight();
			}
			else {
				temp = temp->getLeft();
			}
		}
		//wcout << L"Chave nao encontrada" << endl;
		return (-1);

	}

	TreapNode* removeNo(TreapNode* root, wstring chave){

		if (root == NULL){
			return root;
		}
		else if (root->getKey() < chave){
			FixRight(root, removeNo(root->getRight(), chave));	
		}
		else if (root->getKey() > chave){
			FixLeft(root, removeNo(root->getLeft(), chave));
		}
		else if (root->getKey() == chave){

			if (root->getLeft() == NULL && root->getRight() == NULL){
				TreapNode* temp = root;
				root = NULL;
				delete temp;
			}
			else if (root->getLeft() == NULL){
				//root->getRight() != NULL
				TreapNode* temp = root;
				root = root->getRight();
				root->setParent(temp->getParent());
				delete temp;
			}
			else if (root->getRight() == NULL){
				//root->getLeft() != NULL
				TreapNode* temp = root;
				root = root->getLeft();
				root->setParent(temp->getParent());
				delete temp;
			}
			else{
				if (GetPrio(root->getLeft()) > GetPrio(root->getRight())){
					root = rotateRight(root);
					FixRight(root, removeNo(root->getRight(), chave));
				}
				else {
					root = rotateLeft(root);
					FixLeft(root, removeNo(root->getLeft(), chave));
				}
			}
		}
		FixSize(root);
		return root;
	}

	void remove(wstring chave){

		TreapNode* root = this->getRoot();
		long subtree_size = SubtreeSize(root);
		TreapNode* new_root = removeNo(root, chave);
		this->setRoot(new_root);
		if (subtree_size == SubtreeSize(new_root)){
			//wcout << L"Tamanho da arvore nao mudou. Possivel erro." << endl;
		} 
	
	}

	long rank(wstring chave){

		TreapNode* temp = getRoot();
		long rank = 0;

		while (temp != NULL){
			/*else if (temp->getLeft() == NULL){
				rank = rank + 0;
			}*/
			if (temp->getKey() == chave){
				if (temp->getLeft() != NULL){
					rank = rank + SubtreeSize(temp->getLeft());
				}
				return rank;
			}
			else if (temp->getKey() < chave){
				if (temp->getLeft() != NULL){
					rank = rank + SubtreeSize(temp->getLeft());
				}
				rank = rank + 1;
				temp = temp->getRight();

			}
			else {	//temp->getKey() > chave
				temp = temp->getLeft();
			}
		}
		//wcout << L"chave nao encontrada" << endl;
		return rank;

	}


	wstring select(long given_rank){

		TreapNode* temp = getRoot();
		long cur_rank = 0;

		while (temp != NULL){
			cur_rank = rank(temp->getKey());
			if (cur_rank == given_rank){
				return temp->getKey();
			}
			else if (cur_rank < given_rank){
				temp = temp->getRight();
			}
			else {
				temp = temp->getLeft();
			}
		}
		//wcout << L"Chave nao encontrada" << endl;
		return L"";

	}

	void enqueue_children(TreapNode* node, TreapQueue*& print_queue){
		if (node == NULL){
			return;
		}

		print_queue->Enqueue(node->getLeft());
		print_queue->Enqueue(node->getRight());
		return;

	}

	void print(){

		TreapQueue* print_queue = new TreapQueue;
		print_queue->Enqueue(this->root);
		while(print_queue->QueueEmpty() == 0){
			TreapNode* head = print_queue->Dequeue();
			if (head == NULL){
				wcout << L"Filho NULL. ";
				continue;
			}
			wcout << L"Palavra " << head->getKey() << L" tem valor " << head->getVal() << L" e prioridade " << head->getPriority() << L". ";
			if (head->getParent() != NULL){
				wcout << L"Pai de " << head->getKey() << L": " << head->getParent()->getKey() << L". ";
			}
			enqueue_children(head, print_queue);
		}
		delete print_queue;
	}


};