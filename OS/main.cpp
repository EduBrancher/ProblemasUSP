// EP 3

// Julia Leite
// Eduardo Brancher

#include <iostream>
#include "definitions.hpp" 
#include "classes.hpp"
#include "auxa.hpp"

using namespace std;

fstream file;
string filename;

// FAT
vector<int> FAT;

// Bitmap
vector<int> bitmap;

// Root
Diretorio<Arquivo>* root = nullptr;


//funções
void init(); //inicializa as estruturas de dados
void mount (string arq);
void cp (string origin, string dest);
void mkdir (string dir);
void rmdir (string dir);
void cat (string arq);
void touch (string arq);
void rm (string arq);
void ls (string dir);
void df ();
void umount ();



int main () {

    /*fim da sujeira*/
    //nota: para escrever um long, são necessários 20 bytes. Logo os metadados de um diretorio (formato |dir|nome|int|int|int|int|) ocuparão no maximo 121 bytes (3 para dir, 30 para nome, 80
    //poelos inteiros, 7 para os separadores e um espaço. Cada bloco ocupa na prática 4097 bytes por causa do separador de bloco.) Os metadados de um arquivo ocuparão no máximo 
    //formato |arq|nome|int|int|int|int|int| ocuparão 142 bytes (3 para arq, 30 para nome, 100 para os inteiros, 8 para os separadores, 1 espaço). Logo a combinação desses não pode passar de 4096.

    string cmd;

    while (cmd != "sai") {

        cout << PROMPT; 

        cin >> cmd;

        // Case 1: mount <arq>
        if (cmd == "mount") {

 
            cin >> filename;


            mount (filename);
            

        }

        // Case 2: cp <origin> <dest>
        else if (cmd == "cp") {

            string origin, dest;

            cin >> origin >> dest;

            cp (origin, dest);

        }

        // Case 3: mkdir <dir>
        else if (cmd == "mkdir") {

            string dir;
            cin >> dir;

            mkdir (dir);

        }

        // Case 4: rmdir <dir>
        else if (cmd == "rmdir") {

            string dir;
            cin >> dir;

            rmdir(dir);

            cout << "Removido\n";

        }

        // Case 5: cat <dir>
        else if (cmd == "cat") {

            string arq;
            cin >> arq;

            cat(arq);
        }

        // Case 6: touch <arq>
        else if (cmd == "touch") {

            string arq;
            cin >> arq;

            touch(arq);

        }

        // Case 7: rm <arq>
        else if (cmd == "rm") {

            string arq;
            cin >> arq;

            rm(arq);

        }

        // Case 8: find <dir> <arq>
        else if (cmd == "find") {

            string dir, arq; 

            cin >> dir >> arq;
 
            find (dir, arq, root);
 
        } 

        // Case 9: df 
        else if (cmd == "df") {

            df();
        }

        // Case 10: umount
        else if (cmd == "umount") {

            umount();
            
        }

        else if (cmd == "ls") {

            string arg;
            cin >> arg;

            exec_ls(arg, root);

        }

        // TIRAR
        else if (cmd == "show") {

            string arq;
            cin >> arq;

            string path = get_path(arq);
            Diretorio<Arquivo>* dir = ((path == "/") ? (root) : (searchDir (root, path)));
            arq.erase(0, path.length());
            Arquivo* aux = dir->findArq(arq);

            if (aux == nullptr)
                cout << "Arquivo não encontrado!\n";
            else 
                aux->show();

        }

        else if (cmd == "teste_dir") {

            string dir, nome;

            ofstream arquivo;
            
            clock_t ini, fim;

            cout << "<Nome do dir a ser criado>\n";
            cin >> dir;
            cout << "Nome do txt onde salvar os resultados:";
            cin >> nome;

            arquivo.open(nome);

            arquivo << "Teste criando e removendo dir\n\n";

            for (int i = 0; i < 30; i++) {

                arquivo << "Teste " << i << endl << endl;

                arquivo << "- Copia:\n";

                ini = clock(); 

                mkdir(dir);

                fim = clock();                

                arquivo << "    Segundos: " << ((double) (fim-ini) / CLOCKS_PER_SEC) << endl << endl; 

                ini = clock();

                rmdir(dir);

                fim = clock();

                arquivo << "- Remocao:\n";

                arquivo << "    Segundos: " << ((double) (fim-ini) / CLOCKS_PER_SEC) << endl << endl; 
            }

            arquivo.close();



        }

        else if (cmd == "teste_arq") {

            string arq, dest = "/", nome;
            ofstream arquivo;
            
            clock_t ini, fim;

            cout << "<Nome do arquivo>\n";
            cin >> arq;
            cout << "Nome do txt onde salvar os resultados:";
            cin >> nome;

            arquivo.open(nome);

            cout << "Teste criando e removendo arquivo\n\n";

            for (int i = 0; i < 30; i++) {

                arquivo << "Teste " << i << endl << endl;

                arquivo << "- Copia:\n";

                ini = clock();

                cp(arq, dest);

                fim = clock();                

                arquivo << "    Segundos: " << ((double) (fim-ini) / CLOCKS_PER_SEC) << endl << endl; 

                ini = clock();

                rm (dest + arq);

                fim = clock();

                arquivo << "- Remocao:\n";

                arquivo << "    Segundos: " << ((double) (fim-ini) / CLOCKS_PER_SEC) << endl << endl; 
            }

            arquivo.close();

        }

        // if it's not "sai", this command is not supported
        else if (cmd != "sai")
            cout << "Command not supported!\n";

    }



}

