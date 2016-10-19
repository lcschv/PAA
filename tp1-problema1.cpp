#include <fstream>
#include <limits>
#include <iostream>
#include <string>
#include <istream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <queue>
using namespace std;

struct NODE{
	string id;
	int vertice;	
};
struct Source_Destination_NODE{
	string id;
	int vertice;
	int used;
	int goal;
	string orig;
};
// Definindo pair que sera posicao no grafo
typedef std::pair<int,int> POS;
// Definindo um tipo que sera salvo no grafo
// Definindo tipo de listas para salvar Sources e Destination
typedef map<int,Source_Destination_NODE> TypesListMAP;
// Definindo um tipo para GraphMap
typedef std::map<POS,NODE> GraphMap;
typedef std::map<int,map<int,int>> AdjList;

struct visitedNODE{
	int time;
	int prec;
};

struct sizeOfMatrix{
	int lines;
	int colums;
};


/*Função que realiza leitura do ambiente e modela o grafo, armazena também onde
localiza todos os sources e destinos*/
sizeOfMatrix readAmbientFile(string ambientefile, GraphMap &graphMap, TypesListMAP &sourcemap, TypesListMAP &destinationmap, queue<int> &sourceFifo, map<int,POS> &MapvertexPair){
	ifstream inputfile;
	inputfile.open(ambientefile);
	string line = "";
	int vertexnumber = 0;
	sizeOfMatrix tempSize;
	tempSize.lines= 0;
	tempSize.colums= 0;

	int columnNum = 1;
	int  lineNum = 1;
	while (getline(inputfile,line)){
		istringstream iss(line);
		while (iss){
		    std::string sub;
		    iss >> sub;
		    if (sub.length()){
		        // cout <<"vertex: "<< vertexnumber<<"Posicao("<<lineNum<<","<<columnNum<<") --->" <<sub<<endl;
				tempSize.colums = columnNum;
				if (sub[0] == 'x'){
					sourcemap[vertexnumber].id = sub;
					sourcemap[vertexnumber].vertice = vertexnumber;
					sourcemap[vertexnumber].goal = -1;
					sourcemap[vertexnumber].orig = sub;

					sourceFifo.push(vertexnumber);

				} else if(sub[0] == 'g'){
					destinationmap[vertexnumber].id = sub;
					destinationmap[vertexnumber].vertice = vertexnumber;

				}
				graphMap[POS(lineNum,columnNum)].id = sub;
				graphMap[POS(lineNum,columnNum)].vertice = vertexnumber;
				MapvertexPair[vertexnumber] = POS(lineNum,columnNum);

				// cout <<"vertex: "<< vertexnumber<<"Posicao("<<lineNum<<","<<columnNum<<") --->" <<sub<<endl;

				columnNum++;
		    	vertexnumber++;
		    }
		}
		tempSize.lines = lineNum;
		lineNum++;
		columnNum = 1;
		
    };
    return tempSize;
}

