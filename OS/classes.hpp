#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <list>


using namespace std;

#ifndef _CLASSES_HPP_
#define _CLASSES_HPP_

#define iter(tipo) typename list<tipo>::iterator 

template<class T>
class Diretorio{

    private:

	string name;          
	time_t creation_time;    
	time_t modif_time;       
	time_t access_time;      
	Diretorio<T>* parent;

    public:

    int block;            // bloco na memoria ocupados por este objeto
    list<T> arqs;         // arquivos neste diretório
	list<Diretorio<T>> dirs; // diretórios neste diretório
	int occupied_size;

    // contructor
	Diretorio(string name, Diretorio<T>* pai);

    // destructor
	~Diretorio();

	string getName();
	void setName(string name);
	
	time_t getCreationTime();
	void setCreationTime(time_t creation_time);

	time_t getModifTime();
	void setModifTime(time_t time);

	time_t getAccessTime();
	void setAccessTime(time_t time);

	void printTime(time_t t);

    void addArquivo (T arq);

    Diretorio<T>* getParent();
    void setParent(Diretorio<T>* parent);

    void printTime();
    void printArqs();
    void printDirs();

    int dirEmpty();

    Diretorio<T>* findDir(string target_name);

    T* findArq(string target_name);

    string getPath();

    void addDir(Diretorio<T> diretorio);

    // existing arq
    void addArq(T arquivo);

    // new arq
    void createArq (T Arquivo);    

    // removeArq(arquivo.txt) --> removes arquivo.txt from the dir 
    void removeArq (string arq);

    int getOccupation(){
    	return this->occupied_size;
    }

};

class Arquivo{

	int block;              // first block at FAT 
	string name;            
	int size;               // in bytes
	time_t creation_time;   // in seconds
	time_t modif_time;      // last modification in seconds
	time_t access_time;     // last access in seconds            
	       
 
    public:

    string data;
    Diretorio<Arquivo>* dad;  

    // create an Arquivo
    // 1. name
    // 2. data
    // 3. size
    // 4. creation time
    // 5. modification time
    // 6. access time
    // 7. initial block at FAT
	Arquivo (string name, string data, int size, int c, int m, int a, int ini, Diretorio<Arquivo>* dad);

	~Arquivo();

	string getName();
	void setName(string name);

	int getSize();
	void setSize(int size);

	time_t getCreationTime();
	void setCreationTime(time_t creation_time);

	time_t getModifTime();
	void setModifTime(time_t time);

	time_t getAccessTime();
	void setAccessTime(time_t time);

	string getData();
	void setData(string data);

    // prints some information of the Arquivo:
    // name, size (bytes) and last modification time
	void show();
	
    // returns the first block at FAT 
    int get_block ();

    string getPath();

};


// Globals
long used_size = 0;
long base_used_size;
long used_blocks = 0; 
long base_used_blocks = 0;
long base_wasted_space = 0;
long directories = 0;
long files = 0;

#endif

/********     IMPLEMENTATION OF ARQUIVO       *******/

Arquivo::Arquivo (string name, string data, int size, int c, int m, int a, int ini, Diretorio<Arquivo>* dad) {

	if (name.length() > 100){
		cout << "Tamanho maximo do nome de arquivos: " << MAX_NAME <<" caracteres" << endl;
		return;
	}

    block = ini;
    this->name = name;
    this->size = size;
    creation_time = c;
    modif_time = m;
    access_time = a;
    this->data = data;
    this->dad = dad;

    if (dad != NULL){
   		if (dad->occupied_size + FILE_METADATA_SIZE > 4096){
   			cout << "Dir " << dad->getName() << " has no available space for a file. File " << this->name << "was not created." << endl;
    		throw(1);
    	}
    }

}

Arquivo::~Arquivo(){
    this->dad = nullptr;
}

string Arquivo::getName() {
    return this->name;
}

void Arquivo::setName (string name) {
    this->name = name;
}

int Arquivo::getSize () {
    return this->size;
}

void Arquivo::setSize (int size) {
    this->size = size;
}

time_t Arquivo::getCreationTime () {
    return this->creation_time;
}

void Arquivo::setCreationTime (time_t creation_time) {
    this->creation_time = creation_time;
}

time_t Arquivo::getModifTime () {
    return this->modif_time;
}

void Arquivo::setModifTime (time_t time) {
    this->modif_time = time;
}

time_t Arquivo::getAccessTime () {
    return this->access_time;
}

void Arquivo::setAccessTime (time_t time) {
    this->access_time = time;
}

string Arquivo::getData () {
    return this->data;
}

void Arquivo::setData (string data) {
    this->data = data;
}

int Arquivo::get_block() {
    return this->block;
}

void Arquivo::show() {

    tm m = *localtime(&this->modif_time);

    cout << "\nArquivo:\n";
    cout << "    Nome: " << this->name << endl;
    cout << "    Tamanho: " << this->size << " bytes\n";
    cout << "    Última modificação: " <<  put_time(&m, "%R %D") << endl;

}

string Arquivo::getPath() {

    if (this->dad->getName() == "/")
        return "/" + name;
    return this->dad->getPath() + this->dad->getName() + "/" + name;

}

/*******      IMPLEMENTATION OF DIR        ******/



