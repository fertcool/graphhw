#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//*-------------- Класс Списка ------------------*//
template<typename T>
struct list
{
	T Ver;
	list* next;
	list()
	{
		next = NULL;

	}
	
	//ф-я нахождения длины списка
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
	//ф-я удаления последнего элемента
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
//*-------------- Класс Графа ------------------*//
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
	int weight(int Ver1, int Ver2); //возвращает вес ребра по его вершинам
	bool is_edge(int Ver1, int Ver2); //существует ли ребро по данным вершинам
	vector<vector<int>>* adjacency_matrix(bool copy = true); //возвращает матрицу смежности
	list<list<int>*>* adjacency_list(bool copy = true); //возвращает список смежности
	list<int>* adjacency_list(int Ver); //возвращает список вершин смежных Ver
	list<int>* adjacency_list_in(int Ver); //возвращает список вершин входящих в Ver
	list<int[3]>* list_of_edges(bool copy = true); //возвращает список ребер
	list<int[3]>* list_of_edges(int Ver); //возвращает список всех ребер инцидентных Ver
	bool is_directed(); //граф ориентированный или нет
};
//*-------------- Ф-ии Парсинга ------------------*//

//существование ключа в строке (возвраащает (индекс + 1) или (-1))
int exist_key(int argc, char* argv[], string key);
//есть только 1 ключ файла ввода
bool once_keyfile(int argc, char* argv[]);
//справка
void help(int argc, char* argv[]);
//номер программы
int num_programm(int argc, char* argv[]);
//создает файл, если он не существует, и возвращает его поток
ofstream create_file(string filename);

//*-------------- Ф-ии Вывода ------------------*//
void print_matrix(vector<vector<int>>*matrix, ostream & stream_out);
void print_vector(vector<int>* vec, ostream& stream_out);

//*-------------- Ф-ии Программ ------------------*//
void first_task(int argc, char* argv[],Graph g, ostream& stream_out);

//*-------------- Алгоритмы ------------------*//
//алгоритм флойда
vector<vector<int>>* Floyd_Warshall(vector<vector<int>>* matrix); //для 1 задания
//поиск в ширину
void BFS(Graph GRAPH, vector<bool>* used, int Ver);
