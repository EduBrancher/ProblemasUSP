#include "TreeNode.h"
#include "SymbolTable.h"
#include <iostream>          
#include <wchar.h>
#include <string>

using namespace std;

class TreeQueueNode{

	TreeNode* node;
	TreeQueueNode* next;

public:
	TreeQueueNode(TreeNode* node){
		this->node = node;
		this->next = NULL;
	}
	~TreeQueueNode(){

	}
	void setNext(TreeQueueNode* queue_node){
		this->next = queue_node;
	}
	TreeQueueNode* getNext(){
		return this->next;
	}
	TreeNode* getNode(){
		return this->node;
	}

};

class TreeQueue {

	TreeQueueNode* start;
	TreeQueueNode* end;

public:
	TreeQueue(){
		start = NULL;
		end = NULL;
	}
	~TreeQueue(){

	}
	void Enqueue(TreeNode* recv_node){

		TreeQueueNode* new_node = new TreeQueueNode(recv_node);
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
	TreeNode* Dequeue(){

		TreeQueueNode* temp = this->start;

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
		TreeNode* ret = temp->getNode();
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


class SymbolTableAB: public SymbolTable{

	TreeNode* root; //árvore

public:

	SymbolTableAB(){

		this->root = NULL;

	}
	void DeleteTable(TreeNode* root){

		if (root == NULL){
			return;
		}

		DeleteTable(root->getLeft());
		DeleteTable(root->getRight());
		delete root;
		return;
	}

	long getSize(){
		return SubtreeSize(this->getRoot());
	}

	~SymbolTableAB(){
		DeleteTable(this->root);
	}

	TreeNode* getRoot(){
		return this->root;
	}

	TreeNode* setRoot(TreeNode* new_root){
		this->root = new_root;
		return root;
	}

	long SubtreeSize(TreeNode* node){
		if (node == NULL){
			return 0;
		}
		else{
			return node->getSize();
		}
	}

	void FixRight(TreeNode* broken, TreeNode* right){
		broken->setRight(right);
		if (right != NULL){
			right->setParent(broken);
		}
	}

	void FixLeft(TreeNode* broken, TreeNode* left){
		broken->setLeft(left);
		if (left != NULL){
			left->setParent(broken);
		}
	}

	void FixSize(TreeNode* node){

		if (node == NULL){
			return;
		}
		node->setSize(SubtreeSize(node->getLeft()) + SubtreeSize(node->getRight()) + 1);

	}

	void insere (wstring chave, long valor){

		if (this->getRoot() == NULL){
			this->setRoot(new TreeNode(chave, valor));
			return;
		}

 
		TreeNode* temp = getRoot(); //root != NULL
		TreeNode* temp2;

		while (temp != NULL){

			if (temp->getKey() == chave){
				temp->setVal(temp->getVal() + 1);
				return;
			}
			else if (temp->getKey() < chave){
				temp2 = temp;
				temp = temp->getRight();
			}
			else{
				temp2 = temp;
				temp = temp->getLeft();
			}

		}
		// temp aponta para NULL, temp2 aponta para o nodo final antes de NULL. temp->getKey() != temp2->getKey().
		if (temp2->getKey() < chave){
			FixRight(temp2, new TreeNode(chave, valor));
			FixSize(temp2);
			//ajustar desbalanceamentos
			/*if (temp2->getLeft() == NULL && temp2->getParent() != NULL && temp2->getParent()->getLeft() == NULL){
				TreeNode* temp3 = temp2->getParent();
				temp3->setSize(temp3->getSize() - 1);
				temp2->setLeft(temp3);
				temp2->setSize(temp2->getSize() + 1);
				temp2->setParent(temp3->getParent());
				temp3->setParent(temp2);
				temp3->setRight(NULL);
			}
			else if (temp2->getLeft() == NULL && temp2->getParent() != NULL && temp2->getParent()->getRight() == NULL){
				TreeNode* temp3 = temp2 ->getParent();
				temp = temp2->getRight();
				temp->setParent(temp3->getParent());
				temp->setSize(temp2->getSize() + 1);
				temp->setLeft(temp2);
				temp2->setParent(temp);
				temp2->setSize(temp2->getSize() - 1);
				temp->setRight(temp3);
				temp3->setParent(temp);
				temp3->setSize(temp2->getSize());
				temp2->setRight(NULL);
				temp->setLeft(NULL);
				
			}*/
		}
		else{
			FixLeft(temp2, new TreeNode(chave, valor));
			FixSize(temp2);
			//ajustar desbalanceamentos
			/*if (temp2->getRight() == NULL && temp2->getParent() != NULL && temp2->getParent()->getRight() == NULL){
				temp = temp2->getParent();
				temp2->setParent(temp->getParent());
				temp->setParent(temp2);
				temp->setLeft(NULL);
				temp2->setRight(temp);
				temp->setSize(temp2->getSize() - 1);
				temp2->setSize(temp2->getSize() + 1);

			}
			else if (temp2->getRight() == NULL && temp2->getParent() != NULL && temp2->getParent()->getLeft() == NULL){
				TreeNode* temp3 = temp2->getParent();
				temp = temp2->getLeft();
				temp->setParent(temp3->getParent());
				temp->setSize(temp3->getSize());
				temp3->setRight(NULL);
				temp3->setParent(temp);
				temp3->setSize(temp2->getSize() - 1);
				temp->setLeft(temp3);
				temp2->setLeft(NULL);
				temp2->setParent(temp);
				temp2->setSize(temp2->getSize() - 1);
				temp->setRight(temp2);

				
			}*/
		}

		//voltar subindo para atualizar size.
		while(temp2 != NULL){
			temp2 = temp2->getParent();
			FixSize(temp2);
		}

	}

	long devolve(wstring chave){

		TreeNode* temp = getRoot();

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

	void remove(wstring chave){

		TreeNode* temp = getRoot();

		while (temp != NULL){
			if (temp->getKey() == chave){
				if (temp->getLeft() == NULL && temp->getRight() == NULL){
					TreeNode* temp2 = temp->getParent();
					if (temp2->getLeft() == temp){
						FixLeft(temp2, NULL);
					}
					else if (temp2->getRight() == temp){
						FixRight(temp2, NULL);
					}
					delete temp;
					while (temp2 != NULL){
						FixSize(temp2);
						temp2 = temp2->getParent();
					}
					return;
				}
				else {
					TreeNode* min;
					TreeNode* temp2;
					TreeNode* temp3;
					if (temp->getLeft() == NULL){
						temp2 = temp->getParent();
						if (temp2->getLeft() == temp){
							FixLeft(temp2, temp->getRight());
							FixSize(temp2);
						}
						else { // temp2->getRight() == temp
							FixRight(temp2, temp->getRight());
							FixSize(temp2);
						}
						temp->setParent(NULL);
						delete temp;
					}
					else if (temp->getRight() == NULL){
						temp2 = temp->getParent();
						if (temp2->getLeft() == temp){
							FixLeft(temp2, temp->getLeft());
							FixSize(temp2);
						}
						else{ // temp2->getRight() == temp
							FixRight(temp2, temp->getRight());
						}
						temp->setParent(NULL);
						delete temp;
					}
					else{ //temp->getLeft() != NULL && temp->getRight() != NULL
						min = temp->getRight();
						while (min != NULL){
							temp2 = min;
							min = min->getLeft();
						}
						//temp2 guarda o menor valor da subarvore direita => temp2->getLeft() == NULL. Desconectando min:
						if (temp2->getRight() != NULL){
							min = temp2;
							temp3 = min->getRight();
							temp2 = min->getParent();
							if (temp2->getRight() == min){
								FixRight(temp2, temp3);
							}
							else if (temp2->getLeft() == min){
								FixLeft(temp2, temp3);
							}
							FixSize(temp2);
							FixRight(min, NULL);
							FixSize(min);
							min->setParent(NULL);
						}
						else{ //temp2->getRight() == NULL
							min = temp2;
							temp2 = temp2->getParent();
							if (temp2->getLeft() == min){
								FixLeft(temp2, NULL);
							}
							else if (temp2->getRight() == min){
								FixRight(temp2, NULL);
							}
							FixSize(temp2);
							min->setParent(NULL);

						}
						//trocando temp por min
						temp->setKey(min->getKey());
						temp->setVal(min->getVal());
						delete min;

					}
					while (temp2 != NULL){
						FixSize(temp2);
						temp2 = temp2->getParent();
					}
					return;
				}

			}
			if (temp->getKey() < chave){
				temp = temp->getRight();
			}
			else if (temp->getKey() > chave){
				temp = temp->getLeft();
			}
		}


	}

	long rank(wstring chave){

		TreeNode* temp = getRoot();
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

		TreeNode* temp = getRoot();
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
	void enqueue_children(TreeNode* node, TreeQueue*& print_queue){
		if (node == NULL){
			return;
		}

		print_queue->Enqueue(node->getLeft());
		print_queue->Enqueue(node->getRight());
		return;

	}

	void print(){

		TreeQueue* print_queue = new TreeQueue;
		print_queue->Enqueue(this->root);
		while(print_queue->QueueEmpty() == 0){
			TreeNode* head = print_queue->Dequeue();
			if (head == NULL){
				wcout << L"Filho NULL. ";
				continue;
			}
			wcout << L"Palavra " << head->getKey() << L" tem valor " << head->getVal() << L". ";
			if (head->getParent() != NULL){
				wcout << L"Pai de " << head->getKey() << L": " << head->getParent()->getKey() << L". ";
			}
			enqueue_children(head, print_queue);
		}

		delete print_queue;
	}
};
