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
	//float weight(int Ver1, int Ver2); //���������� ��� ����� �� ��� ��������
	//bool is_edge(int Ver1, int Ver2); //���������� �� ����� �� ������ ��������
	//int** adjacency_matrix(); //���������� ������� ���������
	//list adjacency_list(int Ver); //���������� ������ ������ ������� Ver
	//list list_of_edges(); //���������� ������ �����
	//list list_of_edges(int Ver); //���������� ������ ���� ����� ����������� Ver
	//bool is_directed(); //���� ��������������� ��� ���
};