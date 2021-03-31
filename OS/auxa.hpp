#include <string>
#include <vector>
#include <iostream>
#include "definitions.hpp"
#include <sys/stat.h>

using namespace std;

#ifndef AUX_HPP
#define AUX_HPP

// get_path (/home/ju/test.txt) --> /home/ju/
// get_path (/home/ju/) --> /home/ju/
string get_path (string s);

// finds the first free block at bitmap
int find_free_block (vector<int> bitmap);

// splits the string input and returns the first token
string tokenize(string& input, string separator);

// este cara lê o texto e devolve os blocos
vector<string> read(ifstream file, string name);

void setRoot (Diretorio<Arquivo>* root, vector<string> blocks,  vector<int> FAT);

void exec_mkdir(string dir, Diretorio<Arquivo>* root);

void exec_rmdir(string dir, Diretorio<Arquivo>* root);

void exec_ls(string path, Diretorio<Arquivo>* root);

vector<string> parse (string origin);

void find(string path, string target, Diretorio<Arquivo>* root);

Diretorio<Arquivo>* searchDir(Diretorio<Arquivo>* root, string path);

Diretorio<Arquivo>* searchDirRec(Diretorio<Arquivo>* root, string path);

int dir_amount = 0;

void lsPrint(Diretorio<Arquivo>* root);

void exec_mkdirRec(string dir, Diretorio<Arquivo>* root);

void exec_rmdirRec(string dir, Diretorio<Arquivo>* root);

void findRec(Diretorio<Arquivo>* source, string target);

void build_block (Diretorio<Arquivo>* pai, vector<string> blocks, vector<int> FAT) ;

#endif

bool isDir (string s) {

	string aux = tokenize (s, "|");
	return (aux == "dir");

}

// arq.txt --> true
// /home/ --> false
bool is_arq (string s) {

	int n = s.length();

	if ( n > 4 && s[n-4] == '.' && s[n-3] == 't' && s[n-2] == 'x' && s[n-1] == 't')
		return true;

	return false;

}


string get_path (string s) {

    string path;
    stringstream aux (s);
    string buf;

    while (getline(aux, buf, '/')) {

		if (!is_arq(buf)) {
        	path += buf;
        	path += "/"; 
		}
    }
    
    return path;

}

int find_free_block (vector<int> bitmap) {

    int i = 0;

	while (i < N_BLOCKS && bitmap[i] == 0) {
		i++;
	}

    return i;
}

string tokenize(string& input, string separator) {

	int separator_pos = input.find(separator);
	string token;
	if (separator_pos == -1){
		separator_pos = input.length();
	}
	if (separator_pos == 0){
		token = input.substr(0, separator_pos + 1); 
	}
	else{
		token = input.substr(0, separator_pos);
	}
	input.erase(0, separator_pos + 1);
	return token;
}

vector<string> read(string name) { // este cara lê o texto e devolve os blocos
	
	ifstream file;

	vector<string> blocks (N_BLOCKS);
	file.open(name);
	int count = 0;
	bool end = false;

	// bitmap
	for (; count < BITMAP_SIZE && !end; count++) {

		char c;

		while (true) { 

			file.get(c);
			string aux (1,c);
			if (aux == BLOCK_TERMINATOR)
				break;

			if (c == ' ') {
				count--;
				end = true;
				file.putback(c);
				break;
			}

			blocks[count] += c; 

		} 
	}

	end = false;

	// FAT
	for (; !end; count++) {

		char c;

		while (true) {

			file.get(c);
			string aux (1,c);
			if (aux == BLOCK_TERMINATOR)
				break;

			if (aux == ARQ_SEPARATOR) {
				count--;
				file.putback(c);
				end = true;
				break;
			}

			blocks[count] += c;
		}

	}

	string parser;

	// arquivos
	for (; count < N_BLOCKS && parser != TERMINATOR; count++) {

		while (file >> parser && parser != TERMINATOR && parser != BLOCK_TERMINATOR) {

			blocks[count] += parser;
			blocks[count] += " ";

		}

	}

	file.close();

	return blocks;
}

vector<int> get_bitmap (vector<string> blocos, int& count) {

	vector<int> bitmap (N_BLOCKS, 1);

	for (int i = 0, pos = 0; i < BITMAP_SIZE; i++) { 

		string at = blocos[i];
        
        for (int j = 0; j < (int) blocos[i].size(); j++) {
			
            bitmap[pos++] = at[j]-'0';
            if (!bitmap[pos - 1])
                count++; 
        }   
          
    }

	return bitmap;

}

