
#include <cstdlib>
#include <iostream>          
#include <wchar.h>
#include <fstream>
#include <string>
#include <locale>
#include <ctime>
#include <iomanip>
#include <unistr.h>
#include <codecvt>
#include <cctype>
#include <math.h>
#include <chrono> 
//#include <ustream.h>
//#include <locid.h>

#include <iostream>

#include "SymbolTableLD.h"
#include "SymbolTableLO.h"
#include "SymbolTableVD.h"
#include "SymbolTableVO.h"
#include "SymbolTableTR2.h"
#include "SymbolTableAB.h"
#include "SymbolTableRN.h"
#include "SymbolTableA23.h"
#include "SymbolTableHA2.h"

using namespace std;



long isPrime(long num){

	if (num < 2){
		return 0;
	}

	long divisor = 2;
	while  (divisor <= (long)sqrt((double)num)){
		if (num % divisor == 0){
			return 0;
		}
		divisor++;
	}
	return 1;

}


long nearestPrime(long num){

	if (isPrime(num) == 1){
		return num;
	}
	while (isPrime(num) != 1){
		num++;
	}
	return num;

}

wstring FormatString(wstring received, locale loc){

	wstring formatted;
	wstring c;

	for (unsigned int i = 0; i < received.length(); i++){
		c = tolower(received[i], loc);
		formatted.append(c);
	}

	if (formatted[formatted.length() - 1] == '.' || formatted[formatted.length() - 1] == ',' || formatted[formatted.length() - 1] == ':' ||
		formatted[formatted.length() - 1] == ';' || formatted[formatted.length() - 1] == '?' || formatted[formatted.length() - 1] == '!'){
		formatted.pop_back();
	}
	return formatted;
}

wstring FormatwString(wstring received, locale loc){

	wstring formatted;
	wstring c;

	for (unsigned int i = 0; i < received.length(); i++){
		c = tolower(received[i], loc);
		formatted.append(c);
	}

	if (formatted[formatted.length() - 1] == '.' || formatted[formatted.length() - 1] == ',' || formatted[formatted.length() - 1] == ':' ||
		formatted[formatted.length() - 1] == ';' || formatted[formatted.length() - 1] == '?' || formatted[formatted.length() - 1] == '!'){
		formatted.pop_back();
	}
	return formatted;
}



