#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <limits.h>
#include <queue>
#include <fstream>
#include <string>
#include <istream>
#include <map>

using namespace std;
 

struct NODE{
    string id;
    int vertice;    
};
typedef std::pair<int,int> POS;
typedef std::map<POS,NODE> GraphMap;

struct sizeOfMatrix{
    int lines;
    int colums;
    int vertexnumber;
};


/*Realiza a leitura do arquivo e modelagem em grafo, salva isso no GraphMap*/
sizeOfMatrix readAmbientFile(string ambientefile, GraphMap &graphMap){
    ifstream inputfile;
    inputfile.open(ambientefile);
    string line = "";
    int vertexnumber = 1;
    sizeOfMatrix tempSize;
    tempSize.lines= 0;
    tempSize.colums= 0;
    tempSize.vertexnumber = 0;
    int columnNum = 1;
    int  lineNum = 1;

    while (getline(inputfile,line)){
        istringstream iss(line);
        while (iss){
            std::string sub;
            iss >> sub;
            if (sub.length()){
                tempSize.colums = columnNum;
                if (sub[0] == 'g'){
                    graphMap[POS(lineNum,columnNum)].id = sub;
                    graphMap[POS(lineNum,columnNum)].vertice = vertexnumber;
                    vertexnumber++;
                }
                columnNum++;
            }
        }
        tempSize.lines = lineNum;
        lineNum++;
        columnNum = 1;
        
    }
    tempSize.vertexnumber = vertexnumber-1;
    return tempSize;
}


/*Realiza o cálculo da Matriz de adjacência do grafo. Caso possua vizinhos, adiciona
o peso da aresta como 1 na Matriz de adjacencia nas posições (i,j)*/
void getAdjMatrix(sizeOfMatrix matrixSize, GraphMap &graphMap, vector<vector<int>> &adjmatrix){
    int VertexIndex = 0;
    for (int i = 0; i <= matrixSize.lines; i++ ){
        for (int j = 0; j <= matrixSize.colums; j++ ){
            if (graphMap[POS(i,j)].id[0] == 'g'){
                if (graphMap[POS(i+1,j)].id[0] == 'g'){
                    adjmatrix[graphMap[POS(i,j)].vertice][graphMap[POS(i+1,j)].vertice] = 1;
                }
                if (graphMap[POS(i-1,j)].id[0] == 'g'){
                    adjmatrix[graphMap[POS(i,j)].vertice][graphMap[POS(i-1,j)].vertice] = 1;
                }
                if (graphMap[POS(i,j+1)].id[0] == 'g'){
                    adjmatrix[graphMap[POS(i,j)].vertice][graphMap[POS(i,j+1)].vertice] = 1;
                }
                if (graphMap[POS(i,j-1)].id[0] == 'g'){
                    adjmatrix[graphMap[POS(i,j)].vertice][graphMap[POS(i,j-1)].vertice] = 1;
                }
                if (graphMap[POS(i+1,j+1)].id[0] == 'g'){
                    adjmatrix[graphMap[POS(i,j)].vertice][graphMap[POS(i+1,j+1)].vertice] = 1;
                }
                if (graphMap[POS(i-1,j-1)].id[0] == 'g'){
                    adjmatrix[graphMap[POS(i,j)].vertice][graphMap[POS(i-1,j-1)].vertice] = 1;
                }
                if (graphMap[POS(i-1,j+1)].id[0] == 'g'){
                    adjmatrix[graphMap[POS(i,j)].vertice][graphMap[POS(i-1,j+1)].vertice] = 1;
                }
                if (graphMap[POS(i+1,j-1)].id[0] == 'g'){
                    adjmatrix[graphMap[POS(i,j)].vertice][graphMap[POS(i+1,j-1)].vertice] = 1;
                }
            }
        }
    }
    for (int j = 0; j <= matrixSize.colums; j++ ){
        if (graphMap[POS(1,j)].id[0] == 'g'){
            adjmatrix[0][graphMap[POS(1,j)].vertice] = 1;
        }
    }
    for (int j = 0; j <= matrixSize.colums; j++ ){
        if (graphMap[POS(matrixSize.lines,j)].id[0] == 'g'){
            adjmatrix[graphMap[POS(matrixSize.lines,j)].vertice][matrixSize.vertexnumber+1] = 1;
        }
    }

    // cout <<VertexIndex<< endl;
}

bool BFS(vector<vector<int>> &graphResidual, int source, int sink, vector<int> &precessor, sizeOfMatrix matrixSize){
    /*Inicializa vetor de vertices visitados*/
    vector<bool> visited(matrixSize.vertexnumber+2,0);

    //Coloca vertices visitados na queue
    queue <int> queue;
    queue.push(source);
    visited[source] = 1;
    precessor[source] = -99999;
 
    while (!queue.empty()){
        int u = queue.front();
        queue.pop();

        for (int v=0; v<matrixSize.vertexnumber+2; v++){
            if (visited[v]==0 && graphResidual[u][v] > 0){
                queue.push(v);
                precessor[v] = u;
                visited[v] = 1;
            }
        }
    }

    return (visited[sink] == 1);
}
 
/*Retorna o fluxo máximo do grafo da source até sink, no nosso caso é 
do norte até o sul ..*/
int fordFulkerson(vector<vector<int>> &adjmatrix, int source, int sink, sizeOfMatrix matrixSize){
    int u, v;
    //Inicia fluxo com 0 e vai atualizando a cada iteração do BFS
    vector<int> precessor(matrixSize.vertexnumber+2);
    int fluxo_max = 0; 
    /*Cria um grafo residual, que é uma cópia da matriz de adjacência
    sera utilizado para melhorar o fluxo*/
    vector<vector<int>> graphResidual(matrixSize.vertexnumber+2, vector<int>(matrixSize.vertexnumber+2));

    // Inicializa valores do grafo residual fazendo copia da ADJmatrix    
        for (int i = 0; i <= matrixSize.vertexnumber+1; i++ ){
            for (int j = 0; j <= matrixSize.vertexnumber+1; j++ ){
                graphResidual[i][j] = adjmatrix[i][j];
            }
        }

    /*Enquanto encontrar camiho de source -> sink retorna 1*/
    while (BFS(graphResidual, source, sink, precessor, matrixSize)){
        int fluxo_atual = 99999999;
        for (v=sink; v!=source; v=precessor[v]){
            u = precessor[v];
            fluxo_atual = min(fluxo_atual, graphResidual[u][v]);
        }

        for (v=sink; v != source; v=precessor[v]){
            u = precessor[v];
            graphResidual[u][v] -= fluxo_atual;
            graphResidual[v][u] += fluxo_atual;
        }
 
        fluxo_max += fluxo_atual;
    }
    return fluxo_max;
}
 
int main(int argc,char *argv[]){
    GraphMap graphMap;
    ofstream output;
    output.open("output.txt");
    sizeOfMatrix matrixSize;
    if (argc != 2){
        cout << "Wrong parameters, please use ./tp1-problema3 ambiente.txt" <<endl;
    return 0;
    }
        string ambientefile(argv[1]);
        matrixSize = readAmbientFile(ambientefile,graphMap);
        vector<vector<int>> adjmatrix(matrixSize.vertexnumber+2, vector<int>(matrixSize.vertexnumber+2));
        getAdjMatrix(matrixSize, graphMap, adjmatrix);
      
    int fluxo_maximo = fordFulkerson(adjmatrix, 0, matrixSize.vertexnumber+1, matrixSize);
    output << fluxo_maximo;
 
    return 0;
}