vector<int> getFAT (vector<string> blocks) {

	vector<int> FAT (N_BLOCKS, -1);
	string atual;

	for (int i = 6, pos = 0; i < 12; i++) {

		atual = tokenize(blocks[i], " "); // first ' ' of the block

		for (int j = 0; j < (int) blocks.size() && pos < N_BLOCKS; j++) {

			atual = tokenize(blocks[i], " ");


			if (atual.size() == 0)
				break;
				
			FAT[pos++] = stoi(atual);
		}
			

	}
	
	return FAT;

}

void setRoot (Diretorio<Arquivo>* root, vector<string> blocks,  vector<int> FAT) {

	int i = 12; // TEM QUE MUDAR ISSO

	string cont = tokenize (blocks[i], " ");
	cont = tokenize (blocks[i], " ");

	string aux = tokenize (cont, "|"); // primeiro "|"
	aux = tokenize (cont, "|"); // dir
	aux = tokenize (cont, "|"); // /
	
	aux = tokenize (cont, "|"); // creation time
	root->setCreationTime((time_t) stoi(aux));

	aux = tokenize (cont, "|"); // modification time
	root->setModifTime((time_t) stoi(aux));

	aux = tokenize (cont, "|"); // modification time
	root->setAccessTime((time_t) stoi(aux));

	aux = tokenize (cont, "|"); // first block at fat
	root->block = stoi(aux);

	// Daqui
	root->occupied_size = DIRECTORY_METADATA_SIZE;
	used_size += DIRECTORY_METADATA_SIZE;
	used_blocks++; //bitmap + fat + root
	directories = 1; //root
	files = 0;
	// Até aqui

	build_block(root,blocks, FAT);


}

void build_block (Diretorio<Arquivo>* pai, vector<string> blocks, vector<int> FAT) {

	int i = pai->block;

	while (blocks[i].size() > 0) {

		string atual = tokenize (blocks[i], " ");
		string def;
		tokenize (atual, "|"); 
		def = tokenize (atual, "|");

		string nome = tokenize (atual, "|");
		time_t creation = (long) stoi(tokenize (atual, "|"));
		time_t modif = (long) stoi(tokenize (atual, "|"));
		time_t access = (long) stoi(tokenize (atual, "|"));


		if (isDir(def)) { // dir --> Edu|5405210|5468798|549865463|8|

			int first = stoi(tokenize (atual, "|"));

			// Diretorio<Arquivo> novo (nome, pai);

			// string path = ((pai->getName() == "/") ? ("/") : (pai->getPath() + pai->getName() + "/"));
			
			// exec_mkdir(path + nome, pai);

			exec_mkdir("/" + nome, pai);
			Diretorio<Arquivo>* novo = pai->findDir(nome);

			novo->setCreationTime(creation);
			novo->setModifTime(modif);
			novo->setAccessTime(access);
			novo->block = first;

			// DAQUI
			directories++;
			used_blocks++;
			used_size = used_size + DIRECTORY_METADATA_SIZE;
			// ATE AQUI

			build_block (novo, blocks, FAT);			

		}

		else { // arquivo --> texto.txt|13478175|8174781|2767814|273821|5|

			int size = stoi(tokenize (atual, "|"));
			int first = stoi(tokenize (atual, "|"));
			int aux = first;
			string data;

			if (blocks[aux].length() > 0)
					blocks[aux].pop_back();
					
			data += blocks[aux];

			// DAQUI
			used_blocks++;
			// ATE AQUI

			while (FAT[aux] != -1) {

				aux = FAT[aux];

				if (blocks[aux].length() > 0)
					blocks[aux].pop_back();
				data += blocks[aux];

				// DAQUI
				used_blocks++;
				// ATE AQUI
				
			}

			// DAQUI
			files++;
			used_size = used_size + FILE_METADATA_SIZE + size;
			// ATE AQUI

			try{
				Arquivo novo (nome, data, size, creation, modif, access, first, pai);
				pai->addArq(novo);
			}
			catch (int e){
				if (e == 1){
					return;
				}
			}

		}

	}

}

void exec_mkdir(string dir, Diretorio<Arquivo>* root) { //recursion wrapper

	string current_directory = tokenize(dir, "/");
	if (current_directory != "/"){
		cout << "Digite o caminho corretamente. deve comecar com /." << endl;
		return;
	}
	if (current_directory == "/" && dir.length() == 0){
		cout << "Nao e possivel criar o diretorio /." << endl;
		return;
	}
	if (dir.length() == 0){
		cout << "digite o caminho corretamente." << endl;
		return;
	}

	exec_mkdirRec(dir, root);
}

