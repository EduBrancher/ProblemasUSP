#include "Node23.h"
#include "SymbolTable.h"
#include <iostream>          
#include <wchar.h>
#include <string>



using namespace std;

class A23QueueNode{

	Node23* node;
	A23QueueNode* next;

public:
	A23QueueNode(Node23* node){
		this->node = node;
		this->next = NULL;
	}
	~A23QueueNode(){

	}
	void setNext(A23QueueNode* queue_node){
		this->next = queue_node;
	}
	A23QueueNode* getNext(){
		return this->next;
	}
	Node23* getNode(){
		return this->node;
	}

};

class A23Queue {

	A23QueueNode* start;
	A23QueueNode* end;

public:
	A23Queue(){
		start = NULL;
		end = NULL;
	}
	~A23Queue(){

	}
	void Enqueue(Node23* recv_node){

		A23QueueNode* new_node = new A23QueueNode(recv_node);
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
	Node23* Dequeue(){

		A23QueueNode* temp = this->start;

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
		Node23* ret = temp->getNode();
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

class SymbolTableA23: public SymbolTable{

	Node23* root; //árvore

public:

	SymbolTableA23(){

		this->root = NULL;

	}

	void DeleteTable(Node23* root){

		if (root == NULL){
			return;
		}
		if (root->getState() == 0){
			wcout << L"Erro! Buraco na arvore em delete." << endl;
			return;
		}
		if (root->getState() == 2){
			DeleteTable(root->getLeft());
			DeleteTable(root->getRight());
			delete root;
			return;
		}
		else if (root->getState() == 3){
			DeleteTable(root->getLeft());
			DeleteTable(root->getMid());
			DeleteTable(root->getRight());
			delete root;
			return;
		}
		return;
	}

	~SymbolTableA23(){
		DeleteTable(this->root);
	}

	Node23* getRoot(){
		return this->root;
	}

	long getSize(){
		Node23* root = this->getRoot();
		if (root->getState() == 2){
			return (SubtreeSize(root->getLeft()) + SubtreeSize(root->getRight()) + root->getState()-1);
		}
		else if (root->getState() == 3){
			return (SubtreeSize(root->getLeft()) + SubtreeSize(root->getRight()) + SubtreeSize(root->getMid()) + root->getState()-1);
		}
		return -1;
	}

	long SubtreeSize(Node23* node){
		if (node == NULL){
			return 0;
		}
		else{
			return node->getSize();
		}
	}

	long GetState(Node23* node){
		if (node == NULL){
			return 0;
		}
		else{
			return node->getState();
		}
	}

	void setRoot(Node23* root){
		this->root = root;
	}

	void FixRight(Node23* broken, Node23* right){
		broken->setRight(right);
		if (right != NULL){
			right->setParent(broken);
		}
	}
	void FixMid(Node23* broken, Node23* mid){
		broken->setMid(mid);
		if (mid != NULL){
			mid->setParent(broken);
		}
	}
	void FixLeft(Node23* broken, Node23* left){
		broken->setLeft(left);
		if (left != NULL){
			left->setParent(broken);
		}
	}

	long Folha(Node23* node){

		if (node == NULL){
			wcout << L"Folha chamada com node NULL, erro" << endl;
			return -1;
		}
		if (node->getLeft() == NULL){
			if (node->getState() == 0){
				return 1;
			}
			if (node->getRight() != NULL){
				wcout << L"Nó com NULL à esq e não NULL à dir. Erro." << endl;
				return -1;
			}
			if (node->getState() == 2){
				return 1;
			}
			if (node->getState() == 3 && node->getMid() != NULL){
				wcout << L"Nó com NULL à esq e dir e não NULL em mid. Erro." << endl;
				return -1;
			}
			return 1;
		}
		if (node->getRight() == NULL){
			wcout << L"Nó com NULL à direita e não null à esq. Erro." << endl;
			return -1;
		}
		if (node->getState() == 3){
			if (node->getMid() == NULL){
				wcout << L"3-Nó com mid NULL e esq e dir não NULL. Erro." << endl;
				return -1;
			}
		}
		return 0;
	}

	Node23* Replace(Node23* node, wstring chave){
		//wcout << L"4" << endl; 
		//Recebe um nó a ser removido e retorna um ponteiro para uma folha que pode entrar em seu lugar, sem alterar a ordem da árvore. Para tanto, obtemos o maximo
		//da subarvore esquerda.

		if (node == NULL){
			wcout << L"Erro: Replace chamado em NULL" << endl;
			return NULL;
		}

		Node23* replacement;

		if (Folha(node) == 1){
			//queremos remover uma folha.
			replacement = node;
			return replacement;
		}	

		//node não é folha, logo tem pelo menos dois apontadores, um pra esquerda e um pra direita.
		Node23* temp;

		if (node->getState() == 2 || (node->getState() == 3 && node->getKeyEsq() == chave)){
			temp = node->getLeft();
		}
		else if (node->getState() == 3 && node->getKeyDir() == chave){
			temp = node->getMid();
		}
		
		//temp diferente de null pois node não é folha.
		if (temp == NULL){
			wcout << L"Nodo identificado como nao folha com ponteiro getLeft() == NULL, erro" << endl;
		}

		while (temp != NULL){
			replacement = temp;
			temp = temp->getRight();
		}
		if (Folha(replacement) == 1){
			return replacement;
		}
		else{
			wcout << L"Replace retornou nao-folha, erro!" << endl;
			return NULL;
		}
	}

	void FixSize(Node23* node){
		if (node == NULL){
			wcout << L"FixSize chamado em NULL, erro!" << endl;
		}
		if (node->getState() == 2){
			node->setSize(SubtreeSize(node->getLeft()) + SubtreeSize(node->getRight()) + node->getState()-1);
		}
		else if (node->getState() == 3){
			node->setSize(SubtreeSize(node->getLeft()) + SubtreeSize(node->getRight()) + SubtreeSize(node->getMid()) + node->getState()-1);
		}
		else if (node->getState() == 0){
			node->setSize(SubtreeSize(node->getLeft()));
		}	
	}



	Node23* insereNo(Node23* root, wstring chave, long valor, long& cresceu){ //devolve a raiz da arvore com o nó inserido. (put23)

		if (root == NULL){
			root = new Node23(chave, valor);
			cresceu = 1;
			return (root);
		} //OK

		if (Folha(root) == 1){
			if (root->getState() == 2){
				//2-no
				if (root->getKeyEsq() == chave){
					root->setValEsq(root->getValEsq() + valor);
					cresceu = 0;
					return root;
				} //OK 
				else if (root->getKeyEsq() < chave){
					root->setKeyDir(chave);
					root->setValDir(valor);
					root->setState(3); //3-no
					FixMid(root, NULL);
					cresceu = 0;
					FixSize(root);
					return root;
				} //OK
				else if (root->getKeyEsq() > chave){
					root->setKeyDir(root->getKeyEsq());
					root->setValDir(root->getValEsq());
					root->setKeyEsq(chave);
					root->setValEsq(valor);
					root->setState(3);
					FixMid(root, NULL);
					cresceu = 0;
					FixSize(root);
					return root;
				} //OK
			}
			else if (root->getState() == 3){
				wstring smallest;
				wstring middle;
				wstring greatest;
				long smallest_val;
				long middle_val;
				long greatest_val;

				if (root->getKeyEsq() == chave){
					root->setValEsq(root->getValEsq() + valor);
					cresceu = 0;
					return root;
				} // ok
				else if (root->getKeyDir() == chave){
					root->setValDir(root->getValDir() + valor);
					cresceu = 0;
					return root;
				}// ok
				else if (root->getKeyDir() < chave){
					//chave é o maior dos 3
					greatest = chave;
					greatest_val = valor;
					middle = root->getKeyDir();
					middle_val = root->getValDir();
					smallest = root->getKeyEsq();
					smallest_val = root->getValEsq();
				} // ok
				else if (root->getKeyEsq() > chave){
					//chave é o menor dos 3
					smallest = chave;
					smallest_val = valor;
					middle = root->getKeyEsq();
					middle_val = root->getValEsq();
					greatest = root->getKeyDir();
					greatest_val = root->getValDir();
				} // ok
				else{
					smallest = root->getKeyEsq();
					smallest_val = root->getValEsq();
					greatest = root->getKeyDir();
					greatest_val = root->getValDir();
					middle = chave;
					middle_val = valor;
				} // ok

				Node23* new_root = new Node23(middle, middle_val);
				Node23* left = new Node23(smallest, smallest_val);
				Node23* right = new Node23(greatest, greatest_val);

				FixLeft(new_root, left);
				FixRight(new_root,right);
				FixSize(left);
				FixSize(right);
				FixSize(new_root);

				cresceu = 1;
				delete root;
				return new_root;
			} //ok
		}
		else{
			//não é folha
			if (root->getKeyEsq() == chave){
				root->setValEsq(root->getValEsq() + valor);
				cresceu = 0;
				return root;
			} //ok
			else if (root->getKeyEsq() > chave){
				//chave menor que getkeyesq(), inserimos à esq.
				Node23* temp = insereNo(root->getLeft(), chave, valor, cresceu);
				if (cresceu == 0){
					FixLeft(root, temp);
					FixSize(root);
					return (root);
				} //ok
				else if (cresceu == 1){
					if (root->getState() == 2){
						//sabemos que a chave da raiz é maior que a chave em temp. Logo, temp deve ser o valor à esquerda.
						root->setState(3);
						root->setKeyDir(root->getKeyEsq());
						root->setValDir(root->getValEsq());
						root->setKeyEsq(temp->getKeyEsq());
						root->setValEsq(temp->getValEsq());
						FixMid(root, temp->getRight());
						FixLeft(root, temp->getLeft());
						FixSize(root);
						cresceu = 0;
						delete temp;
						return root;
					} //ok
					if (root->getState() == 3){
						//temos que explodir a raiz. vamos pegar a ordem dos valores. Estamos recebendo um valor de retorno de uma inserção à esquerda, temp. Logo nao ha root->getLeft().
						wstring smallest;
						wstring middle;
						wstring greatest;
						long smallest_val;
						long middle_val;
						long greatest_val;

						smallest = temp->getKeyEsq();
						smallest_val = temp->getValEsq();
						middle = root->getKeyEsq();
						middle_val = root->getValEsq();
						greatest = root->getKeyDir();
						greatest_val = root->getValDir();

						Node23* mid = new Node23(middle, middle_val);
						Node23* right = new Node23(greatest, greatest_val);
						FixLeft(right, root->getMid());
						FixRight(right, root->getRight());
						FixSize(right);
						root->setKeyEsq(smallest);
						root->setValEsq(smallest_val);
						root->setState(2);
						FixLeft(root, temp->getLeft());
						FixRight(root, temp->getRight());
						FixMid(root, NULL);
						FixSize(root);
						FixLeft(mid, root);
						FixRight(mid, right);
						FixSize(mid);
						cresceu = 1;
						delete temp;
						return mid;
					}
				}
			}
			else if (root->getKeyEsq() < chave){
				if (root->getState() == 2){
					//insere à direita
					Node23* temp = insereNo(root->getRight(), chave, valor, cresceu);
					if (cresceu == 0){
						FixRight(root, temp);
						FixSize(root);
						return root;
					}
					else if (cresceu == 1){
						//temp tem que ser inserido na raiz e veio da direita, logo, é a maior das chaves.
						root->setState(3);
						root->setKeyDir(temp->getKeyEsq());
						root->setValDir(temp->getValEsq());
						FixMid(root, temp->getLeft());
						FixRight(root, temp->getRight());
						FixSize(root);
						cresceu = 0;
						delete temp;
						return root;
					}
				}
				else if (root->getState() == 3){
					//root é 3-no, podemos avaliar keydir
					if (root->getKeyDir() == chave){
						root->setValDir(root->getValDir() + valor);
						cresceu = 0;
						return root;
					}
					else if (root->getKeyEsq() < chave && root->getKeyDir() > chave){
						//insere no meio
						Node23* temp = insereNo(root->getMid(), chave, valor, cresceu);
						if (cresceu == 0){
							FixMid(root, temp);
							FixSize(root);
							return root;
						}
						else if (cresceu == 1){
							//valor temp retornando entre a esquerda e a direita da raiz, precisamos explodir a raiz com temp no meio
							wstring smallest = root->getKeyEsq();
							wstring greatest = root->getKeyDir();
							long smallest_val = root->getValEsq();
							long greatest_val = root->getValDir();

							Node23* left = new Node23(smallest, smallest_val);
							Node23* right = new Node23(greatest, greatest_val);
							FixLeft(left, root->getLeft());
							FixRight(left, temp->getLeft());
							FixSize(left);
							FixRight(right, root->getRight());
							FixLeft(right, temp->getRight());
							FixSize(right);
							FixLeft(temp, left);
							FixRight(temp, right);
							FixSize(temp);
							cresceu = 1;
							delete root;
							return (temp);
						}
					}
					else if (root->getKeyDir() < chave){
						//insere à direita
						Node23* temp = insereNo(root->getRight(), chave, valor, cresceu);
						if (cresceu == 0){
							FixRight(root, temp);
							FixSize(root);
							return root;
						}
						else if (cresceu == 1){
							//voltando de uma inserção à direita em um 3-nó, precisamos explodir o nó.
							wstring middle = root->getKeyDir(); //a chave em temp é maior que essa, pois inserimos à direita
							long middle_val = root->getValDir();
							Node23* mid = new Node23(middle, middle_val);
							FixRight(mid, temp);
							FixLeft(mid, root);
							FixSize(mid);
							root->setState(2);
							FixRight(root, root->getMid());
							FixMid(root, NULL);
							FixSize(root);
							cresceu = 1;
							return mid; 

						}
					}
				}
			}
		}
		wcout << L"end of function error 3" << endl;
		return NULL;
	}
		




	void insere (wstring chave, long valor){

		Node23* root = this->getRoot();
		if (root == NULL){
			//wcout <<"Raiz nula!!"<< endl;
		}
		long cresceu = 0;

		Node23* new_root = insereNo(root, chave, valor, cresceu);
		new_root->setParent(NULL);
		this->setRoot(new_root);

	}
	

	long devolve(wstring chave){

		Node23* temp = this->getRoot();

		while (temp != NULL){
			if (temp->getKeyEsq() == chave){
				return temp->getValEsq();
			}
			else if (temp->getKeyEsq() > chave){
				temp = temp->getLeft();
			}
			else if (temp->getKeyEsq() < chave){
				if (temp->getState() == 2){
					temp = temp->getRight();
				}
				else if (temp->getState() == 3){
					if (temp->getKeyDir() == chave){
						return temp->getValDir();
					}
					else if (temp->getKeyDir() > chave){
						temp = temp->getMid();
					}
					else if (temp->getKeyDir() < chave){
						temp = temp->getRight();
					}
				}
			}	
		}
		//wcout << L"Chave nao encontrada" << endl;
		return (-1);

	}

	Node23* FixHole(Node23* hole, Node23*& new_node){

		//wcout << L"3" << endl;
		Node23* parent = hole->getParent();
		//wcout << L"Parent:" <<parent->getKeyEsq() << endl;
		Node23* left_sibling;
		Node23* mid_sibling;
		Node23* right_sibling;

		if (parent == NULL){
			return hole;
		}
		//parent != NULL
		if (parent->getLeft() == hole){
			if (parent->getState() == 3){
				left_sibling = hole;
				mid_sibling = parent->getMid();
				right_sibling = parent->getRight();
			}
			else {
				left_sibling = hole;
				mid_sibling = NULL;
				right_sibling = parent->getRight();
			}
		}
		else if (parent->getRight() == hole){
			if (parent->getState() == 3){
				left_sibling = parent->getLeft();
				mid_sibling = parent->getMid();
				right_sibling = hole;
			}
			else{
				left_sibling = parent->getLeft();
				mid_sibling = NULL;
				right_sibling = hole;
			}
		}
		else if (parent->getState() == 3){
			if (parent->getMid() == hole){
				left_sibling = parent->getLeft();
				right_sibling = parent->getRight();
				mid_sibling = hole;
			}
		}

		//casos

		if (parent->getState() == 2){
			//existem apenas left sibling e right sibling
			if (hole == left_sibling){
				if (right_sibling->getState() == 2){
					//wcout << L"3.1" << endl;
					right_sibling->setState(3);
					right_sibling->setKeyDir(right_sibling->getKeyEsq());
					right_sibling->setValDir(right_sibling->getValEsq());
					right_sibling->setKeyEsq(parent->getKeyEsq());
					right_sibling->setValEsq(parent->getValEsq());
					FixMid(right_sibling, right_sibling->getLeft());
					FixLeft(right_sibling, hole->getLeft());
					FixSize(right_sibling);
					parent->setState(0);
					FixLeft(parent, right_sibling);
					FixRight(parent, NULL);
					FixSize(parent);
					new_node = right_sibling;
					delete hole;
					//wcout << L"3.1end" << endl;
					return (parent); //buraco foi pro pai
				}
				else if (right_sibling->getState() == 3){
					//wcout << L"3.2" << endl;
					hole->setState(2);
					hole->setKeyEsq(parent->getKeyEsq());
					hole->setValEsq(parent->getValEsq());
					FixRight(hole, right_sibling->getLeft());
					FixSize(hole);
					parent->setKeyEsq(right_sibling->getKeyEsq());
					parent->setValEsq(right_sibling->getValEsq());
					right_sibling->setKeyEsq(right_sibling->getKeyDir());
					right_sibling->setValEsq(right_sibling->getValDir());
					right_sibling->setState(2);
					FixLeft(right_sibling, right_sibling->getMid());
					FixMid(right_sibling, NULL);
					FixSize(right_sibling);
					FixSize(parent);
					new_node = parent;
					//wcout << L"3.2end" << endl;
					return parent; //buraco absorvido
				}
			}
			else if (hole == right_sibling){
				//existem apenas left e right sibling, parent é 2-node
				if (left_sibling->getState() == 2){
					//wcout << L"3.3" << endl;
					left_sibling->setState(3);
					left_sibling->setKeyDir(parent->getKeyEsq());
					left_sibling->setValDir(parent->getValEsq());
					FixMid(left_sibling, left_sibling->getRight());
					FixRight(left_sibling, right_sibling->getLeft());
					FixSize(left_sibling);
					parent->setState(0); //pai é o buraco
					FixLeft(parent, left_sibling);
					FixRight(parent, NULL);
					FixSize(parent);
					new_node = left_sibling;
					delete (hole);
					//wcout << L"3.3end" << endl;
					return (parent); //buraco foi pro pai
				}
				else if (left_sibling->getState() == 3){
					//wcout << L"3.4" << endl;
					hole->setState(2);
					hole->setKeyEsq(parent->getKeyEsq());
					hole->setValEsq(parent->getValEsq());
					FixRight(hole, hole->getLeft());
					FixLeft(hole, left_sibling->getRight());
					FixSize(hole);
					parent->setKeyEsq(left_sibling->getKeyDir());
					parent->setValEsq(left_sibling->getValDir());
					left_sibling->setState(2);
					FixRight(left_sibling, left_sibling->getMid());
					FixMid(left_sibling, NULL);
					FixSize(left_sibling);
					FixSize(parent);
					new_node = parent;
					//wcout << L"3.4end" << endl;
					return (parent); //buraco absorvido

				}
			}
		}
		else if (parent->getState() == 3){
			if (hole == left_sibling){
				if (mid_sibling->getState() == 2){
					//wcout << L"3.5" << endl;
					mid_sibling->setState(3);
					mid_sibling->setKeyDir(mid_sibling->getKeyEsq());
					mid_sibling->setValDir(mid_sibling->getValEsq());
					mid_sibling->setKeyEsq(parent->getKeyEsq());
					mid_sibling->setValEsq(parent->getValEsq());
					FixMid(mid_sibling, mid_sibling->getLeft());
					FixLeft(mid_sibling, hole->getLeft());
					FixSize(mid_sibling);
					parent->setKeyEsq(parent->getKeyDir());
					parent->setValEsq(parent->getValDir());
					parent->setState(2);
					FixLeft(parent, mid_sibling);
					FixMid(parent, NULL);
					FixSize(parent);
					delete hole;
					new_node = parent;
					//wcout << L"3.5end" << endl;
					return (parent); //buraco absorvido
				}
				else if (mid_sibling->getState() == 3){
					//wcout << L"3.6" << endl;
					hole->setState(2);
					hole->setKeyEsq(parent->getKeyEsq());
					hole->setValEsq(parent->getValEsq());
					FixRight(hole, mid_sibling->getLeft());
					FixSize(hole);
					parent->setKeyEsq(mid_sibling->getKeyEsq());
					parent->setValEsq(mid_sibling->getValEsq());
					mid_sibling->setState(2);
					mid_sibling->setKeyEsq(mid_sibling->getKeyDir());
					mid_sibling->setValEsq(mid_sibling->getValDir());
					FixLeft(mid_sibling, mid_sibling->getMid());
					FixSize(mid_sibling);
					FixSize(parent);
					new_node = parent;
					//wcout << L"3.6end" << endl;
					return (parent); //buraco absorvido
				}
			}
			else if (hole == right_sibling){
				if (mid_sibling->getState() == 2){
					//wcout << L"3.7" << endl;
					mid_sibling->setState(3);
					mid_sibling->setKeyDir(parent->getKeyDir());
					mid_sibling->setValDir(parent->getValDir());
					FixMid(mid_sibling, mid_sibling->getRight());
					FixRight(mid_sibling, hole->getLeft());
					FixSize(mid_sibling);
					parent->setState(2);
					FixRight(parent, mid_sibling);
					FixMid(parent, NULL); //foi pra direita
					FixSize(parent);
					delete hole;
					new_node = parent;
					//wcout << L"3.7end" << endl;
					return (parent); //buraco absorvido
				}
				else if (mid_sibling->getState() == 3){
					//wcout << L"3.8" << endl;
					hole->setState(2);
					hole->setKeyEsq(parent->getKeyDir());
					hole->setValEsq(parent->getValDir());
					FixRight(hole, hole->getLeft());
					FixLeft(hole, mid_sibling->getRight());
					FixSize(hole);
					parent->setKeyDir(mid_sibling->getKeyDir());
					parent->setValDir(mid_sibling->getValDir());
					mid_sibling->setState(2);
					FixRight(mid_sibling, mid_sibling->getMid());
					FixMid(mid_sibling, NULL);
					FixSize(mid_sibling);
					FixSize(parent);
					new_node = parent;
					//wcout << L"3.8end" << endl;
					return (parent); //buraco absorvido
				}
			}
			else if (hole == mid_sibling){
				//parent->getState() == 3
				if (left_sibling->getState() == 2){
					//wcout << L"3.9" << endl;
					left_sibling->setState(3);
					left_sibling->setKeyDir(parent->getKeyEsq());
					left_sibling->setValDir(parent->getValEsq());
					FixMid(left_sibling, left_sibling->getRight());
					FixRight(left_sibling, hole->getLeft());
					FixSize(left_sibling);
					parent->setState(2);
					parent->setKeyEsq(parent->getKeyDir());
					parent->setValEsq(parent->getValDir());
					FixMid(parent, NULL); //era o buraco
					FixSize(parent);
					delete hole;
					new_node = parent;
					//wcout << L"3.9end" << endl;
					return (parent); //buraco absorvido
				}
				else if (right_sibling->getState() == 2){
					//wcout << L"3.10" << endl;
					right_sibling->setState(3);
					right_sibling->setKeyDir(right_sibling->getKeyEsq());
					right_sibling->setValDir(right_sibling->getValEsq());
					right_sibling->setKeyEsq(parent->getKeyDir());
					right_sibling->setValEsq(parent->getValDir());
					FixMid(right_sibling, right_sibling->getLeft());
					FixLeft(right_sibling, hole->getLeft());
					FixSize(right_sibling);
					parent->setState(2);
					FixMid(parent, NULL);
					FixSize(parent);
					delete hole;
					new_node = parent;
					//wcout << L"3.10end" << endl;
					return parent; //buraco absorvido
				}
				else if (left_sibling->getState() == 3){
					//wcout << L"3.11" << endl;
					hole->setState(2);
					hole->setKeyEsq(parent->getKeyEsq());
					hole->setValEsq(parent->getValEsq());
					FixRight(hole, hole->getLeft());
					FixLeft(hole, left_sibling->getRight());
					FixSize(hole);
					parent->setKeyEsq(left_sibling->getKeyDir());
					parent->setValEsq(left_sibling->getValDir());
					left_sibling->setState(2);
					FixRight(left_sibling, left_sibling->getMid());
					FixMid(left_sibling, NULL);
					FixSize(left_sibling);
					FixSize(parent);
					new_node = parent;
					//wcout << L"3.11end" << endl;
					return parent; //buraco absorvido

				}


			}
		}
		wcout << L"end of function error 1" << endl;
		return NULL;
	}

	Node23* removeFolha(Node23* root, wstring chave, Node23*& new_node){
		if (root == NULL){
			wcout << L"removeFolha chamado em no NULL, erro" << endl;
			return root;
		}
		if (root->getKeyEsq() > chave){
			Node23* new_left = removeFolha(root->getLeft(), chave, new_node);
			if (new_left->getState() == 0){
				root = FixHole(new_left, new_node);
				FixSize(root);
			}
			else{
				FixLeft(root, new_left);
				FixSize(root);
			}
			if (root->getParent() != NULL){
				FixSize(root->getParent());
			}
			return (root);
		}
		else if (root->getKeyEsq() == chave){
			if (root->getState() == 2){
				if (Folha(root) != 1){
					wcout << L"removeFolha encontrou sua wstring e nao era em uma folha, erro!" << endl;
					return NULL;
				}
				root->setState(0); //buraco
				return (root);
			} //OK
			else if (root->getState() == 3){
				if (Folha(root) != 1){
					wcout << L"removeFolha encontrou sua wstring e nao era em uma folha, erro!" << endl;
					return NULL;
				}
				root->setKeyEsq(root->getKeyDir());
				root->setValEsq(root->getValDir());
				root->setState(2);
				FixSize(root);
				new_node = root;
				return(root);
			}
		}
		else if (root->getKeyEsq() < chave){
			if (root->getState() == 2){
				Node23* new_right = removeFolha(root->getRight(), chave, new_node);
				if (new_right->getState() == 0){
					root = FixHole(new_right, new_node);
					FixSize(root);
				}
				else{
					FixRight(root, new_right);
					FixSize(root);
				}
				if (root->getParent() != NULL){
					FixSize(root->getParent());
				}
				return (root);
			}
			else if (root->getState() == 3){
				//podemos avaliar keydir
				if (root->getKeyDir() == chave){
					if (Folha(root) != 1){
						wcout << L"removeFolha encontrou sua wstring e nao era em uma folha, erro!" << endl;
						return NULL;
					}
					root->setState(2);
					FixMid(root, NULL);
					FixSize(root);
					new_node = root;
					return (root);
				}
				else if (root->getKeyEsq() < chave && root->getKeyDir() > chave){
					Node23* new_mid = removeFolha(root->getMid(), chave, new_node);
					if (new_mid->getState() == 0){
						root = FixHole(new_mid, new_node);
						FixSize(root);
					}
					else{
						FixMid(root, new_mid);
						FixSize(root);
					}
					if (root->getParent() != NULL){
							FixSize(root->getParent());
						}
					return (root);
				}
				else if (root->getKeyDir() < chave){
					Node23* new_right = removeFolha(root->getRight(), chave, new_node);
					if (new_right->getState() == 0){
						root = FixHole(new_right, new_node);
						FixSize(root);
					}
					else{
						FixRight(root, new_right);
						FixSize(root);
					}
					if (root->getParent() != NULL){
						FixSize(root->getParent());
					}
					return (root);
				}
			}
		}
		wcout << L"end of function err" << endl;
		return NULL;
	}

	Node23* removeNo(Node23* root, wstring chave){
		//retorna a raiz da nova árvore com o nó removido. Precisamos atualizar a raiz na função chamadora.
		Node23* temp;
		Node23* replacement = NULL;
		Node23* new_node = NULL;
		wstring replacement_key;
		long replacement_val;
		if (root == NULL){
			wcout << L"removeNo pedido em arvore vazia, erro!" << endl;
			return NULL;
		}
		temp = root;

		while (temp != NULL){
			if (temp->getKeyEsq() > chave){
				temp = temp->getLeft();
			}
			else if (temp->getKeyEsq() == chave){
				if (Folha(temp) != 1){	
				replacement = Replace(temp, chave);
					if (replacement->getState() == 3){
						replacement_key = replacement->getKeyDir();
						replacement_val = replacement->getValDir();
					}
					else if (replacement->getState() == 2){
						replacement_key = replacement->getKeyEsq();
						replacement_val = replacement->getValEsq();
					}
					//wcout << L"replacement: " << replacement_key << L" " << endl;
				}
				break;
			}
			else if (temp->getKeyEsq() < chave){
				if (temp->getState() == 2){
					temp = temp->getRight();
				}			
				else if (temp->getState() == 3){
					if (temp->getKeyDir() < chave){
						temp = temp->getRight();
					}
					else if (temp->getKeyDir() == chave){
						if (Folha(temp) != 1){
							replacement = Replace(temp, chave);
							if (replacement->getState() == 3){
								replacement_key = replacement->getKeyDir();
								replacement_val = replacement->getValDir();
								//wcout << L"replacement: " << replacement_key << L" ";
							}
							else if (replacement->getState() == 2){
								replacement_key = replacement->getKeyEsq();
								replacement_val = replacement->getValEsq();
								//wcout << L"replacement: " << replacement_key << L" ";
							}
						}
						break;
					}
					else if (temp->getKeyEsq() < chave && temp->getKeyDir() > chave){
						temp = temp->getMid();
					}
				}
			}
		}
		if (temp == NULL){
			//wcout << L"Chave nao encontrada por removeNo." << endl;
			return NULL;
		}
		if (replacement == NULL){
			root = removeFolha(root, chave, new_node);
		}
		else if (replacement != NULL){
		root = removeFolha(root, replacement_key, new_node);
		//temos que trocar chave por replacement_key, removemos a folha que continha replacement_key
			if (root->getState() == 0){
				temp = root->getLeft();
			}
			else{
				temp = root;
			}
			while (temp != NULL){
				if (temp->getKeyEsq() > chave){
					temp = temp->getLeft();
				}
				else if (temp->getKeyEsq() == chave){
					temp->setKeyEsq(replacement_key);
					temp->setValEsq(replacement_val);
					break;
				}
				else if (temp->getState() == 2){
					if (temp->getKeyEsq() < chave){
						temp = temp->getRight();
					}
				}
				else if (temp->getState() == 3){
					if (temp->getKeyEsq() < chave && temp->getKeyDir() > chave){
						temp = temp->getMid();
					}
					else if (temp->getKeyDir() == chave){
						temp->setKeyDir(replacement_key);
						temp->setValDir(replacement_val);
						break;
					}
					else if (temp->getKeyDir() < chave){
						temp = temp->getRight();
					}
				}
			}
		}
		//if (temp == NULL){
		//	wcout << L"Chave nao encontrada apos remocao de folha, erro." << endl;
		//}

		if (root == NULL || root->getState() == 0){
			//root é um buraco. Vamos chutar pra fora da árvore.
			delete root;
			root = new_node;
			FixSize(root);
			if (new_node == NULL){
				wcout << L"Tree Empty." << endl;
			}
			if (new_node != NULL){
				new_node->setParent(NULL);
			}
		}
		//se o buraco não voltou pra cá, quer dizer que ele foi absorvido pela arvore. Retornaremos a raiz.
		FixSize(root);
		return root;

	}


	void remove(wstring chave){

		Node23* root = this->getRoot();
		long subtree_size = SubtreeSize(root);
		root = removeNo(root, chave);
		if (subtree_size == SubtreeSize(root)){
			//wcout << L"Tamanho da arvore nao mudou. Possivel erro." << endl;
		}
		if (root == NULL){
			return;
		}
		this->setRoot(root); 
	
	}

	long rank(wstring chave){

		Node23* temp = this->getRoot();
		long rank = 0;

		while (temp != NULL){
			/*else if (temp->getLeft() == NULL){
				rank = rank + 0;
			}*/
			if (temp->getKeyEsq() > chave){
				temp = temp->getLeft();
			}
			else if (temp->getKeyEsq() == chave){
				rank = rank + SubtreeSize(temp->getLeft());
				return rank;
			}
			else if (temp->getKeyEsq() < chave){
				if (temp->getState() == 2){
					rank = rank + SubtreeSize(temp->getLeft()) + 1;
					temp = temp->getRight();
				}
				else if (temp->getState() == 3){
					if (temp->getKeyDir() > chave){
						rank = rank + SubtreeSize(temp->getLeft()) + 1;
						temp = temp->getMid();
					}
					else if (temp->getKeyDir() < chave){
						rank = rank + SubtreeSize(temp->getLeft()) + SubtreeSize(temp->getMid()) + 2;
						temp = temp->getRight();
					}
					else if (temp->getKeyDir() == chave){
						rank = rank + SubtreeSize(temp->getLeft()) + SubtreeSize(temp->getMid()) + 1;
						return rank;
					}
				}
				
			}

		}
		wcout << L"chave nao encontrada por rank, erro" << endl;
		return 0;
	}


	wstring select(long given_rank){

		Node23* temp = this->getRoot();
		long cur_rank = 0;

		while (temp != NULL){
			cur_rank = rank(temp->getKeyEsq());
			if (cur_rank == given_rank){
				return temp->getKeyEsq();
			}
			else if (cur_rank > given_rank){
				temp = temp->getLeft();
			}
			else if (cur_rank < given_rank){
				if (temp->getState() == 2){
					temp = temp->getRight();
				}
				else if (temp->getState() == 3){
					cur_rank = rank(temp->getKeyDir());
					if (cur_rank == given_rank){
						return temp->getKeyDir();
					}
					else if (cur_rank > given_rank){
						temp = temp->getMid();
					}
					else if (cur_rank < given_rank){
						temp = temp->getRight();
					}
				}
			}

		}
		wcout << L"Chave nao encontrada" << endl;
		return L"";

	}
	void enqueue_children(Node23* node, A23Queue*& print_queue){
		if (Folha(node) == 1){
			return;
		}

		if (node->getState() == 2){
			print_queue->Enqueue(node->getLeft());
			print_queue->Enqueue(node->getRight());
			return;
		}
		else{
			print_queue->Enqueue(node->getLeft());
			print_queue->Enqueue(node->getMid());
			print_queue->Enqueue(node->getRight());
			return;
		}
		return;
	}

	void print(){
		if (this->root == NULL){
			wcout << "BLARGH" << endl;
			return;
		}
		A23Queue* print_queue = new A23Queue;
		print_queue->Enqueue(this->root);
		while(print_queue->QueueEmpty() == 0){
			Node23* head = print_queue->Dequeue();
			if (head->getState() == 2){
				wcout << L"Palavra " << head->getKeyEsq() << L" tem valor " << head->getValEsq() << L". ";
				/*if (head->getParent() != NULL){
					//wcout << L"Pai de " << head->getKeyEsq() << L": " << head->getParent()->getKeyEsq() << L".";
				}*/
			}
			if (head->getState() == 3){
				wcout << L"Palavra em 3 no " << head->getKeyEsq() << L" tem valor " << head->getValEsq() << L". ";
				wcout << L"Palavra em 3 no " << head->getKeyDir() << L" tem valor " << head->getValDir() << L". ";
				/*if (head->getParent() != NULL){
					//wcout << L"Pai de " << head->getKeyEsq() << L": " << head->getParent()->getKeyEsq() << L".";
				}*/
			}
			enqueue_children(head, print_queue);
		}
		delete print_queue;

	}
};