/*Função que calcula a lista de adjacência para cada um dos vértices do grafo
obs.: não calcula para posições que são Obstáculos */
void getAdjList(sizeOfMatrix numOfVertex, GraphMap &graphMap, AdjList &ADJList){
	int VertexIndex = 0;
	string a = "+";
	string b = "o";
	string c = "g";
	for (int i=1; i <= numOfVertex.lines; i++){
		for (int j = 1; j <= numOfVertex.colums; j++){
			if (graphMap[POS(i,j)].id != b){	
				/*Verifica para cada um das 8 possibilidades de vizinhança*/
				if ((graphMap[POS(i+1,j)].id == a) || (graphMap[POS(i+1,j)].id[0] == 'g')){
					ADJList[VertexIndex][graphMap[POS(i+1,j)].vertice]=1;
				}else if ((graphMap[POS(i+1,j)].id[0] == 'x')){
					ADJList[VertexIndex][graphMap[POS(i+1,j)].vertice]=0;
				}
				if ((graphMap[POS(i-1,j)].id == a) || (graphMap[POS(i-1,j)].id[0] == 'g')){
					ADJList[VertexIndex][graphMap[POS(i-1,j)].vertice] =1;
				}else if (graphMap[POS(i-1,j)].id[0] == 'x'){
					ADJList[VertexIndex][graphMap[POS(i-1,j)].vertice] =0;
				}
				if ((graphMap[POS(i,j+1)].id == a) || (graphMap[POS(i,j+1)].id[0] == 'g')){
					ADJList[VertexIndex][graphMap[POS(i,j+1)].vertice] =1;				
				}else if (graphMap[POS(i,j+1)].id[0] == 'x') {
					ADJList[VertexIndex][graphMap[POS(i,j+1)].vertice] =0;
				}
				if ((graphMap[POS(i,j-1)].id == a) || (graphMap[POS(i,j-1)].id[0] == 'g')){
					ADJList[VertexIndex][graphMap[POS(i,j-1)].vertice] =1;				
				}else if (graphMap[POS(i,j-1)].id[0] == 'x'){
					ADJList[VertexIndex][graphMap[POS(i,j-1)].vertice] =0;
				}
				if ((graphMap[POS(i+1,j+1)].id == a) || (graphMap[POS(i+1,j+1)].id[0] == 'g')){
					ADJList[VertexIndex][graphMap[POS(i+1,j+1)].vertice] =1 ;				
				}else if (graphMap[POS(i+1,j+1)].id[0] == 'x'){
					ADJList[VertexIndex][graphMap[POS(i+1,j+1)].vertice] =0;
				}
				if ((graphMap[POS(i+1,j-1)].id == a) || (graphMap[POS(i+1,j-1)].id[0] == 'g')){
					ADJList[VertexIndex][graphMap[POS(i+1,j-1)].vertice] =1;								
				}else if (graphMap[POS(i+1,j-1)].id[0] == 'x'){
					ADJList[VertexIndex][graphMap[POS(i+1,j-1)].vertice] =0;
				}
				if ((graphMap[POS(i-1,j-1)].id == a) || (graphMap[POS(i-1,j-1)].id[0] == 'g')){
					ADJList[VertexIndex][graphMap[POS(i-1,j-1)].vertice] =1;				
				}else if (graphMap[POS(i-1,j-1)].id[0] == 'x'){
					ADJList[VertexIndex][graphMap[POS(i-1,j-1)].vertice] =0;
				}
				if ((graphMap[POS(i-1,j+1)].id == a) || (graphMap[POS(i-1,j+1)].id[0] == 'g')){
					ADJList[VertexIndex][graphMap[POS(i-1,j+1)].vertice] =1;
				}else if (graphMap[POS(i-1,j+1)].id[0] == 'x'){
					ADJList[VertexIndex][graphMap[POS(i-1,j+1)].vertice] =0;
				}
			}
			VertexIndex++;
		}
	}
}

/*Função que imprime a lista de adjacência (caso necessário chamar essa função) */
void printAdjList(AdjList const ADJList){
	for (auto it:ADJList){
		cout << "Vertice:" <<it.first<< "---->";
		for (auto it2:it.second){
			cout << " " << it2.first<<":" <<it2.second<<" ";
		}
		cout << "\n";
	}
}

/*Função que vai do destino até o source procurando pelo caminho, em seguida
retorna qual deve ser o próximo passo a ser tomado pelo robô*/
int findnextMove(int destinoVertice,int sourceVertex, map<int, visitedNODE> &visited){
	int curentvertice = destinoVertice;
	while (visited[curentvertice].prec != sourceVertex){
		curentvertice = visited[curentvertice].prec;
	}
	return curentvertice;
}

