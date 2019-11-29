#include <iostream> 
#include <fstream> //entrada de arquivos
#include <vector> 
#include <cmath> //funçao pow()
#include <windows.h> //funçao Sleep(), system(), MessageBoxA() e GetOpenFileName()
#include <sstream> //objeto ostrigstream


using namespace std;

vector<int> rn_encoded;
vector<int> rn_decoded;
vector<int> decimal;
char nome_arquivo[MAX_PATH];
string ASCII;

void principal();
template<typename T> //para aceitar qualquer tipo de dado que utilize o operador <<
void alertaResultado(T item){
  ostringstream os;
  os << item;
  MessageBoxA(NULL, os.str().c_str(),  "Resultado", MB_OK | MB_ICONINFORMATION);
}
template<typename T>
void alertaErro(T item){
	ostringstream os;
	os << item;
	int msgid = MessageBoxA(NULL, os.str().c_str(), "Erro", MB_ICONERROR | MB_RETRYCANCEL | MB_DEFBUTTON1);
	switch(msgid) {
		case IDCANCEL:
			system("exit");
			break;
		case IDRETRY:
			principal();
			break;
	}
}

void readme(){
	ifstream rm("readme.txt",ifstream::in);
	string line;
	while(getline(rm,line)){
		cout<<line<<endl;	
	}
}

void alertaMenu(){
	ostringstream os;
	os << "Pressione SIM para selecionar um arquivo RN  \nPressione NÃO para ler o readme.txt \nPressione CANCELAR para sair";
	int msgid = MessageBoxA(NULL, os.str().c_str(), "RN Decoder V2.0",  MB_YESNOCANCEL | MB_DEFBUTTON1);
	switch(msgid) {
		case IDCANCEL:
			system("exit");
			break;
		case IDYES:
			principal();
			break;
		case IDNO:
			system("cls");
			readme();
			cout<<endl<<endl<<endl;
			system("pause");
			alertaMenu();
			break;
	}
}
int escolher_arquivo(){
	OPENFILENAME ofn;
    char nomedoarquivo[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = nomedoarquivo;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";

	int a = GetOpenFileName(&ofn);
	for(int i=0;i<MAX_PATH;i++){
		nome_arquivo[i]=nomedoarquivo[i];
	}
   
	return a;
}
void rnDecoder(int n){
	vector<int> aux; 
	int cont=0;
	for(unsigned int i=0;i<rn_encoded.size();i++){
		aux.push_back(rn_encoded[i]);
		cont++;
		if(cont==n){
			int cont0=0,cont1=0;
			for(unsigned int u=0;u<aux.size();u++){
				if(aux[u]==1){
					cont1++;
				}
				else{
					cont0++;
				}
			}
			if(cont1>cont0){
				rn_decoded.push_back(1);
			}
			else{
				rn_decoded.push_back(0);
			}
			aux.clear();
			cont=0;
		}
		
	}
}
void toDecimal(){
	int cont = 0;
	int aux[8];
	for(unsigned int i=0;i<rn_decoded.size();i++){
		aux[cont]=rn_decoded[i];
		cont++;
		if(cont==8){
			int d = 0;
			for(int u=7;u>=0;u--){
				d += pow(2,u)*aux[7-u];
			}
			decimal.push_back(d);
			cont=0;
		}
	}
}
void toString(){
	char aux;
	for(unsigned int i=0;i<decimal.size();i++){
		aux=decimal[i];
		ASCII+=aux;
	}
}
void ascii_art(){
	ifstream art("bin/art.txt",ifstream::in);
	string line;
	while(getline(art,line)){
		cout<<line<<endl;	
	}
}
void menu_table(){
	ifstream menu("bin/menu.txt",ifstream::in);
	string line;
	while(getline(menu,line)){
		cout<<line<<endl;	
	}
}
void diretorio(){
	ifstream dir("bin/diretorio.txt",ifstream::in);
	string line;
	vector<string> v;
	int cont=0;
	while(getline(dir,line)){
		v.push_back(line);
		cont++;
	}
	for(int i=7;i<cont-2;i++){
		cout<<v[i]<<endl;
	}
}
void principal(){
	memset(nome_arquivo, '\0', sizeof(nome_arquivo));
	rn_encoded.clear();
	rn_decoded.clear();
	decimal.clear();
	ASCII.clear();
	
	system("cls");
	ascii_art();
	
	if(escolher_arquivo() == 0){
		alertaErro("Selecione um arquivo .txt");
		return;
	}	
	ifstream file (nome_arquivo, ifstream::in);
	int linha=0;
	int n,x;
	cout<<"--------------------------------------------------------------------------"<<endl;
	cout<<"Lendo o arquivo";
	for(int i=0;i<56;i++){
		Sleep(50);
		cout<<".";
	}
	while(file>>x){
		if(linha==0){
			n=x;
		}
		else{
			rn_encoded.push_back(x);
		}
		linha++;
	}file.close();
	if(linha==0){
		cout<<" ERRO"<<endl;
		alertaErro("Arquivo inexistente, vazio \nou fora do padrao RN");
		return;
	}else
		cout<<" OK"<<endl;

	cout<<"Aplicando filtros RN anti-ruidos";
	for(int i=0;i<39;i++){
		Sleep(75);
		cout<<".";
	}
	cout<<" OK"<<endl;	
	rnDecoder(n);
	
	cout<<"Transformando de binario para decimal";
	for(int i=0;i<34;i++){
		Sleep(50);
		cout<<".";
	}
	cout<<" OK"<<endl;
	toDecimal();
	cout<<"Transformando de decimal para a tabela ASCII";
	for(int i=0;i<27;i++){
		Sleep(50);
		cout<<".";
	}
	cout<<" OK"<<endl;
	toString();
	cout<<"--------------------------------------------------------------------------"<<endl;
	cout<<endl;
	alertaResultado(ASCII);
	alertaMenu();
}
int main(){
	ascii_art();
	alertaMenu();
	return 0;
}
