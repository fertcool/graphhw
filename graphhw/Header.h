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
	
	//ф-ии нахождения длины списка
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
	int length(list<int[3]>* edgelist) 
	{
		int length = 0;//количество вершин
		for (list<int[3]>* current = edgelist; current; current = current->next)//нахождение количества вершин(максимальная вершина в списке ребер)
		{
			if (length < current->Ver[0])
				length = current->Ver[0];
			if (length < current->Ver[1])
				length = current->Ver[1];
		}
		return length;
	}
	int length(list<list<int>*>* adjlist)
	{
		int length = 0;
		for (list<list<int>*>* cur = adjlist; adjlist; cur = cur->next)
		{
			for (list<int>* sub_cur = cur->Ver; sub_cur; sub_cur = sub_cur->next)
			{
				if (sub_cur->Ver > length)
					length = sub_cur->Ver;
			}
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
	void clear()
	{
		list* current = this;
		list* boof;//буферный указатель для удаления
		if (current->next == NULL) //если пуст
		{
			delete current;
			current = new list;//создаем новый
			return;
		}
		while (current)//цикл по элементам списка
		{
			boof = current;//запоминаем текущий
			current = current->next;//делаем текущий следующим
			delete boof;//удаляем текущий
		}
		delete current;//удаляем лишний элемент
		current = new list;//создаем новый
	}
	//ф-я добавления в конец списка
	void add(T add_data)
	{
		list* current = this;
		for (current; current->next; current = current->next) {}
		current->next = new list<T>;
		current->next->Ver = add_data;
	}
	//*-------------- Функции для сортировки списка ------------------*//
	list* getTail(list* cur)
	{
		while (cur != NULL && cur->next != NULL)
			cur = cur->next;
		return cur;
	}
	list* partition(list* head, list* end, list** newHead, list** newEnd,int i)
	{
		list* pivot = end;
		list* prev = NULL,
	    *cur = head, * tail = pivot;

		// During partition, both the head and 
		// end of the list might change which 
		// is updated in the newHead and newEnd 
		// variables
		while (cur != pivot)
		{
			if (cur->Ver[i] < pivot->Ver[i])
			{
				 // First list that has a value 
				 // less than the pivot - becomes 
				 // the new head
				 if ((*newHead) == NULL)
					 (*newHead) = cur;

				 prev = cur;
				 cur = cur->next;
			}

			// If cur list is greater than pivot
			else
			{
				// Move cur list to next of tail, 
				// and change tail
				if (prev)
					 prev->next = cur->next;
				list* tmp = cur->next;
				cur->next = NULL;
				tail->next = cur;
				tail = cur;
				cur = tmp;
			}
		}
		// If the pivot data is the smallest element 
		// in the current list, pivot becomes the head
		if ((*newHead) == NULL)
			(*newHead) = pivot;

		// Update newEnd to the current last list
		(*newEnd) = tail;

		// Return the pivot list
		return pivot;
	}
	list* quickSortRecur(list* head, list* end ,int i)
	 {
		 // Base condition
		 if (!head || head == end)
			 return head;

		 list* newHead = NULL, * newEnd = NULL;

		 // Partition the list, newHead and newEnd 
		 // will be updated by the partition function
		 list* pivot = partition(head, end, &newHead, &newEnd,i);

		 // If pivot is the smallest element - no need 
		 // to recur for the left part.
		 if (newHead != pivot)
		 {
			 // Set the list before the pivot list 
			 // as NULL
			 list* tmp = newHead;
			 while (tmp->next != pivot)
				 tmp = tmp->next;
			 tmp->next = NULL;

			 // Recur for the list before pivot
			 newHead = quickSortRecur(newHead, tmp,i);

			 // Change next of last list of the 
			 // left half to pivot
			 tmp = getTail(newHead);
			 tmp->next = pivot;
		 }

		 // Recur for the list after the 
		 // pivot element
		 pivot->next = quickSortRecur(pivot->next, newEnd,i);

		 return newHead;
	 }
	 // The main function for quick sort. 
	 // This is a wrapper over recursive 
	 // function quickSortRecur()
	void quickSort(list** headRef, int i = 0)
	 {
		 (*headRef) = quickSortRecur(*headRef, getTail(*headRef), i);
		 return;
	 }
};
//*-------------- Класс Графа ------------------*//
class Graph
{
private:
	list<int[3]>* edgelist;
public:
	inline Graph() {};
	inline Graph(vector<vector<int>>* Matradj) { edgelist = list_of_edges(Matradj); };
	inline Graph(list<int[3]>* Edgel) { edgelist = Edgel; };
	inline Graph(list<list<int>*>* Adjl) { edgelist = list_of_edges(Adjl); };
	Graph(string FPath, string FType);
	~Graph(){};
	int weight(int Ver1, int Ver2); //возвращает вес ребра по его вершинам
	bool is_edge(int Ver1, int Ver2); //существует ли ребро по данным вершинам
	vector<vector<int>>* adjacency_matrix();//возвращает матрицу смежности из списка ребер
	list<list<int>*>* adjacency_list();//возвращает список сежности из списка ребер
	list<int[3]>* list_of_edges();//возвращает копию списка ребер
	list<int[3]>* list_of_edges(vector<vector<int>>* matrix);//возвращает список ребер из введенной матрицы смежности
	list<int[3]>* list_of_edges(list<list<int>*>* adjlist);//возвращает список ребер из введенного списка смежности
	list<int>* adjacency_list(int Ver); //возвращает список вершин смежных Ver
	list<int>* adjacency_list_in(int Ver); //возвращает список вершин входящих в Ver
	list<int[3]>* list_of_edges(int Ver); //возвращает список всех ребер инцидентных Ver
	bool is_directed(); //граф ориентированный или нет
	Graph& operator=(const Graph& graph);//копирование графа
	
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
//печать матрицы
void print_matrix(vector<vector<int>>*matrix, ostream & stream_out);
//печать вектора
template<typename T>
void print_vector(vector<T>* vec, ostream& stream_out);
//печать компонент связности
void print_comp(int length, vector<int>& used, int num_comp, ostream& stream_out, bool is_directed, bool strong_comp);
//печать остовного дерева
void print_spanningtree(list<int*>* spanning_tree, ostream& stream_out, int cost);

//*-------------- Ф-ии Программ ------------------*//
void first_task(int argc, char* argv[],Graph GRAPH, ostream& stream_out);
void second_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out);
void third_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out);
void fourth_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out);
void fifth_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out);
void sixth_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out);
//*-------------- Алгоритмы ------------------*//
//алгоритм флойда
vector<vector<int>>* Floyd_Warshall(vector<vector<int>>* matrix);
//топологическая сортировка графа
vector<int>* TopologicalSort(Graph GRAPH);
//поиск в ширину
void BFS(Graph GRAPH, vector<int>* used, int Ver, int mark);
//поиск в глубину
void DFS(Graph GRAPH, vector<int>* used, int Ver, int mark, vector<int>* order);
//поиск в глубину для работы с поиском мостов
void DFS_BRIDGES(Graph GRAPH, vector<bool>* used, int Ver, int timer, vector<int>* tin, vector<int>* tup, list<int*>* bridges, int back = 0);
//поиск в глубину для рбаботы с поиском шарниров
void DFS_CUTVERTEXES(Graph GRAPH, vector<bool>* used, vector<bool>* is_cut, int Ver, int timer, vector<int>* tin, vector<int>* tup, int back = 0);
//алгоритм Крускала
int Kruscal(Graph GRAPH, list<int*>* spanning_tree);
//алгоритм Прима
int Prim(Graph GRAPH, list<int*>*& spanning_tree);
//алгоритм Борувки
int Boruvka(Graph GRAPH, list<int*>*& spanning_tree);
//доп. функции для Борувки
int find(vector<int>& parent, int i);
void unionSet(vector<int>& parent, vector<int>& rank, int x, int y);
//алгоритм дейкстры
int Dijkstra(Graph GRAPH, vector<int>& answ, vector<int>& prev, int begin_Ver, int end_Ver = 1);
int Dijkstra_Ford(Graph GRAPH, vector<int>& answ, vector<int>& prev, int begin_Ver, int end_Ver = 1);