/*Função que retorna qual deveria ser o caminho se não houvesse concorrência entre robôs*/
int findGroundTruth(int nextStep,int destinoVertice,int sourceVertex, map<int, visitedNODE> &visited, map<string, queue<POS>> &PathsMapGroundTruth,  TypesListMAP &sourcemap, map<int,POS> &MapvertexPair){
	int curentvertice = destinoVertice;
	while (visited[curentvertice].prec != sourceVertex){
		curentvertice = visited[curentvertice].prec;
		PathsMapGroundTruth[sourcemap[sourceVertex].orig].push(MapvertexPair[curentvertice]);
	}
	return curentvertice;
}

/*Encontra para cada um dos sources um goal*/
int getDestination(TypesListMAP &destinationmap, map<int, visitedNODE> &visited){
	int destinoVertice = 0;
	int teste = 99999999;
	for (auto it:destinationmap){
		if (visited[it.first].time < teste && it.second.used != 1){
			destinoVertice = it.first;
			teste = visited[it.first].time;
		}
	}
	return destinoVertice;
}

/*Para cada movimento tomado pelos robôs chama-se essa função para atualizar a Lista de adjacência*/
void updateAdjList(queue<int> &sourceFifo,TypesListMAP &sourcemap,AdjList &ADJList, int sourceVertex, int nextStep, int destinoVertice, map<int,POS> &MapvertexPair ,map<string, queue<POS>> &PathsMap){
	sourcemap[nextStep].vertice = nextStep;
	sourcemap[nextStep].goal = sourcemap[sourceVertex].goal;
	sourcemap[nextStep].orig = sourcemap[sourceVertex].orig;

	if (nextStep != sourcemap[sourceVertex].goal){
		sourceFifo.push(nextStep);
	}

	for (auto it:ADJList[sourceVertex]){
		ADJList[it.first][sourceVertex] = 1;	
	}
	for (auto it:ADJList){
		if (ADJList[it.first].find(nextStep) != ADJList[it.first].end()){
			ADJList[it.first][nextStep] = 0;
		}
	}	
}

/*BFS teste (Ignorar)*/
void BFS2(int sourceVertex, int numofvertices, AdjList &ADJList,TypesListMAP &destinationmap, TypesListMAP &sourcemap,queue<int> &sourceFifoAux,map<int,POS> &MapvertexPair ,map<string, queue<POS>> &PathsMapGroundTruth){
	map<int, visitedNODE> visited;
	queue<int> queue;
	int time = 0;
	int dobfs = 0;
	queue.push(sourceVertex);
    visited[sourceVertex].time = time;
    visited[sourceVertex].prec = -1;
   	int Start = 0;
   	if (ADJList[sourceVertex].size() == 0){
   		sourceFifoAux.push(sourceVertex);
   		return;
   	}
   	for (auto it:ADJList[sourceVertex]){
   		if (it.second == 1){
   			dobfs = 1;
   		}
   	}
   	if (dobfs==0){
   		sourceFifoAux.push(sourceVertex);
   		return;	
   	}
	while(!queue.empty()){
        Start = queue.front();  
        int x=0;
        queue.pop();
        for (auto it:ADJList[Start]){

        	if (it.second == 1){
	        	if (visited.find(it.first) == visited.end()){
	        		x=1;
		        	queue.push(it.first);
	        		visited[it.first].prec = Start;
	        		visited[it.first].time = visited[Start].time + 1;
	        	}
        	}
        }
	}
	int destinoVertice = 0;
	if (sourcemap[sourceVertex].goal == -1){
		destinoVertice = getDestination(destinationmap, visited);
		destinationmap[destinoVertice].used = 1;
		sourcemap[sourceVertex].goal = destinoVertice;
	}
	
	// cout <<sourceVertex<<"<<- source .destino ->>" <<destinoVertice << endl;
	
	int nextStep = findGroundTruth(nextStep, sourcemap[sourceVertex].goal, sourceVertex, visited, PathsMapGroundTruth, sourcemap, MapvertexPair);
}