void exec_mkdirRec(string dir, Diretorio<Arquivo>* root){

	if (dir.length() == 0){
		return;
	}

	string ndir = tokenize(dir, "/");
	Diretorio<Arquivo>* next_directory = root->findDir(ndir); //procura o diretorio ndir em root
	if (next_directory == NULL){
		try{
		Diretorio<Arquivo> novo(ndir, root); //cria o novo diretorio em root
		}
		catch(int e){
			if (e == 1){
				return;
			}
		}
		next_directory = root->findDir(ndir); //&novo
		dir_amount++;
	}
	exec_mkdirRec(dir, next_directory);

}

void exec_rmdirRec(string dir, Diretorio<Arquivo>* root) {
	
	string ndir = tokenize(dir, "/");

	Diretorio<Arquivo>* next_directory = root->findDir(ndir); //procura o diretorio ndir em root
	if (dir.length() == 0){

		list<Diretorio<Arquivo>>::iterator it;
		for (it = next_directory->dirs.begin(); it != next_directory->dirs.end(); ++it){
			if ((*it).getName() == ndir){
				next_directory->dirs.erase(it);
			}
		}
		return;
	}
	else if (next_directory == NULL){
		cout << "invalid path to target." << endl;
		return;
	}
	else{
		exec_rmdirRec(dir, next_directory);
	}

}

void exec_rmdir(string dir, Diretorio<Arquivo>* root) {
	string current_directory = tokenize(dir, "/");
	if (current_directory != "/"){
		cout << "Digite o caminho corretamente. deve comecar com /." << endl;
		return;
	}
	if (current_directory == "/" && dir.length() == 0){
		cout << "Nao e possivel remover o diretorio /." << endl;
		return;
	}
	if (dir.length() == 0){
		cout << "digite o caminho corretamente." << endl;
		return;
	}
	exec_rmdirRec(dir, root);
}

void lsPrint(Diretorio<Arquivo>* root){

	iter(Diretorio<Arquivo>) it;
	for (it = root->dirs.begin(); it != root->dirs.end(); ++it){
		time_t timet = (*it).getModifTime();
		tm modif = *localtime(&timet);
		cout << "Dir: \n    " << (*it).getName() << endl;
		cout << "    Última modificação: " << put_time(&modif, "%R %D") << endl;
	}
	list<Arquivo>::iterator it2;
	for (it2 = root->arqs.begin(); it2 != root->arqs.end(); ++it2){
		
		(*it2).show();

	}
}

void exec_ls(string path, Diretorio<Arquivo>* root) {

	string current_directory = tokenize(path, "/");
	if (current_directory != "/"){
		cout << "Digite o caminho corretamente. deve comecar com /." << endl;
		return;
	}
	while (path.length() > 0){
		string ndir = tokenize(path, "/");
		Diretorio<Arquivo>* next_directory = root->findDir(ndir); //procura o diretorio ndir em root
		if (next_directory == NULL){
			cout << "Caminho incorreto recebido. " << ndir << " nao foi encontrado." << endl;
			return;
		}
		root = next_directory;
	}
	lsPrint(root);

	cout << endl;

}

void findRec(Diretorio<Arquivo>* source, string target){

	if (source->dirEmpty()){
		return;
	}
	
	Arquivo* arq = source->findArq(target);
	if (arq != NULL){
		cout << arq->getPath() << endl;  
	}                           

	if (source->dirs.empty()){
		return;
	}

	list<Diretorio<Arquivo>>::iterator it;
	for (it = source->dirs.begin(); it != source->dirs.end(); ++it){
		Diretorio<Arquivo>* next = source->findDir((*it).getName());
		findRec(next, target);
	}
	
	return;
}

void find (string path, string target, Diretorio<Arquivo>* root){
	
	Diretorio<Arquivo>* source = searchDir(root, path);
	findRec(source, target);
	
}

// clear the s vector
void clean (char* s, int size) {

	for (int i = 0; i < size; i++)
		s[i]='\0';

}