void cp (string origin, string dest) {

    string data; 
    int n_blocks;
    time_t c, t;
    int size;

    t = time(NULL);

    // get the information 
    arq_info(origin, c, n_blocks, data, size);

    if (data.empty())
        return;

    if (used_size + base_used_size + FILE_METADATA_SIZE > MAX_SIZE) {

        cout << "Sistema de arquivos cheio! Nao comporta os metadados do arquivo :(" << endl;
        return;
    }

    else if (used_size + base_used_size + FILE_METADATA_SIZE + size > MAX_SIZE) {

        cout << "Arquivo grande demais! Nao coube no sistema de arquivos :(" << endl;
        return;
    }
   
    int first = find_free_block(bitmap);
    int atual;
    int next = first;

    // set the bitmap and fat 
    for (int i = 0; i < n_blocks; i++) {
        atual = next;
        bitmap[atual] = 0;
        used_blocks++;
        next = find_free_block(bitmap);
        FAT[atual] = next;
        
    }
    FAT[atual] = -1;

    string path = get_path(dest);

    Diretorio<Arquivo>* dir = ((path == "/") ? (root) : (searchDir(root, path)));

    path = get_path(origin);
    origin.erase(0,path.length()); 

    try{
        Arquivo novo(origin, data, size, c, t, t, first, dir);
        dir->addArq(novo);
        files++;
        used_size = used_size + FILE_METADATA_SIZE + size;
    }
    catch (int e){
        if (e == 1){
            return;
        }
    }

    // change the access and modification time
    dir->setAccessTime(t);
    dir->setModifTime(t);
 
} 

void mount (string arq) { 
 
    file.open(arq);   
    filename = arq; 

    // vazio
    if (!file.is_open()){ 

        bitmap = vector<int>(N_BLOCKS, 1); 
        
        base_used_size += N_BLOCKS + 5;

        FAT = vector<int>(N_BLOCKS, -1);

        base_used_size += FAT_MAX_SIZE + 6;
        base_wasted_space += 162;
        base_used_blocks = 12;

        for (int i = 0; i < 12; i++)
            bitmap[i] = 0;

        bitmap[12] = 0;
        root = new Diretorio<Arquivo>("/", nullptr); 
        root->block = 12;
        root->occupied_size = DIRECTORY_METADATA_SIZE;

        used_size += DIRECTORY_METADATA_SIZE;
        used_blocks++; //bitmap + fat + root
        directories++; //root
        files = 0;

        return;
 
    } 

    file.close();  
 
    int count = 0; // numero de blocos ocupados

    vector<string> blocos = read(arq); //este cara transforma o textão em um vetor de blocos (strings)

    // for (int i = 0; i < (int) blocos.size(); i++){
    //     used_size = used_size + blocos[i].length() + 1;
    // }

    // bitmap
    bitmap = get_bitmap (blocos, count);
 
    base_used_size += N_BLOCKS + 5;

    FAT = getFAT(blocos);    

    base_used_size += FAT_MAX_SIZE + 6;
    base_wasted_space += 162; 
    base_used_blocks = 12;
  
    root = new Diretorio<Arquivo>("/", nullptr);

    setRoot(root, blocos, FAT);

}

