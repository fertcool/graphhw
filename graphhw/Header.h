#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//����� ������
template<typename T>
struct list
{
	T Ver;
	list* next;
	list()
	{
		next = NULL;

	}
	
	//�-� ���������� ����� ������
	int length()
	{
		int length = 0;
		list* current = this;
		for (current; current; current = current->next)
		{
			length++;
		}
		return length;
	}
	//�-� �������� ���������� ��������
	void del()
	{
		list* current = this;
		list* current_before = this;
		for (current; current->next; current = current->next)
		{
			current_before = current;
		}
		delete current_before->next;
		current_before->next = NULL;
	}

	
};
//����� �����
class Graph
{
private:
	list<list<int>*>* adjlist;
	list<int[3]>* edgelist;
	vector<vector<int>>* matrix;
public:
	Graph() {};
	Graph(string FPath, string FType);
	~Graph(){};
	int weight(int Ver1, int Ver2); //���������� ��� ����� �� ��� ��������
	bool is_edge(int Ver1, int Ver2); //���������� �� ����� �� ������ ��������
	vector<vector<int>>* adjacency_matrix(bool copy = true); //���������� ������� ���������
	list<list<int>*>* adjacency_list(bool copy = true); //���������� ������ ���������
	list<int>* adjacency_list(int Ver); //���������� ������ ������ ������� Ver
	list<int>* adjacency_list_in(int Ver); //���������� ������ ������ �������� � Ver
	list<int[3]>* list_of_edges(bool copy = true); //���������� ������ �����
	list<int[3]>* list_of_edges(int Ver); //���������� ������ ���� ����� ����������� Ver
	bool is_directed(); //���� ��������������� ��� ���
};
//�-�� ��������

//������������� ����� � ������ (����������� (������ + 1) ��� (-1))
int exist_key(int argc, char* argv[], string key);
//���� ������ 1 ���� ����� �����
bool once_keyfile(int argc, char* argv[]);
//�������
void help(int argc, char* argv[]);
//����� ���������
int num_programm(int argc, char* argv[]);
//������� ����, ���� �� �� ����������, � ���������� ��� �����
ofstream create_file(string filename);

//�-�� ��� ��������
void first_task(int argc, char* argv[],Graph g, ostream& stream_out);
void print_matrix(vector<vector<int>>* matrix, ostream& stream_out);
void print_vector(vector<int>* vec, ostream& stream_out);
vector<vector<int>>* Floyd_Warshall(vector<vector<int>>* matrix); //��� 1 �������