void arq_info (string name, time_t& c, int& n_blocks, string& data, int& size) {

	ifstream arq;
	string atual;

	// try to open the file
    arq.open(name);
    if (!arq.is_open()) {
        cout << "Não foi possível abrir o arquivo\n";
        return;
    }

	// get the data
	
    while (getline(arq, atual)) {
        data += atual;
    }

	// calculate the number of blocks
	n_blocks = (data.size()) / BLOCK_SIZE;
	n_blocks += (n_blocks*BLOCK_SIZE < (int) data.size());

	struct stat file;

	// cast: string --> char*
	char nome [name.size()+1];
	for (int i = 0; i < (int) name.size() + 1; i++)
		nome[i] = name[i];

	if (lstat(nome, &file) == -1) {
		cout << "Erro ao ler as informacoes do arquivo :(\n";
		return;
	}

	c = (file.st_ctim).tv_sec;

	size = data.length();

	arq.close();
}

void fill_blocks_rec (vector<string>& blocks, vector<int> FAT, Diretorio<Arquivo>* root) {

	for (iter(Diretorio<Arquivo>) it = root->dirs.begin(); it != root->dirs.end(); ++it) {

		blocks[root->block] += " |dir|" + (*it).getName() + "|";
		blocks[root->block] += to_string((*it).getCreationTime()) + "|" + to_string((*it).getModifTime()) + "|";
		blocks[root->block] += to_string((*it).getAccessTime()) + "|" + to_string((*it).block) + "|";

		fill_blocks_rec(blocks, FAT, &(*it));

	}

	for (iter(Arquivo) it = root->arqs.begin(); it != root->arqs.end(); ++it) {

		blocks[root->block] += " |arq|" + (*it).getName() + "|";
		blocks[root->block] += to_string((*it).getCreationTime()) + "|" + to_string((*it).getModifTime()) + "|";
		blocks[root->block] += to_string((*it).getAccessTime()) + "|";
		blocks[root->block] += to_string((*it).getSize()) + "|" + to_string((*it).get_block()) + "|";

		int bloco = (*it).get_block();
		for(int i = 0; bloco != -1; i += BLOCK_SIZE, bloco = FAT[bloco]) {
			blocks[bloco] = (*it).data.substr(i, BLOCK_SIZE);
		}

	} 

}

void fill_blocks (vector<string>& blocks, vector<int> bitmap, vector<int> FAT, int& count, Diretorio<Arquivo>* root) {


	string atual;

	// bitmap
    for (int i = 0; i < (int) bitmap.size(); i++) {

		atual += to_string(bitmap[i]);
        
        if ((i+1) % BLOCK_SIZE == 0) {
            blocks[count++] = atual;
            atual.clear();
        }
    }

	if (!atual.empty())
		blocks[count++] = atual;

	atual.clear();

	// FAT
	for (int i = 0; i < (int) FAT.size(); i++) {

		atual += " ";
		atual += to_string(FAT[i]);

		if ((i+1) % BLOCK_SIZE == 0) {
            blocks[count++] = atual + " ";
            atual.clear(); // atual = ""
        }
	}

	if (!atual.empty())
		blocks[count++] = atual + " ";

	// root
	blocks[count] = "|dir|/|" + to_string(root->getCreationTime()) + "|" ;
	blocks[count] += to_string(root->getModifTime()) + "|" + to_string(root->getAccessTime()) + "|";
	blocks[count] += to_string(root->block) + "|";

	fill_blocks_rec(blocks, FAT, root);

}


Diretorio<Arquivo>* searchDirRec(Diretorio<Arquivo>* root, string path){

	string ndir = tokenize(path, "/");
	Diretorio<Arquivo>* next_directory = root->findDir(ndir);
	if (next_directory == NULL){
		cout << "Caminho incorreto. Diretorio " << ndir << " nao foi encontrado." << endl;
		return NULL;
	}
	if (path.length() == 0){
		return next_directory;
	}
	return (searchDirRec(next_directory, path));

}

Diretorio<Arquivo>* searchDir(Diretorio<Arquivo>* root, string path){ //recebe um ponteiro para um diretorio raiz e um caminho /raiz/etc/etc2/etc3 e devolve um ponteiro para etc3 se o caminho estiver correto.
	
	string current_directory = tokenize(path, "/");                   //O primeiro diretorio no caminho deve ser o diretorio apontado pelo ponteiro recebido.
	
	if (current_directory != "/"){
		cout << "Digite o caminho corretamente. deve comecar com /." << endl;
		return NULL;
	}
	
	if (current_directory == "/" && path.length() == 0){
		return root;
	}
	
	Diretorio<Arquivo>* target = searchDirRec(root, path);
	return target;
}
