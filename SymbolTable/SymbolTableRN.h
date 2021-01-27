#include "RedBlackNode.h"
#include "SymbolTable.h"
#include <iostream>          
#include <wchar.h>
#include <string>



using namespace std;

class RBQueueNode{

	RedBlackNode* node;
	RBQueueNode* next;

public:
	RBQueueNode(RedBlackNode* node){
		this->node = node;
		this->next = NULL;
	}
	~RBQueueNode(){

	}
	void setNext(RBQueueNode* queue_node){
		this->next = queue_node;
	}
	RBQueueNode* getNext(){
		return this->next;
	}
	RedBlackNode* getNode(){
		return this->node;
	}

};

class RBQueue {

	RBQueueNode* start;
	RBQueueNode* end;

public:
	RBQueue(){
		start = NULL;
		end = NULL;
	}
	~RBQueue(){

	}
	void Enqueue(RedBlackNode* recv_node){

		RBQueueNode* new_node = new RBQueueNode(recv_node);
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
	RedBlackNode* Dequeue(){

		RBQueueNode* temp = this->start;

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
		RedBlackNode* ret = temp->getNode();
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


class SymbolTableRN: public SymbolTable	{

	RedBlackNode* root; //árvore
	long size;

public:

	SymbolTableRN(){

		this->root = NULL;
		this->size = 0;

	}
	void DeleteTable(RedBlackNode* root){

		if (root == NULL){
			return;
		}

		DeleteTable(root->getLeft());
		DeleteTable(root->getRight());
		delete root;
		return;
	}

	~SymbolTableRN(){
		DeleteTable(this->root);
	}

	RedBlackNode* getRoot(){
		return this->root;
	}

	long getSize(){
		//return this->size;
		return SubtreeSize(this->getRoot());
	}

	long SubtreeSize(RedBlackNode* node){
		if (node == NULL){
			return 0;
		}
		else{
			return node->getSize();
		}
	}
	void Breakaway(RedBlackNode* node){

		RedBlackNode* parent = node->getParent();
		if (parent->getLeft() == node){
			parent->setLeft(NULL);
		}
		else if (parent->getRight() == node){
			parent->setRight(NULL);
		}
		FixSize(parent);
	}

	void FixRight(RedBlackNode* broken, RedBlackNode* right){
		broken->setRight(right);
		if (right != NULL){
			right->setParent(broken);
		}
	}

	void FixLeft(RedBlackNode* broken, RedBlackNode* left){
		broken->setLeft(left);
		if (left != NULL){
			left->setParent(broken);
		}
	}

	void FixSize(RedBlackNode* node){

		if (node == NULL){
			return;
		}
		node->setSize(SubtreeSize(node->getLeft()) + SubtreeSize(node->getRight()) + 1);

	}

	long GetColour(RedBlackNode* node){
		if (node == NULL){
			return 0;
		}
		else{
			return node->getColour();
		}
	}

	RedBlackNode* setRoot(RedBlackNode* root){
		this->root = root;
		return root;
	}

	long Folha(RedBlackNode* node){

		if (node == NULL){
			wcout << L"Folha chamado em NULL, erro." << endl;
		}
		if (node->getLeft() == NULL && node->getRight() == NULL){
			return 1;
		}
		return 0;
	}

	RedBlackNode* Replace(RedBlackNode* node){
		//Recebe um nó a ser removido e coloca um substituto em seu lugar. Retorna um nó que tem que ter cor verificada e, se necessário, corrigida.

		if (node == NULL){
			wcout << L"Replace chamado em NULL, erro." << endl;
		}

		RedBlackNode* temp;
		RedBlackNode* temp2;
		RedBlackNode* replacement;

		temp = node->getLeft();
		if (temp == NULL){
			temp = node->getRight(); // mínimo da direita
			
			if (temp == NULL){ //node é folha
				temp2 = node->getParent();
				if (temp2 == NULL){
					Breakaway(node);
					delete node;
					this->size = this->size - 1;
					return NULL; //arvore vazia (caso particular muito cretino: arvore com um único nó)
				}
				if (GetColour(node) == 1){
					if (temp2->getLeft() == node){
						FixLeft(temp2, NULL);
						FixSize(temp2);
					}
					if (temp2->getRight() == node){
						FixRight(temp2, NULL);
						FixSize(temp2);
					}
					Breakaway(node);
					delete node;
					this->size = this->size - 1;
				}
				else if (GetColour(node) == 0){ // apagando um preto
					RedBlackNode* temp3;
					node->setColour(2); //duplo preto
					temp3 = FixDoubleBlack(node);
					while (GetColour(temp3) == 2){
						temp3 = FixDoubleBlack(temp3);
					}
					//duplo preto resolvido.
					temp2 = node->getParent();
					if (temp2->getRight() == node){
						FixRight(temp2, NULL);
						FixSize(temp2);
					}
					if (temp2->getLeft() == node){
						FixLeft(temp2, NULL);
						FixSize(temp2);
					}
					Breakaway(node);
					delete node;
					this->size = this->size - 1;
				}
				return temp2; //caso particular: deleção de folha
			}
			//temp != NULL. Casos gerais!
			while (temp != NULL){
				temp2 = temp;
				temp = temp->getLeft();
			}
			//temp == NULL, temp2 é mínimo da subárvore direita. Temp2->getLeft() == NULL.
			replacement = temp2;
			node->setKey(replacement->getKey());
			node->setVal(replacement->getVal()); //põe o valor no lugar, efetivando a deleção de node
			temp2 = replacement->getParent(); //replacement->getLeft() == NULL.
			
			if (replacement->getRight() != NULL){
				RedBlackNode* new_right = replacement->getRight();
				if (temp2->getLeft() == replacement){
					FixLeft(temp2, new_right);
					FixSize(temp2);
				}
				else if (temp2->getRight() == replacement){
					FixRight(temp2, new_right);
					FixSize(temp2);
				}
				if (GetColour(replacement) == 0){//apagando um preto;
					if (GetColour(new_right) == 1){
						new_right->setColour(0); //vermelho fica preto
					}
					else if (GetColour(temp2) == 0){
						new_right->setColour(2); //preto fica duplo-preto
					}
				}
				Breakaway(node);
				delete replacement;
				this->size = this->size - 1;
				return(new_right);
			}
			else if (replacement->getRight() == NULL){ //replacement é folha
				if (GetColour(replacement) == 0){ //apagando um preto
					RedBlackNode* temp3;
					replacement->setColour(2); //duplo preto
					temp3 = FixDoubleBlack(replacement);
					while (GetColour(temp3) == 2){
						temp3 = FixDoubleBlack(temp3);
					}
					temp2 = replacement->getParent();
				}
				if (temp2->getLeft() == replacement){
					FixLeft(temp2, NULL);
					FixSize(temp2);
				}
				else if (temp2->getRight() == replacement){
					FixRight(temp2, NULL);
					FixSize(temp2);
				}
			}
			this->size = this->size - 1;
			Breakaway(node);
			delete replacement;
		}
		else {
			while (temp != NULL){ //máximo da esquerda (temp = node->getLeft() != NULL)
				temp2 = temp;
				temp = temp->getRight();
			}
			//temp == NULL, temp2 é máximo da subárvore esquerda. Temp2->getRight() == NULL.
			replacement = temp2;
			node->setKey(replacement->getKey());
			node->setVal(replacement->getVal()); //põe o valor no lugar, efetivando a deleção de node
			temp2 = replacement->getParent(); //replacement->getLeft() == NULL.

			if (replacement->getLeft() != NULL){
				RedBlackNode* new_left = replacement->getLeft();
				if (temp2->getLeft() == replacement){
					FixLeft(temp2, replacement->getLeft());
					FixSize(temp2);
				}
				else if (temp2->getRight() == replacement){
					FixRight(temp2, replacement->getLeft());
					FixSize(temp2);
				}
				if (GetColour(replacement) == 0){
					if (GetColour(new_left) == 1){
						new_left->setColour(0); //vermelho fica preto
					}
					else if (GetColour(temp2) == 0){
						new_left->setColour(2); //preto fica duplo-preto
					}
				}
				this->size = this->size - 1;
				Breakaway(node);
				delete replacement;
				return(new_left);
			}
			else if (replacement->getLeft() == NULL){ //replacement é folha
				if (GetColour(replacement) == 0){ //apagando um preto
					RedBlackNode* temp3;
					replacement->setColour(2); 	  //duplo preto
					temp3 = FixDoubleBlack(replacement);
					while (GetColour(temp3) == 2){
						temp3 = FixDoubleBlack(temp3);
					}
					temp2 = replacement->getParent();
				}
				if (temp2->getLeft() == replacement){
					FixLeft(temp2, NULL);
					FixSize(temp2);
				}
				else if (temp2->getRight() == replacement){
					FixRight(temp2, NULL);
					FixSize(temp2);
				}
				this->size = this->size - 1;
				Breakaway(node);
				delete replacement;
			}
		}
		return temp2;
	}

	RedBlackNode* rotateRight (RedBlackNode* node){

		if (node == NULL){
			wcout << L"rotateRight() chamada em NULL" << endl;
			return(NULL);
		}

		
		RedBlackNode* child;
		RedBlackNode* second_child;

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

	RedBlackNode* rotateLeft (RedBlackNode* node){ //Essas funções não acertam o ultimo link, a saber, retornam uma raiz que sabe quem é o pai, mas o pai nao sabe que é sua filha.
											 	   //Logo, devemos lembrar, no escopo que chama a função, de corrigir este último link (isso permite o uso recursivo dessas funções).
		if (node == NULL){
			wcout << L"rotateLeft() chamada em NULL" << endl;
			return(NULL);
		}

		RedBlackNode* child;
		RedBlackNode* second_child;

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


	long Crooked(RedBlackNode* filho, RedBlackNode* pai, RedBlackNode* avo){ 
	//Recebe uma arvore, olha o pai, o avô, e o tio, retorna um inteiro de 1 a 4 para a configuração da arvore.
	//números ímpares: pai e filho em lados trocados. 1 ou 2: pai à esquerda do avô. 3, 4: pai à direita do avô.
		if (avo->getLeft() == pai){
			if (pai->getLeft() == filho){
				return (2); //esq->esq
			}
			else {
				return (1); //esq->dir
			}
		}
		else{ //pai a direita do avo
			if (pai->getRight() == filho){
				return (4);	//dir->dir
			}
			else {
				return (3); //dir->esq
			}
		}
		return (0);

	}


	RedBlackNode* FixColour(RedBlackNode* filho){ //Dado um nó, assume que a arvore é correta exceto por ele, corrige as cores,
												  //e retorna o proximo nó que pode ter cor incorreta. Se não houver um tal nó, retorna NULL.
		RedBlackNode* pai;						  //Essa função presume e necessita que nós novos sejam inseridos como vermelhos, sempre.
		RedBlackNode* tio;
		RedBlackNode* avo;

		if (filho == NULL){
			wcout << L"FixColour chamado em NULL, possível erro." << endl;
			return (NULL);
		}
		//filho != NULL
		pai = filho->getParent();
		if (pai == NULL){
			return(pai);
		}
		if (GetColour(filho) == 0){ //não se muda nós pretos.
			return(NULL);
		}
		// pai != NULL, filho vermelho
		if (GetColour(pai) == 0){ //0 == preto, 1 == vermelho
			return(NULL);
		}
		if (GetColour(pai) == 1){ //pai vermelho
			avo = pai->getParent();
			if (avo == NULL){ //nao há tio, se não há avô.
				pai->setColour(0);
				return (NULL);
			}
		//pai != NULL, filho != NULL, avo != NULL	
			if (avo->getRight() == pai){
				tio = avo->getLeft();
			}
			else{
				tio = avo->getRight();
			}
			if (GetColour(tio) == 1){ // pai e tio vermelhos, avo tem que ser preto.
				pai->setColour(0);
				tio->setColour(0); // tio nao pode ser NULL pois GetColour(tio) retornou não-zero.
				avo->setColour(1);
				return(avo); //resolve pra cima
			}
			else if (GetColour(tio) == 0){ //Caso do tio preto
				RedBlackNode* bisavo = avo->getParent();
				if (Crooked(filho, pai, avo) == 1){ //esq->dir
					avo->setColour(1);
					filho->setColour(0);
					FixLeft(avo, rotateRight(pai));
					FixSize(avo);
					if (bisavo == NULL){
						rotateLeft(avo);
					}
					else{
						if (bisavo->getLeft() == avo){
							FixLeft(bisavo, rotateRight(avo));
							FixSize(bisavo);
						}
						else{
							FixRight(bisavo, rotateRight(avo));
							FixSize(bisavo);
						}
					}
				}
				else if (Crooked(filho, pai, avo) == 2){ //esq->esq
					avo->setColour(1);
					pai->setColour(0);
					if (bisavo == NULL){
						rotateRight(avo);
					}
					else{
						if (bisavo->getLeft() == avo){
							FixLeft(bisavo, rotateRight(avo));
							FixSize(bisavo);
						}
						else{
							FixRight(bisavo, rotateRight(avo));
							FixSize(bisavo);
						}
					}
				}
				else if (Crooked(filho, pai, avo) == 3){ //dir->esq
					FixRight(avo, rotateLeft(pai));
					FixSize(avo);
					avo->setColour(1);
					filho->setColour(0);
					if (bisavo == NULL){
						rotateLeft(avo);
					}
					else{
						if (bisavo->getLeft() == avo){
							FixLeft(bisavo, rotateLeft(avo));
							FixSize(bisavo);
						}
						else{
							FixRight(bisavo, rotateLeft(avo));
							FixSize(bisavo);
						}
					}
				}
				else if (Crooked(filho, pai, avo) == 4){ //dir->dir
					avo->setColour(1);
					pai->setColour(0);
					if (bisavo == NULL){
						rotateLeft(avo);
					}
					else{
						if (bisavo->getLeft() == avo){
							FixLeft(bisavo, rotateLeft(avo));
							FixSize(bisavo);
						}
						else{
							FixRight(bisavo, rotateLeft(avo));
							FixSize(bisavo);
						}
					}
				}
			}
		}
		return(NULL);
	}



	RedBlackNode* insereNo(RedBlackNode* root, wstring chave, long valor){ //devolve a raiz da arvore com o nó inserido.

		RedBlackNode* temp = root;
		RedBlackNode* temp2;

		if (temp == NULL){
			temp = new RedBlackNode(chave, valor, 1);
			this->size = this->size + 1;
			return temp;
		}

		while (temp != NULL){
			if (temp->getKey() < chave){
				temp2 = temp;
				temp = temp->getRight();
			}
			else if (temp->getKey() > chave){
				temp2 = temp;
				temp = temp->getLeft();
			}
			else if (temp->getKey() == chave){
				temp->setVal(temp->getVal() + valor);
				return (root);
			}
		}
		//temp == NULL, temp2 é o pai do nodo a ser inserido
		RedBlackNode* new_node = new RedBlackNode(chave, valor, 1);
		this->size = this->size + 1;
		if (temp2->getKey() < chave){
			FixRight(temp2, new_node);
			FixSize(temp2);

		}
		else{
			FixLeft(temp2, new_node);
			FixSize(temp2);

		}
		RedBlackNode* flag = new_node;
		while (new_node != NULL){
			new_node = FixColour(new_node);
		}
		while (flag != NULL){
			FixSize(flag);
			flag = flag->getParent();
		}
		return root;
	}



	void insere (wstring chave, long valor){

		RedBlackNode* root = this->getRoot();

		this->setRoot(insereNo(root, chave, valor));

	}
	

	long devolve(wstring chave){

		RedBlackNode* temp = this->getRoot();

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
		wcout << L"Chave nao encontrada" << endl;
		return (-1);

	}


	RedBlackNode* FixDoubleBlack(RedBlackNode* double_black){
	//Recebe um nó duplo preto e resolve o problema de cor. Retorna o próximo nó que deve ser avaliado.

		RedBlackNode* brother;
		RedBlackNode* parent;
		RedBlackNode* grandparent;
		RedBlackNode* nearest_nephew;
		RedBlackNode* furthest_nephew;
		long parent_colour;

		if (double_black == NULL){
			wcout << L"FixDoubleBlack(RedBlackNode* double_black) chamado em NULL, erro" << endl;
			return NULL;
		}

		//pegando o pai:
		parent = double_black->getParent();
		if (parent == NULL){
			//double_black é raiz
			double_black->setColour(1);
			return double_black;
		}
		//pegando o irmao:
		if (parent->getRight() == double_black){
			brother = parent->getLeft();
		}
		else if (parent->getLeft() == double_black){
			brother = parent->getRight();
		}
		if (brother == NULL){
			//irmão preto, filhos pretos
			//brother->setColour(1);//vermelho de mentirinha
			double_black->setColour(0);//preto
			if (GetColour(parent) == 0){
				//preto vira duplo-preto
				parent->setColour(2);
			}
			else if (GetColour(parent) == 1){
				//vermelho vira preto
				parent->setColour(0);
			}
			return(parent);
		}
		//pegando os sobrinhos:
		if (parent->getLeft() == double_black){
			nearest_nephew = parent->getRight()->getLeft();
			furthest_nephew = parent->getRight()->getRight();
		}
		else if (parent->getRight() == double_black){
			nearest_nephew = parent->getLeft()->getRight();
			furthest_nephew = parent->getLeft()->getLeft();
		}

		//resolvendo duplos pretos
		if (GetColour(brother) == 0 && GetColour(brother->getLeft()) == 0 && GetColour(brother->getRight()) == 0){
			//irmao preto, filhos pretos
			brother->setColour(1);//vermelho
			double_black->setColour(0);//preto
			if (GetColour(parent) == 0){
				//preto vira duplo-preto
				parent->setColour(2);
			}
			else if (GetColour(parent) == 1){
				//vermelho vira preto
				parent->setColour(0);
			}
			return (parent);
		}
		else if (GetColour(brother) == 1){
			//irmao vermelho, filhos obrigatoriamente pretos
			//troca a cor do pai e do irmao
			parent_colour = GetColour(parent);
			parent->setColour(GetColour(brother));
			brother->setColour(parent_colour);
			//rotaciona para o lado do duplo preto. 
			grandparent = parent->getParent();
			if (grandparent == NULL){
				//parent é a raiz;
				if (parent->getLeft() == double_black){
					rotateLeft(parent);
				}
				else if (parent->getRight() == double_black){
					rotateRight(parent);
				}
			}
			else if (grandparent != NULL){
				//precisamos acertar o ponteiro de grandparent para parent depois de rotacionar
				if (parent->getLeft() == double_black){
					if (grandparent->getLeft() == parent){
						FixLeft(grandparent, rotateLeft(parent));
						FixSize(grandparent);
					}
					else if (grandparent->getRight() == parent){
						FixRight(grandparent, rotateLeft(parent));
						FixSize(grandparent);
					}
				}
				else if (parent->getRight() == double_black){
					if (grandparent->getLeft() == parent){
						FixLeft(grandparent, rotateRight(parent));
						FixSize(grandparent);
					}
					else if (grandparent->getRight() == parent){
						FixRight(grandparent, rotateRight(parent));
						FixSize(grandparent);
					}
				}
			}
			//agora o duplo preto tem irmao preto, e seu ponteiro nao mudou, vamos retorná-lo para que o caso em que caímos seja adequadamente tratado.
			return(double_black);
		}
		else if (GetColour(brother) == 0 && GetColour(nearest_nephew) == 1 && GetColour(furthest_nephew) == 0){
			//irmao preto, sobrinho proximo vermelho, sobrinho distante preto
			brother->setColour(1);
			nearest_nephew->setColour(0);

			if (parent->getRight() == brother){
				//duplo preto está à esquerda
				FixRight(parent, rotateRight(brother));
				FixSize(parent);
			}
			else if (parent->getLeft() == brother){
				//duplo preto está à direita
				FixLeft(parent, rotateLeft(brother));
				FixSize(parent);
			}
			return (double_black);
		}
		else if (GetColour(brother) == 0 && GetColour(furthest_nephew) == 1){
			//irmao preto, sobrinho distante vermelho
			//troca cor do pai com irmao
			parent_colour = GetColour(parent);
			parent->setColour(0);
			brother->setColour(parent_colour);
			//rotaciona pro lado do duplo preto.
			grandparent = parent->getParent();
			if (grandparent == NULL){
				//parent é a raiz
				if (parent->getLeft() == double_black){
					rotateLeft(parent);
				}
				else if (parent->getRight() == double_black){
					rotateRight(parent);
				}
				double_black->setColour(0); //preto
				furthest_nephew->setColour(0); //preto, era vermelho
				//duplo preto eliminado
				double_black = parent->getParent(); //agora é não null
				return (double_black); //nova raiz
			}
			else if (grandparent != NULL){
				if (grandparent->getLeft() == parent){
					if (parent->getLeft() == double_black){
						FixLeft(grandparent, rotateLeft(parent));
						FixSize(grandparent);
					}
					else if (parent->getRight() == double_black){
						FixLeft(grandparent, rotateRight(parent));
						FixSize(grandparent);
					}
				}
				else if (grandparent->getRight() == parent){
					if (parent->getRight() == double_black){
						FixRight(grandparent, rotateRight(parent));
						FixSize(grandparent);
					}
					else if (parent->getLeft() == double_black){
						FixRight(grandparent, rotateLeft(parent));
						FixSize(grandparent);
					}
				}
				double_black->setColour(0); //preto
				furthest_nephew->setColour(0); //preto, era vermelho
				double_black = parent->getParent();
				return (double_black);
			}


		}
		return NULL;
	}


	RedBlackNode* removeNo(RedBlackNode* root, wstring chave){
	//retorna a raiz da nova árvore com o nó removido. Precisamos atualizar a raiz na função chamadora.
		RedBlackNode* temp;
		RedBlackNode* return_value = root;

		if (root == NULL){
			wcout << L"removeNo pedido em arvore vazia, erro!" << endl;
			return NULL;
		}

		temp = root;

		while (temp != NULL){
			if (temp->getKey() > chave){
				temp = temp->getLeft();		
			}
			else if (temp->getKey() < chave){
				temp = temp->getRight();
			}
			else if (temp->getKey() == chave){
			return_value = Replace(temp);
			break;
			}
		}
		if (temp == NULL){
			//wcout << "Chave a ser removida nao encontrada." << endl;
			return root;
		}
		if (return_value == NULL){
			wcout << "Tree Empty" << endl;
			return return_value;
		}
		FixSize(return_value);
		//se chegamos aqui, alguem foi removido.
		while (GetColour(return_value) > 1){
			return_value = FixDoubleBlack(return_value); //consertamos o duplo preto em return_value. Retorna o proximo nó a ser consertado.
			if (return_value == NULL){
				wcout << L"return_value assumiu NULL em removeNo(RedBlackNode* root, wstring chave). Erro." << endl;
				return(NULL);
			}
		}
		temp = return_value->getParent();
		while (temp != NULL){
			return_value = temp;
			FixSize(return_value);
			temp = temp->getParent();
		}
		return (return_value); //nova raiz
	}


	void remove(wstring chave){

		RedBlackNode* root = this->getRoot();
		long subtree_size = SubtreeSize(root);
		root = removeNo(root, chave);
		if (subtree_size == SubtreeSize(root)){
			//wcout << L"Tamanho da arvore nao mudou. Possivel erro." << endl;
		}
		if (root == NULL){
			wcout << L"remove retornou valor NULL. Erro." << endl;
			return;
		}
		this->setRoot(root); 
	
	}

	long rank(wstring chave){

		RedBlackNode* temp = getRoot();
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
		wcout << L"chave nao encontrada" << endl; 
		return rank;
	}


	wstring select(long given_rank){

		RedBlackNode* temp = getRoot();
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
		wcout << L"Chave nao encontrada" << endl;
		return L"";

	}
	void enqueue_children(RedBlackNode* node, RBQueue*& print_queue){
		if (node == NULL){
			return;
		}

		print_queue->Enqueue(node->getLeft());
		print_queue->Enqueue(node->getRight());
		return;

	}

	void print(){

		RBQueue* print_queue = new RBQueue;
		print_queue->Enqueue(this->root);
		while(print_queue->QueueEmpty() == 0){
			RedBlackNode* head = print_queue->Dequeue();
			if (head == NULL){
				wcout << L"Filho NULL. ";
				continue;
			}
			wcout << L"Palavra " << head->getKey() << L" tem valor " << head->getVal() << L" e nó é ";
			if (GetColour(head) == 0){
				wcout << L"preto. ";
			}
			else{
				wcout << L"vermelho. ";
			}
			if (head->getParent() != NULL){
				wcout << L"Pai de " << head->getKey() << L": " << head->getParent()->getKey() << L". ";
			}
			enqueue_children(head, print_queue);
		}
		delete print_queue;
	}

};