void rm (string arq) {

    string path = get_path(arq);

    // we find the dir
    Diretorio<Arquivo>* dir = ((path == "/") ? (root) : (searchDir (root, path)));

    arq.erase(0, path.length()); // now we need just the name

    Arquivo* target = dir->findArq(arq);

    if (target == nullptr) {
        cout << "Arquivo não encontrado\n";
        return;
    }

    int block = target->get_block();
    int next = FAT[block];

    bitmap[block] = 1; // free the bitmap
    FAT[block] = -1;
    used_blocks--;


    while (next != -1) {
        block = next;
        next = FAT[next];
        bitmap[block] = 1;
        FAT[block] = -1;
        used_blocks--;
    }

    dir->removeArq(arq); //CODIGO BEM FEITO
    files--;
    used_size = used_size - target->getSize() - FILE_METADATA_SIZE;

   dir->occupied_size -= FILE_METADATA_SIZE;

    time_t t = time(NULL);
    dir->setAccessTime(t);
    dir->setModifTime(t);

    cout << "Arquivo removido\n";

}

void cat (string arq) {

    string path = get_path(arq);

    Diretorio<Arquivo>* dir = ((path == "/") ? (root) : (searchDir (root, path)));

    if (dir == nullptr) {
        cout << "Arquivo não encontrado!\n";
        return;
    }

    arq.erase(0, path.length());

    Arquivo* aux = dir->findArq(arq);
    cout << ((aux == nullptr) ? ("Arquivo não encontrado!") : (aux->getData())) << endl;
   
}

void touch (string arq) {

    string path = get_path(arq);

    // find the dir
    Diretorio<Arquivo>* dir = ((path == "/") ? (root) : (searchDir (root, path)));

    time_t t = time(NULL); // current time

    arq.erase(0, path.length()); // get the name

    Arquivo* aux = dir->findArq(arq); 
 
    // arq doesn't exist
    if (aux == nullptr) { 

        if (used_size + base_used_size + FILE_METADATA_SIZE > MAX_SIZE){
            cout << "Sistema de arquivos cheio!" << endl;
            return;
        }

        string path = get_path(arq);
        int block = find_free_block(bitmap); 
        
        try{
            aux = new Arquivo(arq, "", 0, t,t,t,block,dir);
        }
        catch (int e){
            if (e == 1){
                return;
            }
        }
        bitmap[block] = 0;
        FAT[block] = -1;
        used_blocks++;
        files++;
        used_size = used_size + FILE_METADATA_SIZE;

        dir->addArq(*aux);

        dir->setModifTime(t);
        dir->setAccessTime(t);

        dir = nullptr;

    }

    else {

        aux->setAccessTime(t);
        aux->setModifTime(t);

    }
        

}

void ls (string dir){
    
    /*lista os arquivos e diretorios que estejam “embaixo” do diretorio dir.
    Na listagem que sera exibida, para todos os arquivos existentes, deverao ser exibidos: nome, tama-
    nho em bytes e data de ultima modificacao. Diretorios devem ser exibidos com alguma informacao
    a mais que os diferencie como diretorios.*/

    //percorre a lista do diretorio. imprime os nomes e os metadados.

    cout << "TO DO\n";
}

void df() {

    cout << "Numero de arquivos: " << files << endl;
    cout << "Numero de diretorios: " << directories << endl;
    cout << "Espaco desperdicado: " << used_blocks * 4096 - used_size + base_wasted_space << endl;
    cout << "Espaco livre: " << MAX_SIZE - base_used_size - used_size << endl << endl;

}

void umount() {

    vector<string> blocks(N_BLOCKS, "");
    int count = 0;
    
    fill_blocks(blocks, bitmap, FAT, count, root);

    ofstream output;
    output.open(filename);

    // bitmap
    for (int i = 0; i < 6; i++)
        output << blocks[i] << BLOCK_TERMINATOR;
    
    // FAT
    for (int i = 6; i < 12; i++)
        output << blocks[i] << BLOCK_TERMINATOR;

    for (int i = 12; i < (int) blocks.size(); i++) // :) 
        if (!bitmap[i] || blocks[i].length() > 0)
            output << " " << blocks[i] << " " << BLOCK_TERMINATOR;


    output.close();

    // we clean the bitmap, fat and root
    bitmap.clear();
    FAT.clear();
    free(root);
    root = nullptr;

    used_size = 0;
    base_used_blocks = 0;
    base_used_size = 0;
    base_wasted_space = 0;
    directories = 0;
    used_blocks = 0;
    files = 0;

}

