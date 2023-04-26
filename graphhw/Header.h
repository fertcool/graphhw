#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

template<typename T>
struct list
{
	T Ver;
	list* next;
	list()
	{
		next = NULL;
	}
	
};
class Graph
{
public:
	list<list<int>*>* adjlist;
	list<int[3]>* edgelist;
	vector<vector<int>>* matrix;
public:
	Graph(string FPath, string FType);
	~Graph(){};
	//float weight(int Ver1, int Ver2); //возвращает вес ребра по его вершинам
	//bool is_edge(int Ver1, int Ver2); //существует ли ребро по данным вершинам
	//int** adjacency_matrix(); //возвращает матрицу смежности
	//list adjacency_list(int Ver); //возвращает список вершин смежных Ver
	//list list_of_edges(); //возвращает список ребер
	//list list_of_edges(int Ver); //возвращает список всех ребер инцидентных Ver
	//bool is_directed(); //граф ориентированный или нет
};