int main(int argc, char* argv[])
{	

	long sem;
	long sem_defined = 0;
	long DEBUG = 0;
	long experiments = 0;
	long standardmode = 1;
	string arg3;
	string arg4;
	locale LocPadrao("");
	setlocale(LC_CTYPE, "");
	wcout << setprecision(10) << fixed;
	if (argc < 3){
		wcout << L"Leia o README para instruções de uso." << endl;
		return 0;
	}
	string arg2 = argv[2];
	if (argc >= 4){
		arg3 = string(argv[3]);
		if (arg3 == "D"){
			//modo interativo puro (não lê o texto)
			DEBUG = 1;
			standardmode = 0;
		}
		if (arg3 == "E"){
			//run experiments
			standardmode = 0;
			experiments = 1;

		}
		if (arg3 == "S"){
			//dummy
		}
		if ((arg2 == "TR" || arg2 == "HA") && argc == 5){
			arg4 = string(argv[4]);
			sem = stol(arg4);
			wcout << sem << endl;
			sem_defined = 1;
			wcout << "Semente: " << sem << endl;
		}
		else if (argc == 5){
			wcout << L"Leia o README para instruções de uso." << endl;
			return 0;
		}
	}
	if (argc > 5){
		wcout << L"Leia o README para instruções de uso." << endl;
		return 0;
	}

	//clock_t timer;
	//timer = clock();

	wstring word;

	SymbolTable* tabela;

	wfstream textfile;

	string nome_arquivo(argv[1]);

	if (standardmode == 1){
		wcout << L"Iniciando o programa em modo padrão." << endl;
		textfile.imbue(LocPadrao);
		textfile.open(nome_arquivo);

		long conta = 0;
		long size = 0;

		if (textfile.is_open()){
			while (textfile >> word){
				if (iswdigit(word[0]) != 0){
					continue;
				} 
				word = FormatString(word, LocPadrao);
				conta++;
			}
		}
		textfile.close();
		textfile.imbue(LocPadrao);
		textfile.open(nome_arquivo);

		if (textfile.is_open()){
			while (textfile >> word){
				if (iswdigit(word[0]) != 0){
					continue;
				} 
				word = FormatString(word, LocPadrao);
				conta++;
			}
		}
		textfile.close();
		size = conta + 100;
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		seed = int((seed * 47) % 10007);
		string input(argv[2]);

		if (input == "VD"){
				tabela = new SymbolTableVD(size);
			}
			else if (input == "VO"){
				tabela = new SymbolTableVO(size);
			}
			else if (input == "LD"){
				tabela = new SymbolTableLD;
			}
			else if (input == "LO"){
				tabela = new SymbolTableLO;
			}
			else if (input == "AB"){
				tabela = new SymbolTableAB;
			}
			else if (input == "TR"){
				if (sem_defined == 1){
					wcout << L"Semente para o Treap: " << sem << L"." << endl;
					tabela = new SymbolTableTR(to_string(sem));
				}
				else{
					wcout << L"Semente para o Treap: " << seed << L"." << endl;
					tabela = new SymbolTableTR(to_string(seed));
				}
			}
			else if (input == "RN"){
				tabela = new SymbolTableRN;
			}
			else if (input == "A23"){
				tabela = new SymbolTableA23;
			}
			else if (input == "HA"){
				long hash_size;
				if (sem_defined == 1){
					hash_size = sem;
					wcout << L"Tamanho fornecido para tabela de hashing: " << hash_size << L"." << endl;
				}
				else{
					if (size == 0){
						wcout << L"Tamanho de hash não fornecido e texto não fornecido. Erro. Leia o README para instruções de uso." << endl;
						return 0;
					}
					hash_size = nearestPrime(size/5);
					wcout << L"Tamanho da tabela de hashing: " << hash_size << L"." << endl;
				}
				tabela = new SymbolTableHA2(hash_size);
			}
			else{
				wcout << L"Input não reconhecido." << endl;
			}

			wcout << L"Inserindo texto na tabela de símbolos." << endl;

			textfile.imbue(LocPadrao);
			textfile.open(nome_arquivo);

			if (textfile.is_open()){
				while (textfile >> word){
					if (iswdigit(word[0]) != 0){
						continue;
					} 
					word = FormatString(word, LocPadrao);
					tabela->insere(word, 1);
				}
			}
			textfile.close();

			wcout << L"Inserção terminada." << endl;
			DEBUG = 1;
			wcout << L"Modo Interativo:" << endl;
		while (DEBUG == 1){
			wstring instruction;
			wcout << L"Digite a prox. instrução: I - insere, R - remove, D - devolve, K - rank, S - seleção, P - imprimir, T - tamanho, E - terminar" << endl;
			wcin >> instruction;
			if (instruction == L"I"){
				wcout << L"Digite a próxima palavra a ser inserida na tabela, com valor, por exemplo: banana 3" << endl;
				wcin >> word;
				long num;
				wcin >> num;
				word = FormatString(word, LocPadrao);
				tabela->insere(word, num);
			}
			else if (instruction == L"R"){
				wcout << L"Digite a palavra a ser removida:" << endl;
				wcin >> word;
				word = FormatString(word, LocPadrao);
				tabela->remove(word);
			}
			else if (instruction == L"D"){
				wcout << L"Digite a palavra a ser consultada:" << endl;
				wcin >> word;
				word = FormatString(word, LocPadrao);
				long val = tabela->devolve(word);
				wcout << L"Palavra " << word << L" tem valor " << val << endl;
	
			}
			else if (instruction == L"K"){
				wcout << L"Digite a palavra a ser rankeada:" << endl;
				wcin >> word;
				word = FormatString(word, LocPadrao);
				long rank_num = tabela->rank(word);
				wcout << L"Palavra " << word << L" tem rank " << rank_num << L"." << endl;
			}
			else if (instruction == L"S"){
				wcout << L"Digite o rank procurado:" << endl;
				long rank;
				wcin >> rank;
				word = tabela->select(rank);
				wcout << L"Rank " << rank << L" corresponde à palavra " << word << L"." << endl;
			}
			else if (instruction == L"E"){
				DEBUG = 0;
				delete tabela;
				return 1;
			}
			else if (instruction == L"P"){
				wcout << L"Imprimindo tabela:" <<endl;
				tabela->print();
			}
			else if (instruction == L"P"){
				wcout << L"tamanho da tabela:" << endl;
				tabela->print();
			}

			wcin.ignore(numeric_limits<streamsize>::max(),'\n');
		}
	
	} //END OF STANDARD MODE	


	if (DEBUG == 1){
		textfile.imbue(LocPadrao);
		textfile.open(nome_arquivo);
		long conta = 0;

		if (textfile.is_open()){
			while (textfile >> word){
				if (iswdigit(word[0]) != 0){
					continue;
				} 
				word = FormatString(word, LocPadrao);
				conta++;
			}
		}
		textfile.close();
		wcout << L"Palavras no texto: " << conta << endl;
	
			wcout << L"Primeiras 100 palavras: " << endl;
	
			textfile.imbue(LocPadrao);
			textfile.open(nome_arquivo);
			conta = 0;
	
			if (textfile.is_open()){
				while (conta < 100){
					textfile >> word;
					if (iswdigit(word[0]) != 0){
						continue;
					} 
					word = FormatString(word, LocPadrao);
					wcout << word << L" ";
					conta++;
				}
			}
			textfile.close();
			
			wcout << endl;
			long size = 0;
	
			size = conta + 100;
			
			long seed = (long)time(NULL);
			seed = int((seed * 47) % 10007);
			wcout << L"Semente gerada: " << seed << endl;
			string input(argv[2]);
	
			if (input == "VD"){
				wcout << L"Inicializando tabela de símbolos com vetor desordenado." << endl;
				tabela = new SymbolTableVD(size);
			}
			else if (input == "VO"){
				wcout << L"Inicializando tabela de símbolos com vetor ordenado." << endl;
				tabela = new SymbolTableVO(size);
			}
			else if (input == "LD"){
				wcout << L"Inicializando tabela de símbolos com lista desordenada." << endl;
				tabela = new SymbolTableLD;
			}
			else if (input == "LO"){
				wcout << L"Inicializando tabela de símbolos com lista ordenada." << endl;
				tabela = new SymbolTableLO;
			}
			else if (input == "AB"){
				wcout << L"Inicializando tabela de símbolos com arvore na ordem de entrada." << endl;
				tabela = new SymbolTableAB;
			}
			else if (input == "TR"){
				wcout << L"Inicializando tabela de símbolos com treap." << endl;
				if (sem_defined == 1){
					wcout << L"Semente para o Treap: " << sem << L"." << endl;
					tabela = new SymbolTableTR(arg4);
				}
				else{
					wcout << L"Semente para o Treap: " << seed << L"." << endl;
					string semente = to_string(seed);
					tabela = new SymbolTableTR(semente);
				}
			}
			else if (input == "RN"){
				wcout << L"Inicializando tabela de símbolos com árvore rubro-negra." << endl;
				tabela = new SymbolTableRN;
			}
			else if (input == "A23"){
				wcout << L"Inicializando tabela de símbolos com árvore 2-3." << endl;
				tabela = new SymbolTableA23;
			}
			else if (input == "HA"){
				wcout << L"Inicializando tabela de símbolos com tabela de hashing." << endl;
				long hash_size;
				if (sem_defined == 1){
					hash_size = sem;
					wcout << L"Tamanho fornecido para tabela de hashing: " << hash_size << L"." << endl;
				}
				else{
					if (size == 0){
						wcout << L"Tamanho de hash não fornecido e texto não fornecido. Erro. Leia o README para instruções de uso." << endl;
						return 0;
					}
					hash_size = nearestPrime(size/5);
					wcout << L"Tamanho da tabela de hashing: " << hash_size << "." << endl;
				}
				tabela = new SymbolTableHA2(hash_size);
			}
			else{
				wcout << L"Input não reconhecido." << endl;
			}
			wstring instruction;
		while (DEBUG == 1){
			wcout << L"Modo Interativo:" << endl;
			wcout << L"Digite a prox. instrução: I - insere, R - remove, D - devolve, K - rank, S - seleção, P - imprimir, E - terminar" << endl;
			wcin >> instruction;
			if (instruction == L"I"){
				wcout << L"Digite a próxima palavra a ser inserida na tabela, com valor, por exemplo: banana 3" << endl;
				wcin >> word;
				long num;
				wcin >> num;
				word = FormatString(word, LocPadrao);
				tabela->insere(word, num);
			}
			else if (instruction == L"R"){
				wcout << L"Digite a palavra a ser removida:" << endl;
				wcin >> word;
				word = FormatString(word, LocPadrao);
				tabela->remove(word);
			}
			else if (instruction == L"D"){
				wcout << L"Digite a palavra a ser consultada:" << endl;
				wcin >> word;
				word = FormatString(word, LocPadrao);
				long val = tabela->devolve(word);
				wcout << L"Palavra " << word << L" tem valor " << val << endl;
			}
			else if (instruction == L"K"){
				wcout << L"Digite a palavra a ser rankeada:" << endl;
				wcin >> word;
				word = FormatString(word, LocPadrao);
				long rank_num = tabela->rank(word);
				wcout << L"Palavra " << word << L" tem rank " << rank_num << L"." << endl;
			}
			else if (instruction == L"S"){
				wcout << L"Digite o rank procurado:" << endl;
				long rank;
				wcin >> rank;
				word = tabela->select(rank);
				wcout << L"Rank " << rank << L" corresponde à palavra " << word << L"." << endl;
			}
			else if (instruction == L"E"){
				DEBUG = 0;
				delete tabela;
				return 1;
			}
			else if (instruction == L"P"){
				wcout << L"Imprimindo tabela:" <<endl;
				tabela->print();
			}
			wcin.ignore(numeric_limits<streamsize>::max(),'\n');
		}

	} // END OF DEBUG MODE

	long factor = 64;
	wcout << L"Iniciando o programa em modo de experimentos." << endl;

	while (experiments == 1){
		
		SymbolTable* tabela;

		wcout << endl;
		//long hash_size = 0;
		wcout << L"Experimento com fator de escala: "<< factor << endl;
		textfile.imbue(LocPadrao);
		textfile.open(nome_arquivo);

		long conta = 0;
		long size = 0;
		vector<wstring> test_cases;
		vector<long> test_ranks;

		if (textfile.is_open()){
			while (textfile >> word){
				if (iswdigit(word[0]) != 0){
					continue;
				} 
				word = FormatString(word, LocPadrao);
				conta++;
			}
		}
		textfile.close();
		size = conta/factor;
		long seed = (long)time(NULL);
		seed = int((seed * 47) % 10007);
		string input(argv[2]);

		if (input == "VD"){
			wcout << L"Inicializando tabela de símbolos com vetor desordenado." << endl;
				tabela = new SymbolTableVD(size);
			}
			else if (input == "VO"){
				wcout << L"Inicializando tabela de símbolos com vetor ordenado." << endl;
				tabela = new SymbolTableVO(size);
			}
			else if (input == "LD"){
				wcout << L"Inicializando tabela de símbolos com lista desordenada." << endl;
				tabela = new SymbolTableLD;
			}
			else if (input == "LO"){
				wcout << L"Inicializando tabela de símbolos com lista ordenada." << endl;
				tabela = new SymbolTableLO;
			}
			else if (input == "AB"){
				wcout << L"Inicializando tabela de símbolos com árvore binaria desordenada." << endl;
				tabela = new SymbolTableAB;
			}
			else if (input == "TR"){
				wcout << L"Inicializando tabela de símbolos com treap." << endl;
				if (sem_defined == 1){
					wcout << L"Semente para o Treap: " << sem << "." << endl;
					tabela = new SymbolTableTR(arg4);
				}
				else{
					wcout << L"Semente para o Treap: " << seed << "." << endl;
					string semente = to_string(seed);
					tabela = new SymbolTableTR(semente);
				}
			}
			else if (input == "RN"){
				wcout << L"Inicializando tabela de símbolos com árvore rubro-negra." << endl;
				tabela = new SymbolTableRN;
			}
			else if (input == "A23"){
				wcout << L"Inicializando tabela de símbolos com árvore 2-3" << endl;
				tabela = new SymbolTableA23;
			}
			else if (input == "HA"){
				wcout << L"Inicializando tabela de símbolos com tabela de hashing." << endl;
				long hash_size;
				if (sem_defined == 1){
					hash_size = sem;
					wcout << L"Tamanho fornecido para tabela de hashing: " << hash_size << "." << endl;
				}
				else{
					if (size == 0){
						wcout << L"Tamanho de hash não fornecido e texto não fornecido. Erro. Leia o README para instruções de uso." << endl;
						return 0;
					}
					hash_size = nearestPrime(size/4);
					wcout << L"Tamanho da tabela de hashing: " << hash_size << "." << endl;
				}
				tabela = new SymbolTableHA2(hash_size);
			}
			else{
				wcout << L"Input não reconhecido." << endl;
			}

		wcout << L"Inserindo texto na tabela de símbolos." << endl;
		conta = 0;
		//long present = 0;
		textfile.imbue(LocPadrao);
		textfile.open(nome_arquivo);
		if (textfile.is_open()){
			while (textfile >> word && conta < (size)){
				if (iswdigit(word[0]) != 0){
					continue;
				} 
				word = FormatString(word, LocPadrao);
				tabela->insere(word, 1);
				//tabela->print();
				if (conta % (size/10) == 0){ //uma palavra a cada 100
					/*for (unsigned int i = 0; i < test_cases.size(); i++){
						if (test_cases[i] == word){
							present = 1;
							break;
						}
					}*/
					//if (present == 0){
					test_cases.push_back(word);
					//}
				}
				conta++;
				//present = 0;
			}
		}
		//medição de tempo de https://www.geeksforgeeks.org/measure-execution-time-with-high-precision-in-c-c/
		//ios_base::sync_with_stdio(false);
		textfile.close();
		wcout << L"Inserção terminada." << endl;

		auto start = chrono::high_resolution_clock::now();
		wcout << L"Testes de inserção em tabela de " << tabela->getSize() << L" elementos: " << endl;
		for (unsigned int i = 0; i < test_cases.size(); i++){
			tabela->insere(test_cases[i], 1);
		}
		auto end = chrono::high_resolution_clock::now();
		double timer = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		timer *= 1e-9;
		wcout << L"Testes concluidos em " << timer << L" segundos." << endl;
		wcout << L"Tempo médio de inserção foi: " << timer/(test_cases.size()) << L" em tabela de " << tabela->getSize() << " elementos distintos. " << endl;
		wcout << L"Elementos distintos na tabela: " << tabela->getSize() << endl;

		wcout << endl;

		start = chrono::high_resolution_clock::now();
		wcout << L"Testes de devolução(busca) em tabela de tamanho " << tabela->getSize() << L": " << endl;
		for (unsigned int i = 0; i < test_cases.size(); i++){
			tabela->devolve(test_cases[i]);
		}
		end = chrono::high_resolution_clock::now();
		timer = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		timer *= 1e-9;
		wcout << L"Testes concluidos em " << timer << L" segundos." << endl;
		wcout << L"Tempo médio de devolução foi: " << timer/(test_cases.size()) << L" em tabela de " << tabela->getSize() << " elementos distintos. " << endl;
		
		wcout << endl;

		start = chrono::high_resolution_clock::now();
		wcout << L"Testes de rankeamento em tabela de tamanho " << tabela->getSize() << L": " << endl;
		for (unsigned int i = 0; i < test_cases.size(); i++){
			test_ranks.push_back(tabela->rank(test_cases[i]));
		}
		end = chrono::high_resolution_clock::now();
		timer = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		timer *= 1e-9;
		wcout << L"Testes concluidos em " << timer << L" segundos." << endl;
		wcout << L"Tempo médio de rankeamento foi: " << timer/(test_cases.size()) << L" em tabela de " << tabela->getSize() << " elementos distintos. " << endl;
		
		wcout << endl;

		start = chrono::high_resolution_clock::now();
		wcout << L"Testes de seleção em tabela de tamanho " << tabela->getSize() << L": " << endl;
		for (unsigned int i = 0; i < test_ranks.size(); i++){
			tabela->select(test_ranks[i]);
		}
		end = chrono::high_resolution_clock::now();
		timer = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		timer *= 1e-9;
		wcout << L"Testes concluidos em " << timer << L" segundos." << endl;
		wcout << L"Tempo médio de seleção foi: " << timer/(test_cases.size()) << L" em tabela de " << tabela->getSize() << " elementos distintos. " << endl;
		
		wcout << endl;

		start = chrono::high_resolution_clock::now();
		long size_before = tabela->getSize();
		wcout << L"Testes de remoção em tabela de tamanho " << size_before << L": " << endl;
		for (unsigned int i = 0; i < test_cases.size(); i++){
			tabela->remove(test_cases[i]);
		}
		end = chrono::high_resolution_clock::now();
		timer = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		timer *= 1e-9;
		wcout << L"Testes concluidos em " << timer << L" segundos." << endl;
		wcout << L"Tempo médio de remoção foi: " << timer/(test_cases.size()) << L" em tabela de " << size_before << " elementos distintos. " << endl;

		//tabela->print();
		wcout << endl;
		delete tabela;
		if (factor <= 1){
			experiments = 0;
		}
		factor = factor/2;

	}



}