void mkdirRec(string dir, Diretorio<Arquivo>* root){

    if (dir.length() == 0){
        return;
    }

    string ndir = tokenize(dir, "/");
    
    Diretorio<Arquivo>* next_directory = root->findDir(ndir); //procura o diretorio ndir em root
    
    if (next_directory == NULL){ 
    
        try{
    
            if (used_size + base_used_size + DIRECTORY_METADATA_SIZE > MAX_SIZE){
                cout << "Filesystem full. Adding another directory would put it over 100MB." << endl;
                return;
            }
    
            Diretorio<Arquivo> novo(ndir, root); //cria o novo diretorio em root
    
            int free_block = find_free_block(bitmap);
            bitmap[free_block] = 0;
            FAT[free_block] = -1;
            
            Diretorio<Arquivo>* fresh = root->findDir(ndir);
            fresh->block = free_block;
        }
        catch(int e){
            if (e == 1){
                return;
            }
        }
        next_directory = root->findDir(ndir); //&novo
        directories++;
        used_blocks++;
        used_size = used_size + DIRECTORY_METADATA_SIZE;
    }
    mkdirRec(dir, next_directory);

}

void mkdir(string dir) { //recursion wrapper

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

    mkdirRec(dir, root);
}

iter(Diretorio<Arquivo>) rmdirRec(string dir, Diretorio<Arquivo>* root) {
    
    string ndir = tokenize(dir, "/");
    list<Arquivo>::iterator it;
    list<Diretorio<Arquivo>>::iterator it2;

    Diretorio<Arquivo>* next_directory = root->findDir(ndir); //procura o diretorio ndir em root
    if (next_directory == NULL){
        cout << "invalid path to target. " << ndir << " was not found." << endl;
        return it2;
    }
    //next_directory != NULL
    if (dir.length() == 0){ //chegamos no alvo, next_directory é quem queremos remover
        if (!next_directory->dirEmpty()){ //not empty

            // arquivos
            for (it = next_directory->arqs.begin(); it != next_directory->arqs.end(); ++it){ //remove todos os arquivos em next_directory
                Arquivo* file = next_directory->findArq((*it).getName());
                int block = file->get_block();
                int next = FAT[block];
                bitmap[block] = 1;
                FAT[block] = -1;
                used_blocks--;
                while (next != -1) {
                    block = next;
                    next = FAT[next];
                    bitmap[block] = 1;
                    FAT[block] = -1; 
                    used_blocks--;
                }
                it = next_directory->arqs.erase(it);
                used_size = used_size - FILE_METADATA_SIZE - file->getSize();
                next_directory->occupied_size -= FILE_METADATA_SIZE;
                files--;
            }

            // diretorios
            for (it2 = next_directory->dirs.begin(); it2 != next_directory->dirs.end(); ++it2){
                string to_be_removed = (*it2).getName();
                it2 = rmdirRec(to_be_removed, next_directory); //chama recursivo para os diretórios filhos de next_directory
            } 
        }
        //now it's empty
        for (it2 = root->dirs.begin(); it2 != root->dirs.end(); ++it2){
            if ((*it2).getName() == ndir){
                bitmap[next_directory->block] = 1;
                FAT[next_directory->block] = -1;
                directories--;
                used_blocks--;
                used_size = used_size - DIRECTORY_METADATA_SIZE;
                root->occupied_size -= DIRECTORY_METADATA_SIZE;

                time_t atual = time(NULL);
                root->setAccessTime(atual);
                root->setModifTime(atual);

                it2 = root->dirs.erase(it2); //remove do raiz
                return it2;
            }
        }
    } 
    else{ //dir.length() != 0, have not reached target
        rmdirRec(dir, next_directory);
    }
    return it2;
}

void rmdir(string dir) {
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
    rmdirRec(dir, root);
}