template<class T>
Diretorio<T>::Diretorio(string name, Diretorio<T>* pai){

	if (name.length() > MAX_NAME){
		cout << "Tamanho maximo do nome de diretorios: " << MAX_NAME << " caracteres" << endl;
		return;
	}

    this->name = name;
    this->parent = pai;
    this->creation_time = time(NULL);
    this->modif_time = time(NULL);
    this->access_time = time(NULL);
    this->arqs = list<T>();
    this->dirs = list<Diretorio<T>>();
    this->occupied_size = 0;


    if (pai != NULL){
        pai->dirs.push_back(*this);
    }

    if (pai != NULL){
    	if (pai->occupied_size + DIRECTORY_METADATA_SIZE > 4096){
    		cout << "Dir " << pai->name << " has no available space for a directory. Directory " << this->name << "was not created." << endl;
    		throw(1);
    	}
    	pai->occupied_size = pai->occupied_size + DIRECTORY_METADATA_SIZE;
    }

}

template<class T>
Diretorio<T>::~Diretorio(){
    
}

template<class T>
string Diretorio<T>::getName(){
    return this->name;
}

template<class T>
void Diretorio<T>::setName(string name){
    this->name = name;
}

template<class T>
time_t Diretorio<T>::getCreationTime(){
    return this->creation_time;
}

template<class T>
void Diretorio<T>::setCreationTime(time_t creation_time){
    this->creation_time = creation_time;
}

template<class T>
time_t Diretorio<T>::getModifTime(){
    return this->modif_time;
}

template<class T>
void Diretorio<T>::setModifTime(time_t time){
    this->modif_time = time;
}

template<class T>
time_t Diretorio<T>::getAccessTime(){
    return this->access_time;
}

template<class T>
void Diretorio<T>::setAccessTime(time_t time){
    this->access_time = time;
}

// ***
template<class T>
void Diretorio<T>::printTime (time_t t) {
    tm creation  = *localtime(&this->creation_time);
    tm modif = *localtime(&this->modif_time);
    tm access = *localtime(&this->access_time);
    cout << " Criado: " << put_time(&creation, "%R %D");
    cout << " modificado: " << put_time(&modif, "%R %D");
    cout << " acessado: " << put_time(&access, "%R %D") << endl;
}

template<class T>
void Diretorio<T>::printArqs(){
    typename list<T>::iterator it;
    for (it = this->arqs.begin(); it != this->arqs.end(); ++it){
        cout << " " << (*it).getName();
    }
    cout << endl;
}

template<class T>
void Diretorio<T>::printDirs(){
    typename list<Diretorio<T>>::iterator it;
    for (it = this->dirs.begin(); it != this->dirs.end(); ++it){
        cout << " " << (*it).name;
    }
    cout << endl;
}

template<class T>
int Diretorio<T>::dirEmpty(){
    if (this->arqs.empty() == 1 && this->dirs.empty() == 1){
        return 1;
    }
    else{
        return 0;
    }
}

template<class T>
Diretorio<T>* Diretorio<T>::findDir(string target_name){

    typename list<Diretorio<T>>::iterator it;
    for (it = this->dirs.begin(); it != this->dirs.end(); ++it){
        if ((*it).name == target_name){
            return &(*it);
        }
    }

    return (nullptr);
}

template<class T>
T* Diretorio<T>::findArq(string target_name){
    typename list<T>::iterator it;
    for (it = this->arqs.begin(); it != this->arqs.end(); ++it){
        if ((*it).getName() == target_name){
            return &(*it);
        }
    }

    return (NULL);
}

template<class T>
string Diretorio<T>::getPath(){
    Diretorio<T>* parent = this->parent;
    if (parent == NULL){
        return "Root path is empty";
    }
    if (parent->getParent() == NULL){
        return "/";
    }
    string path;
    while (parent->getParent() != NULL){

        path = "/" + parent->name + path;
        parent = parent->getParent();
        
    }
    return path;
}

template<class T>
Diretorio<T>* Diretorio<T>::getParent(){
    return this->parent;
}

template<class T>
void Diretorio<T>::setParent(Diretorio<T>* parent){
    this->parent = parent;
}

template<class T>
void Diretorio<T>::addDir(Diretorio<T> diretorio){
	if (this->occupied_size + 151 > 4096){
		cout << "Não foi possível criar o diretório (diretório pai está cheio)\n" << endl;
		return;
	}
    this->dirs.push_back(diretorio);
    this->occupied_size = this->occupied_size + 151;
}

template<class T>
void Diretorio<T>::createArq(T arquivo){
    this->arqs.push_back(arquivo);
    this->access_time = time(NULL);
}

template<class T>
void Diretorio<T>::addArq(T arquivo){
	if (this->occupied_size + 162 > 4096){
		cout << "Não foi possível criar o diretório (diretório pai está cheio)\n" << endl;
		return;
	}
    this->arqs.push_back(arquivo);
    this->occupied_size = this->occupied_size + 162;
}

template<class T>
void Diretorio<T>::removeArq (string arq) {


    for (iter(T) it = this->arqs.begin(); it != this->arqs.end(); ++it)
        if (arq == (*it).getName()) {
            this->arqs.erase(it);
            return;
        }

}