/*BFS método que realiza o cálculo dos caminhos mínimos, essa função é chamada a cada movimento 
para cada um dos robôs, após cada iteração atualiza-se a lista de adjacencia chamando a updateAdjList()*/
void BFS(int sourceVertex, int numofvertices, AdjList &ADJList,TypesListMAP &destinationmap, TypesListMAP &sourcemap,queue<int> &sourceFifo,map<int,POS> &MapvertexPair ,map<string, queue<POS>> &PathsMap){
	map<int, visitedNODE> visited;
	queue<int> queue;
	int time = 0;
	int dobfs = 0;
	queue.push(sourceVertex);
    visited[sourceVertex].time = time;
    visited[sourceVertex].prec = -1;
   	int Start = 0;
   	if (ADJList[sourceVertex].size() == 0){
   		sourceFifo.push(sourceVertex);
   		return;
   	}
   	for (auto it:ADJList[sourceVertex]){
   		if (it.second == 1){
   			dobfs = 1;
   		}
   	}
   	if (dobfs==0){
   		sourceFifo.push(sourceVertex);
   		return;	
   	}
	while(!queue.empty()){
        Start = queue.front();  
        int x=0;
        queue.pop();
        for (auto it:ADJList[Start]){
        	if (it.second == 1){
	        	if (visited.find(it.first) == visited.end()){
	        		x=1;
		        	queue.push(it.first);
	        		visited[it.first].prec = Start;
	        		visited[it.first].time = visited[Start].time + 1;
	        	}
        	}
        }
	}

	int destinoVertice = 0;
	if (sourcemap[sourceVertex].goal == -1){
		destinoVertice = getDestination(destinationmap, visited);
		destinationmap[destinoVertice].used = 1;
		sourcemap[sourceVertex].goal = destinoVertice;
	}
		
	int nextStep = findnextMove(sourcemap[sourceVertex].goal, sourceVertex, visited);

	PathsMap[sourcemap[sourceVertex].orig].push(MapvertexPair[nextStep]); 
	updateAdjList(sourceFifo ,sourcemap, ADJList, sourceVertex, nextStep, destinoVertice, MapvertexPair , PathsMap);
}	

int main(int argc,char *argv[]){
	if (argc != 2){
		cout << "Wrong parameters, please use ./tp1-problema1 ambiente.txt" <<endl;
	return 0;
	}

	GraphMap graphMap;
	AdjList ADJList;
	map<int,POS> MapvertexPair;
	map<string, queue<POS>> PathsMap;
	map<string, queue<POS>> PathsMapGroundTruth;
	TypesListMAP sourcemap;
	queue<int> sourceFifo;
	queue<int> sourceFifoAux;

	TypesListMAP destinationmap;
	sizeOfMatrix numOfVertex;
	
		string ambientefile(argv[1]);

	numOfVertex = readAmbientFile(ambientefile, graphMap, sourcemap, destinationmap, sourceFifo, MapvertexPair);
	getAdjList(numOfVertex,graphMap, ADJList);
	sourceFifoAux = sourceFifo;
	/*Enquanto nao terminar de fazer o BFS para todos os passos de todos robos faça*/
	while (!sourceFifo.empty()){
		BFS(sourceFifo.front(),numOfVertex.lines*numOfVertex.colums,ADJList, destinationmap, sourcemap, sourceFifo, MapvertexPair,PathsMap);
		sourceFifo.pop();
	}

	/*Imprime na tela...*/
	ofstream output;
	output.open("output.txt");
	while (!sourceFifoAux.empty()){
		for (auto it:PathsMap){
			output << it.first<< ": [("<<MapvertexPair[sourceFifoAux.front()].first<<","<<MapvertexPair[sourceFifoAux.front()].second<<"), ";
			while (!it.second.empty()){
				if (it.second.size() == 1){
					output << "("<<it.second.front().first<<","<< it.second.front().second<<")]";
				}else{
					output << "("<<it.second.front().first<<","<< it.second.front().second<<"),";
				}
				it.second.pop();
			}	
		output<<"\n";
		sourceFifoAux.pop();
		}
	}
	
	return 0;
}
