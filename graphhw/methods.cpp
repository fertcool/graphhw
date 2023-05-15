#pragma once
#define INF 9999999
#include "Header.h"
#include <iomanip>
#include <fstream>
#include <queue>
#include <stack>
#include <set>
#include <chrono>
#include <map>
//*-------------- ф-ии класса графа ------------------*//
//возвращает вес ребра по его вершинам
int Graph::weight(int Ver1, int Ver2) 
{
	for (list<int[3]>* cur = edgelist; cur; cur = cur->next)
	{
		if (cur->Ver[0] == Ver1 && cur->Ver[1] == Ver2)
			return cur->Ver[2];
	}
	return NULL;
};

//существует ли ребро по данным вершинам
bool Graph::is_edge(int Ver1, int Ver2)
{
	if (weight(Ver1, Ver2))
		return true;
	return false;
};

//граф ориентированный или нет
bool Graph::is_directed() 
{
	vector<vector<int>>* matrix = adjacency_matrix();
	int msize = matrix->size();
	for (int i = 0; i < msize - 1; i++)//цикл по элементам выше главной диаганали
	{
		for (int j = i + 1; j < msize; j++)
		{
			if (!((*matrix)[i][j] && (*matrix)[j][i]) && !((*matrix)[i][j] == 0 && (*matrix)[j][i] == 0))
				return true;
		}
	}
	return false;

};

//возвращает матрицу смежности
vector<vector<int>>* Graph::adjacency_matrix()
{
	//нахождение размера матрицы(максимальная вершина в списке ребер)
	int h_matr = 0;
	list<int[3]>* current = edgelist;
	for (current; current; current = current->next)//нахождение размера матрицы(максимальная вершина в списке ребер)
	{
		if (h_matr < current->Ver[0])
			h_matr = current->Ver[0];
		if (h_matr < current->Ver[1])
			h_matr = current->Ver[1];
	}

	vector<vector<int>>* matrix = new vector<vector<int>>;
	(*matrix).resize(h_matr);
	for (int i = 0; i < h_matr; i++)//выделение памяти
	{
		(*matrix)[i].resize(h_matr);
	}
	for (int i = 0; i < h_matr; i++)//заполнение матрицы 0
	{
		for (int j = 0; j < h_matr; j++)
		{
			(*matrix)[i][j] = 0;
		}
	}

	current = edgelist;
	for (current; current; current = current->next)//цикл заполнения матрицы
	{
		(*matrix)[current->Ver[0] - 1][current->Ver[1] - 1] = current->Ver[2];
	}

	return matrix;

}

//возвращает список смежности
list<list<int>*>* Graph::adjacency_list()
{
	vector<vector<int>>* matrix = adjacency_matrix();
	int lsize = matrix->size();//размер списка (матрицы)
	list<list<int>*>* adjlist = new list<list<int>*>; //выделение памяти под список списков вершин
	adjlist->Ver = new list<int>; //выделение памяти под список вершин
	list<list<int>*>* current = adjlist;
	for (int i = 0; i < lsize; i++)
	{
		list<int>* current_sub = current->Ver;//указатель на текущий список вершин
		for (int j = 0; j < lsize; j++)
		{
			if ((*matrix)[i][j])
			{
				current_sub->Ver = j + 1; //добавление вершины в список
				current_sub->next = new list<int>;//итерация
				current_sub = current_sub->next;
			}
		}
		current->Ver->del();//удаляем лишний элемент
		current->next = new list<list<int>*>;//итерация
		current->next->Ver = new list<int>;
		current = current->next;
	}
	adjlist->del();//удаляем лишний элемент
	return adjlist;
};

//возвращает список вершин смежных Ver
list<int>* Graph::adjacency_list(int Ver)
{
	list<list<int>*>* adjlist = adjacency_list();

	list<int>* adjlist_c = new list<int>;//копия списка смежных вершин
	int init = adjlist_c->Ver;//изначальный мусор в списке
	list<int>* current_c = adjlist_c; // текущая вершина в копии списка
	list<list<int>*>* current = adjlist;//текущая вершина в основном списке
	for (int i = 0; i < Ver - 1; i++)//цикл смещения указателя на нужный список смежных вершин
	{
		current = current->next;
	}
	list<int>* current_sub = current->Ver;//текущая вершина j смежная с Ver
	for (current_sub;current_sub;current_sub = current_sub->next)//цикл копирования
	{
		current_c->Ver = current_sub->Ver;//копирование
		if (current_sub->next)//выделение памяти для следующего элемента
		{
			current_c->next = new list<int>;
			current_c = current_c->next;
		}
	}
	if (adjlist_c->Ver == init)//если список вершин пуст
		return NULL;
	return adjlist_c;
}
//возвращает список вершин входящих в Ver
list<int>* Graph::adjacency_list_in(int Ver)
{
	//со3дание списка смежности
	vector<vector<int>>* matrix = adjacency_matrix();

	list<int>* adjlistin_c = new list<int>;//копия списка смежных вершин
	int init = adjlistin_c->Ver;//изначальный мусор в списке
	list<int>* current_c = adjlistin_c; // текущая вершина в копии списка
	
	int length = matrix->size();//длина списка (количество вершин)
	for (int i = 0; i<length; i++) //цикл по всем вершинам основного списка
	{
		if ((*matrix)[i][Ver - 1])
		{
			current_c->Ver = i + 1;
			if (i != length - 1)
			{
				current_c->next = new list<int>;
				current_c = current_c->next;
			}
		}
	}
	if(current_c->Ver!=length)
		adjlistin_c->del();
	if (adjlistin_c->Ver == init)//если список вершин пуст
		return NULL;
	return adjlistin_c;
}

//возвращает копию списка ребер
list<int[3]>* Graph::list_of_edges()
{
	//возврат копии списка ребер
	list<int[3]>* edgelist_c = new list<int[3]>;//копия списка ребер
	list<int[3]>* current_c = edgelist_c;//текущее ребро в копии
	list<int[3]>* current = edgelist;//текущее ребро в основном списке
	int length = edgelist->length();//длина списка ребер
	for (int i = 0; i < length; i++)//цикл копирования
		{
			current_c->Ver[0] = current->Ver[0];
			current_c->Ver[1] = current->Ver[1];//копирование
			current_c->Ver[2] = current->Ver[2];
			if (i != length - 1)//выделение памяти для следующего элемента
			{
				current_c->next = new list<int[3]>;
				current_c = current_c->next;
				current = current->next;
			}
		}
	
	return edgelist_c;
	

};
list<int[3]>* Graph::list_of_edges(vector<vector<int>>* matrix)
{
	
	int lsize = matrix->size();//размер списка (матрицы)
	edgelist = new list<int[3]>;//выделенеи памяти для ребра
	list<int[3]>* current = edgelist; //текущее ребро
	for (int i = 0; i < lsize; i++)
	{
		for (int j = 0; j < lsize; j++)
		{
			if ((*matrix)[i][j]) //если вершины связаны
			{
				current->Ver[0] = i + 1;//1 вершина
				current->Ver[1] = j + 1;//2 вершина
				current->Ver[2] = (*matrix)[i][j];//вес ребра
				current->next = new list<int[3]>;
				current = current->next;
			}
		}
	}
	edgelist->del();
	return list_of_edges();
}

list<int[3]>* Graph::list_of_edges(list<list<int>*>* adjlist)
{
	int h_matr = adjlist->length();//длина матрицы

	vector<vector<int>>* matrix = new vector<vector<int>>;
	(*matrix).resize(h_matr);
	for (int i = 0; i < h_matr; i++)//выделение памяти
	{
		(*matrix)[i].resize(h_matr);
	}

	for (int i = 0; i < h_matr; i++)//заполнение матрицы 0
	{
		for (int j = 0; j < h_matr; j++)
		{
			(*matrix)[i][j] = 0;
		}
	}

	list<list<int>*>* current = adjlist;
	int i = 0;
	for (current; current; current = current->next)//цикл заполнения матрицы
	{
		list<int>* subcurrent = current->Ver;
		for (subcurrent; subcurrent; subcurrent = subcurrent->next)
		{
			(*matrix)[i][subcurrent->Ver - 1] = 1;
		}
		i++;
	}
	//возврат списка ребер по полученной из списка смежности матрицы
	return list_of_edges(matrix);
}
//возвращает список всех ребер иcходящих из Ver
list<int[3]>* Graph::list_of_edges(int Ver)
{
	
	list<int[3]>* list_out = new list<int[3]>;//выделение памяти для списка исходящих ребер
	int init = list_out->Ver[0];//изначальный мусор в списке
	list<int[3]>* current_out = list_out;//текущее ребро, исходящее из Ver
	
	
	for (list<int[3]>* cur = edgelist; cur;cur = cur->next)//цикл просмотра смежных с Ver вершин и добавления исходящего ребра в список ребер
	{
		if (cur->Ver[0]==Ver)
		{
			current_out->Ver[0] = Ver;//добавление 1 вершины
			current_out->Ver[1] = cur->Ver[1];//добавление исходящей вершины
			current_out->Ver[2] = cur->Ver[2];//добавление веса ребра

			current_out->next = new list<int[3]>;//выделение памяти
			current_out = current_out->next;//итерация
		}
	}
	list_out->del();//удаление лишнего элемента
	if (list_out->Ver[0] == init)//если список вершин пуст
		return NULL;
	return list_out;

};
//копирование графа
Graph& Graph::operator=(const Graph& graph)
{
	edgelist = graph.edgelist;
	return *this;
}
//*-------------- Функции Класса Map ------------------*//
Map::Map(string FPath) 
{
	//нахождение высоты матрицы
	ifstream in_ch(FPath); //поток по файлу
	if (!in_ch.is_open()) {
		cout << "Файл с картой!!!" << endl;
		return;
	}
	int h_matr = 1;
	string str_ch;
	string substr_ch;//проверочные переменные
	getline(in_ch, str_ch);
	istringstream strstream_ch(str_ch); //поток по строке
	while (!strstream_ch.eof()) //цикл считывания 
	{
		strstream_ch >> substr_ch;
		if (!strstream_ch.eof())
			h_matr++;
	}
	string str;
	string substr;
	ifstream fin(FPath); //поток по файлу

	map = new vector<vector<int>>;
	(*map).resize(h_matr);
	for (int i = 0; i < h_matr; i++)//выделение памяти
	{
		(*map)[i].resize(h_matr);
	}

	for (int i = 0; i < h_matr; i++)
	{
		for (int j = 0; j < h_matr; j++)
		{
			fin >> (*map)[i][j];
		}
	}
}
int Map::GetHeight(Cell c) 
{
	if (c.x >= 0 && c.x < map->size() && c.y >= 0 && c.y < map->size())
		return (*map)[c.x][c.y];
	else
		return -1;
}
list<pair<int,Cell>>* Map::neighbors(Cell c)
{
	list<pair<int,Cell>>* nbrs = new list<pair<int,Cell>>;
	nbrs->Ver.second.x = -1;
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			if (!(dy || dx)||(dx == 1 && dy == 1)||(dx==-1&&dy==-1)|| (dx == 1 && dy == -1)|| (dx == -1 && dy == 1))
				continue;
			if (c.x + dx < 0 || c.x + dx >= map->size() || c.y + dy < 0 || c.y + dy >= map->size())
				continue;
			else
			{
				if (nbrs->Ver.second.x == -1)
				{
					nbrs->Ver.second.x = c.x + dx;
					nbrs->Ver.second.y = c.y + dy;
					nbrs->Ver.first = (*map)[c.x+dx][c.y+dy];
				}
				else
				{
					nbrs->add(pair<int, Cell>((*map)[c.x+dx][c.y+dy], Cell(c.x + dx, c.y + dy)));
				}

			}
		}
	}
	return nbrs;
}
Cell& Cell::operator=(const Cell cell)
{
	x = cell.x;
	y = cell.y;
	return *this;
}


//конструктор
Graph::Graph(string FPath, string FType)
{
	edgelist = NULL;

	if (FType == "-l")//ввод списка смежности
	{
		//реализовано без удаления лишних элементов//

		ifstream fin(FPath); //поток по файлу
		if (!fin.is_open()) {
			cout << "Файл с списком смежности не найден!!!" << endl;
			return;
		}
		list<list<int>*>* adjlist = new list<list<int>*>; //выделение памяти под список списков вершин
		adjlist->Ver = new list<int>; //выделение памяти под список вершин
		list<list<int>*>* current = adjlist;//указатель на текущий список с вершинами
		while (!fin.eof())
		{
			string str; //текущая строка с вершинами
			getline(fin, str); //считывание строки с вершинами
			if (str == "" && fin.eof()) //выход если конец файла(лишняя пустая строка)
			{
				adjlist->del();
				break;
			}
			if (str == "" && !fin.eof())
			{
				current->Ver = NULL;
				current->next = new list<list<int>*>;//выделение памяти 
				current->next->Ver = new list<int>;
				current = current->next;//итерация
				continue;
			}

			istringstream strstream(str); //поток по строке
			list<int>* current_sub = current->Ver; //указатель на текущую считываемую вершину
			
			//перед циклом делаем ввод первого числа 
			//(так сделано, чтобы дальше реализовать цикл с выделением памяти после чтения числа)
			int check_space;//проверка на пробел в конце строки
			strstream >> current_sub->Ver;//считывание числа из строчной вершины
	
			while (!strstream.eof()) //цикл считывания 
			{
				strstream >> check_space;//считывание числа из строчной вершины
				if (strstream.eof())//если конец строки, то выходим
					break;
				current_sub->next = new list<int>;//выделение памяти 
				current_sub = current_sub->next;//итерация
				current_sub->Ver = check_space;//считывание числа из строчной вершины
			}
			if (fin.eof())//выход если конец (чтобы лишнюю память не выделять)
				break;
			current->next = new list<list<int>*>;//выделение памяти 
			current->next->Ver = new list<int>;
			current = current->next;//итерация
			
		}
		edgelist = list_of_edges(adjlist);

	}
	if (FType == "-e")//ввод списка ребер
	{
		//реализовано без удаления лишних элементов//

		ifstream fin(FPath); //поток по файлу
		if (!fin.is_open()) {
			cout << "Файл с списком ребер не найден!!!" << endl;
			return;
		}
		edgelist = new list<int[3]>; //выделение памяти под список ребер

		list<int[3]>* current = edgelist;//указатель на текущий ребро
		
		while (!fin.eof())
		{
			string str; //строчка текущего ребра
			getline(fin, str); //считывание ребра
			if (str == "") //выход если конец файла(лишняя паустая строка)
			{
				edgelist->del();
				break;
			}
			istringstream strstream(str); //поток по строке

			string check_space;//проверка на пробел в конце строки
			//strstream >> current->Ver[0]; //считывание вершины или веса ребра

			for (int i = 0; i < 2; i++) //цикл считывания двух вершин
			{
				strstream >> current->Ver[i]; //считывание вершины или веса ребра
				
			}
			strstream >> check_space;//проверка и считывание веса (если пусто, то 1)
			if (check_space == "")
				current->Ver[2] = 1;
			else
				current->Ver[2] = stoi(check_space);

			if (fin.eof())//выход если конец (чтобы лишнюю память не выделять)
				break;
			current->next = new list<int[3]>; //выделение памяти
			current = current->next;//итерация
			
		}
	}
	if (FType == "-m")//ввод матрицы смежности
	{
		//нахождение высоты матрицы
		ifstream in_ch(FPath); //поток по файлу
		if (!in_ch.is_open()) {
			cout << "Файл с матрицей смежности не найден!!!" << endl;
			return;
		}
		int h_matr = 0;
		string str_ch;
		string substr_ch;//проверочные переменные
		getline(in_ch, str_ch);
		istringstream strstream_ch(str_ch); //поток по строке
		while (!strstream_ch.eof()) //цикл считывания 
		{
			strstream_ch >> substr_ch;
			if (!strstream_ch.eof())
				h_matr++;
		}
		string str;
		string substr;
		ifstream fin(FPath); //поток по файлу

		vector<vector<int>>* matrix = new vector<vector<int>>;
		(*matrix).resize(h_matr);
		for (int i = 0; i < h_matr; i++)//выделение памяти
		{
			(*matrix)[i].resize(h_matr);
		}

		for (int i = 0; i < h_matr; i++)
		{
			for (int j = 0; j < h_matr; j++)
			{
				fin >> (*matrix)[i][j];
			}
		}

		edgelist = list_of_edges(matrix);
	}
	
}


//*-------------- ф-ии парсинга ------------------*//

//существование ключа в строке (возвраащает (индекс + 1) или (0))
int exist_key(int argc, char* argv[], string key)
{
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == key)
			return i + 1;
	}
	return 0;
}
//есть только 1 ключ файла ввода
bool once_keyfile(int argc, char* argv[])
{
	bool ex_e = exist_key(argc, argv, "-e");
	bool ex_m = exist_key(argc, argv, "-m");
	bool ex_l = exist_key(argc, argv, "-l");
	if (ex_e && ex_m || ex_e && ex_l || ex_m && ex_l)
	{
		cout << "\nВведено несколько ключей файла ввода!!!\n" << endl;
		return false;
	}
	return true;
}
//справка
void help(int argc, char* argv[])
{
	
	cout << "Автор работы: Иващенко А.В.\n" << endl;
	cout << "Группа: M30-211б-21\n" << endl;
	cout << "Список ключей с параметрами: \n" << endl;
	cout << "Входные данные для работы программы задаются следующими ключами с параметрами:" << endl;
	cout << "-e \"edges_list_file_path\"" << endl;
	cout << "-l \"adjacency_list_file_path\"" << endl;
	cout << "-m \"adjacency_matrix_file_path\"\n" << endl;
	cout << "Одновременно может указываться только один из этих ключей!\n" << endl;
	cout << "Результаты работы выводятся на экран, либо в файл при указании следующего ключа: " << endl;
	cout << "-o \"output_file_path\"\n" << endl;	
	
	cout << "Дополнительные ключи для программы: \n" << endl;
	cout << "№4:" << endl;
	cout << "Алгоритм для расчёта задаётся следующими ключами: " << endl;
	cout << "-k – алгоритм Крускала" << endl;	
	cout << "-p – алгоритм Прима" << endl;
	cout << "-b – алгоритм Борувки" << endl;
	cout << "-s – расчёт производится тремя алгоритмами поочерёдно\n" << endl;
	cout << "№5:" << endl;
	cout << "Начальная вершина задаётся следующим ключом:" << endl;
	cout << "-n begin_vertex_number - Данный ключ является обязательным." << endl;
	cout << "Конечная вершина задаётся следующим ключом:" << endl;
	cout << "-d end_vertex_number - Данный ключ является обязательным.\n" << endl;
	cout << "№6:" << endl;
	cout << "Алгоритм для расчёта задаётся следующими ключами:" << endl;
	cout << "-d – алгоритм Дейкстры" << endl;
	cout << "-b – алгоритм Беллмана-Форда-Мура" << endl;
	cout << "-t – алгоритм Левита\n" << endl;
	cout << "Начальная вершина задаётся следующим ключом:" << endl;
	cout << "-n begin_vertex_number - Данный ключ является обязательным\n" << endl;
	cout << "№8:" << endl;
	cout << "Входной файл для работы программы задаётся следующим ключом:" << endl;
	cout << "-m \"map_file_path\"\n" << endl;
	cout << "Начальная вершина задаётся следующим ключом:" << endl;
	cout << "-n begin_vertex_x begin_vertex_y - Данный ключ является обязательным.\n" << endl;
	cout << "Конечная вершина задаётся следующим ключом:" << endl;
	cout << "-d end_vertex_x end_vertex_y - Данный ключ является обязательным.\n" << endl;
	cout << "№9:" << endl;
	cout << "Алгоритм для расчёта задаётся следующими ключами:" << endl;
	cout << "-b – метод ветвей и границ" << endl;
	cout << "-a – метод муравьиной колонии\n" << endl;
	cout << "Начальная вершина задаётся следующим ключом:" << endl;
	cout << "-n begin_vertex_number - Данный ключ является обязательным" << endl;
}
//номер программы
int num_programm(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (atoi(argv[i]) < 0)
			return abs(atoi(argv[i]));
	}
	return 0;
}
//создает файл, если он не существует, и возвращает его поток
ofstream create_file(string filename)
{
	ofstream file(filename);
	return file;
}

//*-------------- Ф-ии Вывода ------------------*//
//печать матрицы
void print_matrix(vector<vector<int>>* matrix, ostream& stream_out)
{
	int length = matrix->size();
	for (int i = 0; i < length; i++)//вывод матрицы после кратчайших расстояний
	{
		for (int j = 0; j < length; j++)
		{
			if ((*matrix)[i][j] != INF)
				stream_out << setfill(' ') << setw(3) << (*matrix)[i][j];
			else
				stream_out << setfill(' ') << setw(3) << "-";
		}
		stream_out << endl;
	}
}
//печать вектора
template<typename T>
void print_vector(vector<T>* vec, ostream& stream_out)
{
	int length = vec->size();
	stream_out << "[";
	for (int i = 0; i < length-1; i++)
	{
		stream_out << (*vec)[i]<<", ";
	}
	stream_out << (*vec)[length-1] << "]"<<endl;
}
//печать компонент связности
void print_comp(int length, vector<int>& used, int num_comp, ostream& stream_out,bool is_directed, bool strong_comp)
{
	
	bool connected = true;//флаг связности
	for (int i = 0; i < length - 1; i++)//цикл проверки на связность
	{
		if (used[i] != used[i + 1])
			connected = false;
	}
	if (!is_directed) {
		if (connected)
			stream_out << "Граф связен ";
		else
			stream_out << "Граф не связен ";
	}
	else
	{
		if (connected && !strong_comp)
			stream_out << "Орграф связен ";
		else if(!strong_comp)
			stream_out << "Орграф не связен ";
		else if(connected)
			stream_out << "Орграф сильносвязен ";
		else 
			stream_out << "Орграф слабосвязен ";
	}
	if(!strong_comp)
		stream_out << "и имеет " << num_comp << " компонент связности:" << endl;
	else 
		stream_out << "и имеет " << num_comp << " компонент сильной связности:" << endl;
	stream_out << "[";
	for (int i = 0; i < num_comp; i++)//вывод компонент
	{
		if (i != 0)
			stream_out << ", ";
		else
			stream_out << endl << "  ";
		vector<int> comp;//вектор с вершинами i компоненты
		for (int j = 0; j < length; j++)
		{
			if (used[j] == i + 1)
			{
				comp.push_back(j + 1);
			}
		}
		print_vector(&comp, stream_out);
	}
	stream_out << "]" << endl;
	
}
//печать остовного дерева
void print_spanningtree(list<int*>* spanning_tree, ostream& stream_out, int cost)
{
	//печать остова и его веса
	stream_out << "Минимальное остовное дерево: " << endl << "[";
	for (list<int*>* current = spanning_tree; current; current = current->next)
	{
		stream_out << "(" << current->Ver[0] << ", " << current->Ver[1] << ", " << current->Ver[2] << ")";
		if (current->next)
			stream_out << ", ";
	}
	stream_out << "]" << endl << "Вес миимального остова: " << cost << endl;
	
}
//*-------------- Ф-ии Программ ------------------*//

void first_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out)
{
	
	vector<vector<int>>* matrix = GRAPH.adjacency_matrix();//матрица смежности
	int length = matrix->size();//размир матрицы
	int count;//счетчик
	bool is_directed = GRAPH.is_directed();//флаг ориентированности
	vector<int>* deg_in = new vector<int>;//вектор степеней входа вершин
	deg_in->resize(length);
	//вывод степеней вершин
	if (is_directed)//если граф ориентированный
	{
		
		stream_out << "deg+ = ";
		for (int j = 0; j < length; j++)//цикл нахождения степеней входа (кроме последней вершины)
		{
			count = 0;
			for (int i = 0; i < length; i++)
			{
				if ((*matrix)[i][j])
				{
					count++;
				}
			}
			(*deg_in)[j] = count;
		}
		print_vector(deg_in, stream_out);
	}
	if (is_directed)
		stream_out << "deg- = ";
	else
		stream_out << "deg = ";
	vector<int>* deg_out = new vector<int>;//вектор степеней входа вершин
	deg_out->resize(length);
	for (int i = 0; i < length; i++)//цикл нахождения степеней выхода (кроме последней вершины)
	{
		count = 0;
		for (int j = 0; j < length; j++)
		{
			if ((*matrix)[i][j])
			{
				count++;
			}
		}
		(*deg_out)[i] = count;
	}
	print_vector(deg_out, stream_out);
	
	//создаем матрицу расстояний
	Floyd_Warshall(matrix);

	stream_out << endl;
	print_matrix(matrix, stream_out);
	
	//поиск эксцентриситетов
	int max;
	bool inf = false;
	vector<int>* exc = new vector<int>;//вектор эксцентриситетов
	exc->resize(length);
	for (int i = 0; i < length; i++)
	{
		max = 0;
		for (int j = 0; j < length; j++)
		{
			if ((*matrix)[i][j] > max)
			{
				max = (*matrix)[i][j];
				if (max == INF)//если есть бесконечность то выходим
				{
					inf = true;
					break;
				}

			}
		}
		(*exc)[i] = max;
		if (inf)	
			break;

	}
	if (is_directed)//выход из программы если граф ориентированный
		return;
	if (!inf)//выводим эксцентриситеты, если они существуют
	{
		stream_out << "Eccentricity:" << endl;
		print_vector(exc, stream_out);


		//нахождение диаметра
		max = 0;
		for (int i = 0; i < length; i++)
		{
			if ((*exc)[i] > max && (*exc)[i] != 0 && (*exc)[i] != INF)
				max = (*exc)[i];
		}
		stream_out << "D = " << max << endl;
		//нахождение радиуса
		int min = INF;
		for (int i = 0; i < length; i++)
		{
			if ((*exc)[i] < min && (*exc)[i] != 0 && (*exc)[i] != INF)
				min = (*exc)[i];
		}
		stream_out << "R = " << min << endl;
		//нахождение центральных вершин
		vector<int>* Z = new vector<int>;
		stream_out << "Z = ";
		for (int i = 0; i < length; i++)
		{
			if ((*exc)[i] == min)
				Z->push_back(i + 1);
		}
		print_vector(Z, stream_out);
		//нахождение переферийных вершин
		vector<int>* P = new vector<int>;
		stream_out << "P = ";
		for (int i = 0; i < length; i++)
		{
			if ((*exc)[i] == max)
				P->push_back(i + 1);
		}
		print_vector(P, stream_out);
	}
}
void second_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out)
{
	list<list<int>*>* adjlist = GRAPH.adjacency_list();//список смежности
	int length = adjlist->length();//длина списка(количество вершин)
	int num_comp = 1;//номер компоненты (или их количество)
	vector<int> used(length);//вектор с маркированными вершинами(марка - номер компоненты вершины)
	
	if (!GRAPH.is_directed())//если граф неориентированный
	{
		for (int i = 1; i <= length; i++)//цикл поиска в ширину для каждой непосещенной вершины
		{
			if (!used[i - 1])
			{
				BFS(GRAPH, &used, i, num_comp);//поиск в ширину
				num_comp++;//инкремент номера компоненты
			}

		}

		print_comp(length, used, num_comp - 1, stream_out, false, false);
	}
	else//если граф ориентированный
	{
		vector<vector<int>>* matrix = GRAPH.adjacency_matrix();//матрица смежности графа
		int length = matrix->size();//количество вершин
		//делаем соотнесенный граф
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < length; j++)
			{
				if ((*matrix)[i][j])
					(*matrix)[j][i] = 1;
			}
		}
		Graph GraphCor(matrix);//соотнесенный граф
		//цикл поиска в ширину для каждой непосещенной вершины
		for (int i = 1; i <= length; i++)
		{
			if (!used[i - 1])
			{
				BFS(GraphCor, &used, i, num_comp);//поиск в ширину
				num_comp++;//инкремент номера компоненты
			}

		}
		//печать компонент слабой связности
		print_comp(length, used, num_comp - 1, stream_out, true, false);
		
		//инвертируем граф
		list<int[3]>* edgelist = GRAPH.list_of_edges();
		list<int[3]>* current = edgelist;
		for (current; current; current = current->next)
		{
			swap(current->Ver[0], current->Ver[1]);
		}
		//обнуляем список маркеров
		for (int i = 0; i < length; i++)
			used[i] = 0;
		
		Graph Graph_Inv(edgelist);//инвертированный граф
		vector<int>* order = TopologicalSort(Graph_Inv);//вектор топологической сортировки инвертированного графа
		num_comp = 1;
		for (int i = 0; i < order->size(); i++)//цикл DFS по вершинам обычного графа в порядке топологической сортировки инвертированного
		{
			if (!used[(*order)[i]-1]) 
			{
				DFS(GRAPH, &used, (*order)[i], num_comp, 0);
				num_comp++;
			}
		}
		print_comp(length, used, num_comp - 1, stream_out, true, true);
	}
	
}
void third_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out) 
{
	if (GRAPH.is_directed())//если граф ориентированный создаем соотнесенный граф
	{
		vector<vector<int>>* matrix = GRAPH.adjacency_matrix();//матрица смежности графа
		int length = matrix->size();//количество вершин
		//делаем соотнесенный граф
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < length; j++)
			{
				if ((*matrix)[i][j])
					(*matrix)[j][i] = 1;
			}
		}
		Graph GraphCor(matrix);
		GRAPH = GraphCor;

	}
	list<list<int>*>* adjlist = GRAPH.adjacency_list();//список смежности
	int length = adjlist->length();//длина списка(количество вершин)
	vector<bool> used(length);//вектор маркеров
	vector<int> tin(length);//векторов маркеров времени входа в врешину
	vector<int> tup(length);//вектор маркеров выхода из вершины с учетом обратных ребер
	list<int*> * bridges = new list<int*>;//список мостов
	bridges->Ver = NULL;
	int timer = 0;//таймер уровня обхода дерева DFS
	//поиск мостов с помощью поиска в глубину
	for (int i = 0; i < length; i++)//поиск мостов с помощью поиска в глубину
	{
		if (!used[i])
			DFS_BRIDGES(GRAPH, &used, i + 1, timer, &tin, &tup, bridges);
	}
	//вывод мостов
	stream_out << "Мосты в графе:" << endl<<"[";
	list<int*>* current = bridges;
	if (current->Ver) {
		for (current; current; current = current->next)
		{
			stream_out << "(" << current->Ver[0] << ", " << current->Ver[1] << ")";
			if (current->next)
				stream_out << ", ";
		}
	}
	stream_out << "]"<<endl;
	//поиск шарниров с помощью поиска в глубину
	timer = 0;
	vector<bool> is_cut(length);//вектор маркеров на то является ли i вершина шарниром
	//обнуляем списки маркеров
	for (int i = 0; i < length; i++) 
	{
		tin[i] = 0;
		tup[i] = 0;
		used[i] = false;
	}
	for (int i = 0; i < length; i++)//поиск шарниров с помощью поиска в глубину
	{
		if (!used[i])
			DFS_CUTVERTEXES(GRAPH, &used, &is_cut, i+1, timer, &tin, &tup);
	}
	//вывод шарниров в графе
	stream_out << "Шарниры в графе:" << endl;
	vector<int> cut_vertexes;
	for (int i = 0; i < length; i++)
	{
		if (is_cut[i]) 
		{
			cut_vertexes.push_back(i + 1);
		}
	}
	if (cut_vertexes.size() != 0)
		print_vector(&cut_vertexes, stream_out);
	else
		stream_out << "[]" << endl;
}
void fourth_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out)
{
	int num_alg = 0;
	if (exist_key(argc, argv, "-k"))
		num_alg = 1;
	if (exist_key(argc, argv, "-p"))
		num_alg = 2;
	if (exist_key(argc, argv, "-b"))
		num_alg = 3;
	if (exist_key(argc, argv, "-s"))
		num_alg = 4;
	if (!num_alg)
	{
		cout << "Не введен ключ алгоритма!!!" << endl;
		return;
	}
	if (GRAPH.is_directed())//если граф ориентированный создаем соотнесенный граф
	{
		vector<vector<int>>* matrix = GRAPH.adjacency_matrix();//матрица смежности графа
		int length = matrix->size();//количество вершин
		//делаем соотнесенный граф
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < length; j++)
			{
				if ((*matrix)[i][j])
					(*matrix)[j][i] = (*matrix)[i][j];
			}
		}
		Graph GraphCor(matrix);
		GRAPH = GraphCor;
	}
	switch (num_alg)
	{
	case 1://алгоритм крускала
	{
		list<int*>* spanning_tree = new list<int*>;//ребра остовного дерева
		//алгоритм крускала
		int cost = Kruscal(GRAPH, spanning_tree);
		//печать остова и его веса
		print_spanningtree(spanning_tree, stream_out, cost);
		break; 
	}
	case 2://алгоритм прима
	{
		list<int*>* spanning_tree = new list<int*>;//ребра остовного дерева
		int cost = Prim(GRAPH, spanning_tree);
		//печать остова и его веса
		print_spanningtree(spanning_tree, stream_out, cost);
		break;
	}
	case 3://алгоритм ,борувки
	{
		list<int*>* spanning_tree = new list<int*>;//ребра остовного дерева
		int cost = Boruvka(GRAPH, spanning_tree);
		//печать остова и его веса
		print_spanningtree(spanning_tree, stream_out, cost);
		break;
		
	}
	case 4:
	{
		list<int*>* spanning_tree = new list<int*>;//ребра остовного дерева
		stream_out << "/*--------------Алгоритм Крускала--------------*/" << endl;
		auto begin = chrono::steady_clock::now();//старт счётчика тактов
		int cost = Kruscal(GRAPH, spanning_tree);
		auto end = chrono::steady_clock::now();//стоп счётчика тактов
		auto elapsed_micros = chrono::duration_cast<chrono::microseconds>(end - begin);//получаем время работы
		print_spanningtree(spanning_tree, stream_out, cost);
		stream_out << "Время Работы (мкс): " << elapsed_micros.count() << endl;

		spanning_tree = new list<int*>;//ребра остовного дерева
		stream_out << "/*--------------Алгоритм Прима--------------*/" << endl;
		begin = chrono::steady_clock::now();//старт счётчика тактов
		cost = Prim(GRAPH, spanning_tree);
		end = chrono::steady_clock::now();//стоп счётчика тактов
		elapsed_micros = chrono::duration_cast<chrono::microseconds>(end - begin);//получаем время работы
		print_spanningtree(spanning_tree, stream_out, cost);
		stream_out << "Время Работы (мкс): " << elapsed_micros.count() << endl;

		spanning_tree = new list<int*>;//ребра остовного дерева
		stream_out << "/*--------------Алгоритм Борувки--------------*/" << endl;
		begin = chrono::steady_clock::now();//старт счётчика тактов
		cost = Boruvka(GRAPH, spanning_tree);
		end = chrono::steady_clock::now();//стоп счётчика тактов
		elapsed_micros = chrono::duration_cast<chrono::microseconds>(end - begin);//получаем время работы
		print_spanningtree(spanning_tree, stream_out, cost);
		stream_out << "Время Работы (мкс): " << elapsed_micros.count() << endl;
	}
	}
	


}
void fifth_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out)
{
	if (!(exist_key(argc, argv, "-n") && exist_key(argc, argv, "-d")))
	{
		cout << "Не введены ключи начальной и конечной вершин!!!" << endl;
		return;
	}
	int begin_Ver = stoi(argv[exist_key(argc, argv, "-n")]);//начальная вершина
	int end_Ver = stoi(argv[exist_key(argc, argv, "-d")]);//конечная вершина
	vector<int> answ;//массив расстояний
	vector<int> prev;//массив восстановления пути
	//дейкстра
	int dst = Dijkstra(GRAPH, answ, prev, begin_Ver, end_Ver);
	if (dst == INF)//если пути нет
	{
		stream_out << "Нет пути между " << begin_Ver<<" и " << end_Ver << "." << endl;
		return;
	}
	vector<vector<int>>* matrix = GRAPH.adjacency_matrix();//матрицы смежности для данного графа
	
	int curPrev = end_Ver - 1;//текущая вершина в массиве пути
	vector<int*>* way = new vector<int*>;//вектор пути
	while (curPrev != begin_Ver - 1)//заполняем вектор пути
	{
		int* pushed_edge = new int[3] {prev[curPrev] + 1, curPrev + 1, (*matrix)[prev[curPrev]][curPrev]};
		way->push_back(pushed_edge);
		curPrev = prev[curPrev];
	}
	reverse(way->begin(), way->end());//переворачиваем путь

	//вывод кратчайшего пути и его расстояния
	stream_out << "Путь между " << begin_Ver << " и " << end_Ver << ": " <<dst<< endl<<"[";
	int length = way->size();
	for (int i = 0; i < length; i++)
	{
		stream_out << "(" << (*way)[i][0] << ", " << (*way)[i][1] << ", " << (*way)[i][2] << ")";
		if (i != length - 1)
			stream_out << ", ";
	}
	stream_out << "]" << endl;

}
void sixth_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out)
{
	int num_alg = 0;
	if (exist_key(argc, argv, "-d"))
		num_alg = 1;
	if (exist_key(argc, argv, "-b"))
		num_alg = 2;
	if (exist_key(argc, argv, "-t"))
		num_alg = 3;
	if (!num_alg)
	{
		cout << "Не введен ключ алгоритма!!!" << endl;
		return;
	}
	if (!(exist_key(argc, argv, "-n")))
	{
		cout << "Не введен ключ начальной вершины!!!" << endl;
		return;
	}
	int begin_Ver = stoi(argv[exist_key(argc, argv, "-n")]);//начальная вершина
	vector<int> answ;//массив расстояний
	vector<int> prev;//массив восстановления пути(в данной программе не нужен, только для работы дейкстры)
	switch (num_alg)
	{
	case 1://алгоритм дейкстры
	{
		//запускаем модификацию форда для дейкстры
		if (Dijkstra_Ford(GRAPH, answ, prev, begin_Ver) == -1)//алгоритм дейкстры, если -1, то есть отр. цикл
		{
			stream_out << "В графе есть отрицательный цикл!!!" << endl;
			return;
		}
		bool negedge_exist = false;//флаг существования ребер отрицательного веса\
				//проверка на ребра отрицательного веса
		for (list<int[3]>* cur = GRAPH.list_of_edges(); cur; cur = cur->next)
		{
			if (cur->Ver[2] < 0)
				negedge_exist = true;
		}
		if (!negedge_exist)
			stream_out << "Граф не имеет ребер отрицательного веса." << endl;
		else
			stream_out << "Граф имеет ребра отрицательного веса." << endl;
		stream_out << "Длины кратчайших путей:" << endl;
		int length = answ.size();//количество вершин
		//вывод расстояний
		for (int i = 0; i < length; i++)
		{
			if (i + 1 != begin_Ver)
			{
				if (answ[i] != INF)
					stream_out << begin_Ver << " - " << i + 1 << ": " << answ[i] << endl;
				else
					stream_out << begin_Ver << " - " << i + 1 << ": INF " << endl;
			}
		}
		break; 
	}
	case 2://алгоритм беллмана форда
	{
		//запускаем алгоритм Беллмана-Форда
		if (Bellman_Ford(GRAPH, answ, begin_Ver) == -1)//алгоритм дейкстры, если -1, то есть отр. цикл
		{
			stream_out << "В графе есть отрицательный цикл!!!" << endl;
			return;
		}
		bool negedge_exist = false;//флаг существования ребер отрицательного веса\
						//проверка на ребра отрицательного веса
		for (list<int[3]>* cur = GRAPH.list_of_edges(); cur; cur = cur->next)
		{
			if (cur->Ver[2] < 0)
				negedge_exist = true;
		}
		if (!negedge_exist)
			stream_out << "Граф не имеет ребер отрицательного веса." << endl;
		else
			stream_out << "Граф имеет ребра отрицательного веса." << endl;
		stream_out << "Длины кратчайших путей:" << endl;
		int length = answ.size();//количество вершин
		//вывод расстояний
		for (int i = 0; i < length; i++)
		{
			if (i + 1 != begin_Ver)
			{
				if (answ[i] != INF)
					stream_out << begin_Ver << " - " << i + 1 << ": " << answ[i] << endl;
				else
					stream_out << begin_Ver << " - " << i + 1 << ": INF " << endl;
			}
		}
		break;
	}
	case 3://алгоритм левита
	{
		//запускаем алгоритм Беллмана-Форда
		if (Bellman_Ford(GRAPH, answ, begin_Ver) == -1)//алгоритм дейкстры, если -1, то есть отр. цикл
		{
			stream_out << "В графе есть отрицательный цикл!!!" << endl;
			return;
		}
		answ.clear();

		//если нет отриц цикла запускаем левита
		Levit(GRAPH, answ, begin_Ver);
		bool negedge_exist = false;//флаг существования ребер отрицательного веса\
								//проверка на ребра отрицательного веса
		for (list<int[3]>* cur = GRAPH.list_of_edges(); cur; cur = cur->next)
		{
			if (cur->Ver[2] < 0)
				negedge_exist = true;
		}
		if (!negedge_exist)
			stream_out << "Граф не имеет ребер отрицательного веса." << endl;
		else
			stream_out << "Граф имеет ребра отрицательного веса." << endl;
		stream_out << "Длины кратчайших путей:" << endl;
		int length = answ.size();//количество вершин
		//вывод расстояний
		for (int i = 0; i < length; i++)
		{
			if (i + 1 != begin_Ver)
			{
				if (answ[i] != INF)
					stream_out << begin_Ver << " - " << i + 1 << ": " << answ[i] << endl;
				else
					stream_out << begin_Ver << " - " << i + 1 << ": INF " << endl;
			}
		}
		break;
	}
	}

}
void seventh_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out)
{
	vector<vector<int>> answ;
	if (Jonson(GRAPH, answ) == -1)
	{
		stream_out << "В графе есть отрицательный цикл!!!" << endl;
		return;
	}
	bool negedge_exist = false;//флаг существования ребер отрицательного веса\
							//проверка на ребра отрицательного веса
	for (list<int[3]>* cur = GRAPH.list_of_edges(); cur; cur = cur->next)
	{
		if (cur->Ver[2] < 0)
			negedge_exist = true;
	}
	if (!negedge_exist)
		stream_out << "Граф не имеет ребер отрицательного веса." << endl;
	else
		stream_out << "Граф имеет ребра отрицательного веса." << endl;
	stream_out << "Длины кратчайших путей:" << endl;
	int length = answ.size();
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length; j++)
		{
			if (i == j || answ[i][j] == INF)
				continue;
			else
				stream_out << i + 1 << " - " << j + 1 << ": " << answ[i][j] << endl;
			
		}
	}

}
void eighth_task(int argc, char* argv[], Map MAP, ostream& stream_out)
{
	if (!(exist_key(argc, argv, "-n")))
	{
		cout << "Не введен ключ начальной вершины!!!" << endl;
		return;
	}
	if (!(exist_key(argc, argv, "-d")))
	{
		cout << "Не введен ключ конечной вершины!!!" << endl;
		return;
	}
	list<Cell>* way;
	int length = AStar(MAP, way, Cell(stoi(argv[exist_key(argc, argv, "-n")]), stoi(argv[exist_key(argc, argv, "-n") + 1])),
		Cell(stoi(argv[exist_key(argc, argv, "-d")]), stoi(argv[exist_key(argc, argv, "-d") + 1])), &Manhattan);
	cout << "dsds";
}

//*-------------- Алгоритмы ------------------*//
//алгоритм флойда
vector<vector<int>>* Floyd_Warshall(vector<vector<int>>* matrix)
{
	int length = matrix->size();
	for (int i = 0; i < length; i++)//заполнение бесконечностями недостижимых вершин
	{
		for (int j = 0; j < length; j++)
		{
			if ((*matrix)[i][j] == 0 && i!=j)
				(*matrix)[i][j] = INF;
		}
	}
	for (int k = 0; k < length; k++)//алгоритм флойда
	{
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < length; j++)
			{
				(*matrix)[i][j] = min((*matrix)[i][j], (*matrix)[i][k] + (*matrix)[k][j]);
			}
		}
	}
	return matrix;

}
//поиск в ширину
void BFS(Graph GRAPH, vector<int>* used, int Ver, int mark)
{
	queue<int> q;//очередь вершин
	q.push(Ver);//вставляем в очередь начальную вершину
	(*used)[Ver - 1] = mark;//маркируем начальную вершину
	while (!q.empty())
	{
		int v = q.front();//доставем вершину
		q.pop();
		list<int>* current = GRAPH.adjacency_list(v);//итератор на смежные вершины v
		for (current; current; current = current->next)//цикл по всем немаркированным смежным вершинам v
		{
			if (!(*used)[current->Ver - 1])
			{
				q.push(current->Ver);//добавляем в очередь
				(*used)[current->Ver - 1] = mark;//маркируем
			}
		}

	}
}
//топологическая сортировка графа
vector<int>* TopologicalSort(Graph GRAPH)
{
	int length = GRAPH.adjacency_list()->length();
	vector<int> used(length);
	vector<int>* sortedgraph = new vector<int>;
	for (int i = 0; i < length; i++)
	{
		if (!used[i])
		{
			DFS(GRAPH, &used, i+1, 1, sortedgraph);
		}
			
	}
	reverse(sortedgraph->begin(), sortedgraph->end());
	return sortedgraph;
}
//поиск в глубину
//для топологической сортировки
void DFS(Graph GRAPH, vector<int>* used, int Ver, int mark, vector<int>* order)
{
	stack<int> s;
	s.push(Ver);
	(*used)[Ver-1] = mark;
	while (!s.empty())
	{
		int v = s.top();
		s.pop();
		list<int>* current = GRAPH.adjacency_list(v);
		for (current; current; current = current->next)//цикл по всем немаркированным смежным вершинам v
		{
			if (!(*used)[current->Ver - 1])
			{
				s.push(v);
				(*used)[current->Ver - 1] = mark;
				s.push(current->Ver);	
				break;
			}
		}
		if(!current && order)
			order->push_back(v);
	}
}
//для поиска мостов
void DFS_BRIDGES(Graph GRAPH, vector<bool>* used, int Ver, int timer, vector<int>* tin, vector<int>* tup, list<int*>* bridges, int back)
{   
	int length = used->size();
	(*used)[Ver - 1] = true;
	(*tin)[Ver - 1] = timer;
	(*tup)[Ver - 1] = timer;//при входе в врешину tup = tin
	list<int>* current = GRAPH.adjacency_list(Ver);
	for (current; current; current = current->next)
	{
		int next = current->Ver;
		if (next == back)
			continue;
		if ((*used)[next - 1])//при нахождении обратного ребра
			(*tup)[Ver - 1] = min((*tup)[Ver - 1], (*tin)[next - 1]);
		else
		{
			DFS_BRIDGES(GRAPH, used, next, timer + 1, tin, tup, bridges, Ver);//поиск в глубину для следующей вершины
			(*tup)[Ver - 1] = min((*tup)[Ver - 1], (*tup)[next - 1]);//возврат в вершину
			if ((*tup)[next - 1] > (*tin)[Ver - 1])
			{
				if (bridges->Ver)//добавление моста в список
				{
					int* bridge = new int[2] { Ver, next };
					bridges->add(bridge);
				}
				else//добавление первого моста
				{
					int* bridge = new int[2] { Ver, next };
					bridges->Ver = bridge;
				}
			}
		}
	}

}
//для поиска шарниров
void DFS_CUTVERTEXES(Graph GRAPH, vector<bool>* used, vector<bool>* is_cut, int Ver, int timer, vector<int>* tin, vector<int>* tup, int back)
{
	int length = used->size();
	(*used)[Ver - 1] = true;
	(*tin)[Ver - 1] = timer;
	(*tup)[Ver - 1] = timer;
	list<int>* current = GRAPH.adjacency_list(Ver);
	int children = 0;
	for (current; current; current = current->next)
	{
		int next = current->Ver;
		if (next == back)
			continue;
		if ((*used)[next - 1])//при нахождении обратного ребра
			(*tup)[Ver - 1] = min((*tup)[Ver - 1], (*tin)[next - 1]);
		else
		{
			DFS_CUTVERTEXES(GRAPH, used, is_cut, next, timer + 1, tin, tup, Ver);//поиск в глубину для следующей вершины
			(*tup)[Ver - 1] = min((*tup)[Ver - 1], (*tup)[next - 1]);//возврат в вершину
			if ((*tup)[next - 1] >= (*tin)[Ver - 1] && back != 0)
			{
				(*is_cut)[Ver - 1] = true;
			}
			children++;
		}
	}
	if (back == 0 && children > 1)//если начальная имеет более одного потомка
	{
		(*is_cut)[Ver - 1] = true;
	}
}
//алгоритм крускала
int Kruscal(Graph GRAPH, list<int*>* spanning_tree)
{
	list<int[3]>* edgelist = GRAPH.list_of_edges();//список ребер
	spanning_tree->Ver = 0;
	int length = edgelist->length(edgelist);
	//сортировка ребер
	edgelist->quickSort(&edgelist, 2);

	vector<int> tree_id(length);//вектор принадлежности вершины некоторой компоненте
	int cost = 0;//вес остова
	//изначально каждая вершина принадлежит отдельной компоненте
	for (int i = 0; i < length; i++)
	{
		tree_id[i] = i;
	}
	//обход по ребрам (нахождение остова)
	for (list<int[3]>* current = edgelist; current; current = current->next)
	{
		int a = current->Ver[0] - 1;//1 вершина
		int b = current->Ver[1] - 1;//2 вершина
		int l = current->Ver[2];//вес ребра
		if (tree_id[a] != tree_id[b])//вершины из разных компонент
		{
			cost += l;//увеличение веса остова
			if (spanning_tree->Ver)//добавление ребра в остов
			{
				int* edge = new int[3] { a + 1, b + 1, l };
				spanning_tree->add(edge);
			}
			else//добавление первого ребра в остов
			{
				int* edge = new int[3] { a + 1, b + 1, l };
				spanning_tree->Ver = edge;
			}
			int old_id = tree_id[b];
			int new_id = tree_id[a];
			for (int j = 0; j < length; j++)//объединение компонент
			{
				if (tree_id[j] == old_id)
					tree_id[j] = new_id;
			}
		}
	}
	return cost;
}
//алгоритм прима
int Prim(Graph GRAPH, list<int*>*& spanning_tree)
{
	list<int[3]>* edgelist = GRAPH.list_of_edges();//список ребер
	spanning_tree->Ver = 0;
	int length = edgelist->length(edgelist);//количество вершин
	int mst_weight = 0;//текущий вес остова
	priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> q;//приоитетная очередь ребер смежных текущему остову
	//q.first - вес ребра
	//q.second.first - 1 вершина ребра
	//q.second.second - 2 вершина ребра
	vector<bool> used(length);//вектор маркеров посещенности вершин
	q.push({ 0, {0, 0 } });//начинаем с 1 вершины
	while (!q.empty())
	{
		pair<int, pair<int, int>> c = q.top();//винимаем из очереди ребро с минимальным весом
		q.pop();

		int dst = c.first;//вес нового ребра в остове
		int v = c.second.second;//вершина 

		if (used[v]) //вершина уже добавлена в остов
		{      
			continue;
		}

		used[v] = true;//вершина v в остове
		mst_weight += dst;//увеличиваем вес остова

		if (spanning_tree->Ver)//добавление ребра в остов
		{
			int* edge = new int[3] { c.second.first+1, v+1, dst };
			spanning_tree->add(edge);
		}
		else//добавление первого ребра в остов
		{
			int* edge = new int[3] { c.second.first + 1, v + 1, dst};
			spanning_tree->Ver = edge;
		}

		list<int[3]>* edges = GRAPH.list_of_edges(v + 1);
		for (list<int[3]>* cur = edges; cur; cur = cur->next)//добавление в очередь новых ребер смежных с новой вершиной в остове V
		{
			pair<int, int> e;
			int u = cur->Ver[1] - 1;
			int len_vu = cur->Ver[2];

			if (!used[u])
			{
				q.push({ len_vu, {v,u} });//добавление в очередь нового ребра
			}
		}
		
	}
	//удаление лишней 1 вершины из списка ребер остова
	list<int*>* next = spanning_tree->next;
	delete spanning_tree;
	spanning_tree = next;
	return mst_weight;
}
//алгоритм борувки
int Boruvka(Graph GRAPH, list<int*>*& spanning_tree)
{
	spanning_tree->Ver = 0;
	list<int[3]>* edgelist = GRAPH.list_of_edges();
	int length = edgelist->length(edgelist);
	vector<int> parent(length);

	// Массив для хранения индекса самого дешевого ребра
	// подмножества. Он хранит [u,v,w] для каждого компонента
	vector<int> rank(length);
	vector<vector<int>> cheapest(length, vector<int>(3, -1));

	// Изначально существует V разных деревьев.
	// Наконец, останется одно дерево, которое будет MST
	int numTrees = length;
	int MSTweight = 0;//вес минимального остова

	// Создать V подмножеств с отдельными элементамиts
	for (int node = 0; node < length; node++) 
	{
		parent[node] = node;
		rank[node] = 0;
	}

	// Продолжайте комбинировать компоненты (или наборы) до тех пор, пока все
	// компоненты не будут объединены в один MST
	while (numTrees > 1) {

		// Пройдитесь по всем ребрам и обновите
		// самый дешевый из всех компонентов

		for (list<int[3]>* cur = edgelist;cur;cur = cur->next)
		{

			// Найти компоненты (или наборы) двух вершин
			// текущего ребра
			int u = cur->Ver[0]-1; 
			int v = cur->Ver[1]-1;
			int	w = cur->Ver[2];
			int set1 = find(parent, u),
				set2 = find(parent, v);

			// Если две вершины текущего ребра принадлежат
			// тому же набору, игнорируйте текущее ребро. Еще раз проверьте
			// если текущее ребро ближе к предыдущим
			// самые дешевым ребрам set1 и set2
			if (set1 != set2) {
				if (cheapest[set1][2] == -1 || cheapest[set1][2] > w) 
				{
					cheapest[set1] = { u, v, w };
				}
				if (cheapest[set2][2] == -1 || cheapest[set2][2] > w) 
				{
					cheapest[set2] = { u, v, w };
				}
			}
		}

		// Рассмотрим выбранные выше самые дешевые ребра и
		// добавьте их в MST
		for (int node = 0; node < length; node++) {

			// Проверьте, существует ли самый дешевый вариант для текущего набора(set)
			if (cheapest[node][2] != -1) {
				int u = cheapest[node][0],
					v = cheapest[node][1],
					w = cheapest[node][2];
				int set1 = find(parent, u),
					set2 = find(parent, v);
				if (set1 != set2) 
				{
					MSTweight += w;
					unionSet(parent, rank, set1, set2);
					if (spanning_tree->Ver)//добавление ребра в остов
					{
						int* edge = new int[3] { u+1, v+1, w };
						spanning_tree->add(edge);
					}
					else//добавление первого ребра в остов
					{
						int* edge = new int[3] { u+1, v+1, w};
						spanning_tree->Ver = edge;
					}
					numTrees--;
				}
			}
		}
		for (int node = 0; node < length; node++) {

			// сброс самого дешевого массива
			cheapest[node][2] = -1;
		}
	}
	return MSTweight;
}
// Вспомогательная функция для поиска set элемента i
// (используется метод сжатия пути)
int find(vector<int>& parent, int i)
{
	if (parent[i] == i) {
		return i;
	}
	return find(parent, parent[i]);
}
// Функция, которая выполняет объединение двух set x и y
// (использует объединение по рангу)
void unionSet(vector<int>& parent, vector<int>& rank, int x, int y)
{
	int xroot = find(parent, x);
	int yroot = find(parent, y);

	// Присоедините дерево меньшего ранга к корню дерева высокого ранга
	// дерево (объединение по рангу)
	if (rank[xroot] < rank[yroot]) {
		parent[xroot] = yroot;
	}
	else if (rank[xroot] > rank[yroot]) {
		parent[yroot] = xroot;
	}
	// Если ранги одинаковы, то сделайте единицу корневой и
	// увеличьте ее ранг на единицу
	else {
		parent[yroot] = xroot;
		rank[xroot]++;
	}
}
//алгоритм Дейкстры
int Dijkstra(Graph GRAPH, vector<int>& answ, vector<int>& prev, int begin_Ver, int end_Ver)
{
	list<int[3]>* edgelist = GRAPH.list_of_edges();//список ребер
	int length = edgelist->length(edgelist);//количество вершин
	prev.resize(length);
	answ.resize(length);
	vector<int> colored(length);
	vector<bool> count_mark(length);//вектора количества обработки вершины (если = length у какойто вершины, то существует отр. цикл)
	for (int i = 0; i < length; i++)//заполняем массивы
	{
		answ[i] = INF;
		prev[i] = -1;
	}
	answ[begin_Ver - 1] = 0;//начальная вершина 0
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q; //приоритетная очередь с парой: вершина и расстояние до нее

	q.push({ 0, begin_Ver-1 });
	while (!q.empty())
	{
		pair<int, int> c = q.top();
		q.pop();
		int dst = c.first;//расстояние до рассматриваемой минимальной вершины
		int v = c.second;//рассматриваемая минимальная вершина
		count_mark[v] = count_mark[v]+1;//итерируем маркер обработки
		if (count_mark[v] >= length) //если существует отр. цикл возвращаем -1
		{
			return -1;
		}
		if (answ[v] != dst) //пропускаем повтор
		{
			continue;

		}
		//цикл по всем смежным с v вершинами
		for (list<int[3]>* cur = GRAPH.list_of_edges(v+1); cur; cur = cur->next)
		{
			int u = cur->Ver[1] - 1;//смежная с v вершина
			int len_vu = cur->Ver[2];//вес ребра (v,u)

			int n_dst = dst + len_vu;//новое расстояние
			if (n_dst < answ[u]) //ослабление пути
			{
				answ[u] = n_dst;
				prev[u] = v;//изменяем путь
				q.push({ n_dst, u });//добавляем ребро с измененным растоянием в очередь
			}
			
		}
	}
	return answ[end_Ver - 1];
}
//модификация форда для работы дейкстры с отрицательными весами
int Dijkstra_Ford(Graph GRAPH, vector<int>& answ, vector<int>& prev, int begin_Ver, int end_Ver)
{
	list<int[3]>* edgelist = GRAPH.list_of_edges();//список ребер
	int length = edgelist->length(edgelist);//количество вершин
	prev.resize(length);
	answ.resize(length);
	vector<bool> colored(length);
	vector<int> count_mark(length);//вектора количества обработки вершины (если = length у какойто вершины, то существует отр. цикл)
	for (int i = 0; i < length; i++)//заполняем массивы
	{
		answ[i] = INF;
		prev[i] = -1;
	}
	answ[begin_Ver - 1] = 0;//начальная вершина 0
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q; //приоритетная очередь с парой: вершина и расстояние до нее

	q.push({ 0, begin_Ver - 1 });
	while (!q.empty())
	{
		pair<int, int> c = q.top();
		q.pop();
		int dst = c.first;//расстояние до рассматриваемой минимальной вершины
		int v = c.second;//рассматриваемая минимальная вершина
		colored[v] = true;//масиив меток окрашивания
		count_mark[v] = count_mark[v] + 1;//итерируем маркер обработки
		if (count_mark[v] >= length) //если существует отр. цикл возвращаем -1
		{
			return -1;
		}
		if (answ[v] != dst) //пропускаем повтор
		{
			continue;

		}
		//цикл по всем смежным с v вершинами
		int count_changes = 0;//счётчик изменений расстояния (нашлось новое поменьше)
		for (list<int[3]>* cur = GRAPH.list_of_edges(v + 1); cur; cur = cur->next)
		{
			int u = cur->Ver[1] - 1;//смежная с v вершина
			int len_vu = cur->Ver[2];//вес ребра (v,u)

			int n_dst = dst + len_vu;//новое расстояние
			if (n_dst < answ[u]) //ослабление пути
			{
				answ[u] = n_dst;
				prev[u] = v;//изменяем путь
				q.push({ n_dst, u });//добавляем ребро с измененным растоянием в очередь
				count_changes++;
			}
			else if (colored[v])//если новое расстояние больше, то делаем неокрашенными обе вершины
			{
				colored[v] = false;
				colored[u] = false;
			}

		}
		bool color_check = true;//проверка на то что все вершины окрашены
		for (size_t i = 0; i < length; i++)
		{
			if (!colored[i])
				color_check = false;
		}
		if (!count_changes && color_check)//если все вершины окрашены и изменения не происходят - выходим из алгоритма
			break;
	}
	return answ[end_Ver - 1];
}
//алгоритм беллмана форда
int Bellman_Ford(Graph GRAPH, vector<int>& answ, int begin_Ver)
{
	list<int[3]>* edgelist = GRAPH.list_of_edges();//список ребер
	int length = edgelist->length(edgelist);//количество вершин
	answ.resize(length);
	for (int i = 0; i < length; i++)//заполняем массив
	{
		answ[i] = INF;
	}
	answ[begin_Ver - 1] = 0;

	//цикл |V|-1 раз
	for (int i = 0; i < length-1; i++)
	{
		//цикл по всем ребрам
		for (list<int[3]>* cur = edgelist;cur;cur = cur->next)
		{
			//релаксация
			if (answ[cur->Ver[1]-1] > answ[cur->Ver[0]-1] + cur->Ver[2])
				answ[cur->Ver[1]-1] = answ[cur->Ver[0]-1] + cur->Ver[2];
		}
	}
	//проверка на отриц. цикл
	for (list<int[3]>* cur = edgelist; cur; cur = cur->next)
	{
		//релаксация
		if (answ[cur->Ver[1]-1] > answ[cur->Ver[0]-1] + cur->Ver[2])
			return -1;

	}
	return 1;
}
//алгоритм левита
int Levit(Graph GRAPH, vector<int>& answ, int begin_Ver)
{
	list<int[3]>* edgelist = GRAPH.list_of_edges();//список ребер
	int length = edgelist->length(edgelist);//количество вершин
	answ.resize(length);

	set<int> M0;//вершины, расстояние до которых уже вычислено
	set<int> M1;//множество элементов в очереди
	queue<int> M1_1;//основнаяя очередь на обработку
	queue<int> M1_2;//срочная очередь
	set<int> M2;//вершины, расстояние до которых еще не вычисленоы
	for (int i = 0; i < length; i++)//заполняем массив
	{
		answ[i] = INF;
	}
	answ[begin_Ver - 1] = 0;

	M1_1.push(begin_Ver);//добавляем начальную вершину в очередь
	M1.insert(begin_Ver);
	//заполянем множество необработанных вершин
	for (int i = 0;i < length; i++)
	{
		M2.insert(i + 1);
	}
	M2.erase(begin_Ver);
	//пока обе очереди не пусты
	while (!M1.empty())
	{
		int u;//вершина, которую берем либо из M1 либо из M2
		//если M2 пуста, то берем из M1
		if (M1_2.empty())
		{
			u = M1_1.front();
			M1_1.pop();
			M1.erase(u);
		}
		//если M2 не пуста, то берем из неё
		else
		{
			u = M1_2.front();
			M1.erase(u);
			M1_2.pop();
		}
		//цикл по ребрам uv c 3-мя случаями
		for (list<int[3]>* cur = GRAPH.list_of_edges(u); cur; cur = cur->next)
		{
			//1 случай
			//v принадлежит M2
			if (M2.find(cur->Ver[1]) != M2.end())
			{
				//переводим v в конец очереди M1_1 и производим релаксацию ребра uv
				M1_1.push(cur->Ver[1]);
				M1.insert(cur->Ver[1]);
				M2.erase(cur->Ver[1]);
				answ[cur->Ver[1] - 1] = min(answ[cur->Ver[1] - 1], answ[u-1] + cur->Ver[2]);

			}
			//2 случай
			//v принадлежит M1
			else if (M1.find(cur->Ver[1]) != M1.end())
			{
				//производим релаксацию ребра uv
				answ[cur->Ver[1] - 1] = min(answ[cur->Ver[1] - 1], answ[u-1] + cur->Ver[2]);
			}
			//3 случай
			//v принадлежит M0 и answ[v]>answ[u]+w(uv)
			else if (M0.find(cur->Ver[1]) != M0.end() && answ[cur->Ver[1] - 1] > answ[u - 1] + cur->Ver[2])
			{
				//v помещается в М1_2 и производится релаксация ребра uv
				M1_2.push(cur->Ver[1]);
				M1.insert(cur->Ver[1]);
				M0.erase(cur->Ver[1]);
				answ[cur->Ver[1] - 1] = answ[u - 1] + cur->Ver[2];
			}
		}
		//помещаем u в М0
		M0.insert(u);
	}
	return 1;
}
int Jonson(Graph GRAPH, vector<vector<int>>& answ)
{
	
	list<int[3]>* edgelist = GRAPH.list_of_edges();
	int length = edgelist->length(edgelist);
	answ.resize(length);
	for (size_t i = 0; i < length; i++)//выделение памяти
		answ[i].resize(length);
	
	//создание нового списка ребер с доп. вершиной
	list<int[3]>* last = 0;
	for (list<int[3]>* cur = edgelist; cur; cur = cur->next)
		last = cur;
	for (int i = 0; i < length; i++)
	{
		last->next = new list<int[3]>;
		last = last->next;
		last->Ver[0] = length + 1;
		last->Ver[1] = i + 1;
		last->Ver[2] = 0;
		
	}
	
	//создание новного графа с доп. вершиной
	Graph GRAPH_PLUS1(edgelist);

	vector<int> h(length+1);//вектор весовых меток
	//запускаем беллмана форда для поиска расстояний от новой вершины до остальных
	if (Bellman_Ford(GRAPH_PLUS1, h, length + 1) == -1)
		return -1;
	//удаляем новые ребра
	for (size_t i = 0; i < length; i++)
		edgelist->del();
	
	//пересчитываем веса ребер
	for (list<int[3]>* cur = edgelist; cur; cur = cur->next)
		cur->Ver[2] = cur->Ver[2] + h[cur->Ver[0] - 1] - h[cur->Ver[1] - 1];
	
	//GRAPH_PLUS1 - теперь без доп. ребер
	//запускаем дейкстру для каждой вершины
	for (size_t i = 0; i < length; i++)
	{
		vector<int> prev;
		Dijkstra(GRAPH_PLUS1, answ[i], prev,i+1);
		//изменяем расстояния согласно массиву меток
		for (size_t j = 0; j < length; j++)
		{
			if(answ[i][j]!=INF)
				answ[i][j] = answ[i][j] + h[j] - h[i];
		}
	}
	return 1;
}
int AStar(Map MAP, list<Cell>*& way, Cell begin_Ver, Cell end_Ver, int (*h)(Cell Ver1, Cell Ver2))
{
	int length = MAP.length();
	way = new list<Cell>;
	int way_length = 0;
	vector<vector<int>> gScore(length);
	vector<vector<int>> fScore(length);
	vector<vector<Cell>> cameFrom(length);
	for (size_t i = 0; i < length; i++)
	{
		gScore[i].resize(length);
		fScore[i].resize(length);
		cameFrom[i].resize(length);
		for (size_t j = 0; j < length; j++)
		{
			gScore[i][j] = INF;
			fScore[i][j] = INF;
		}
	}
	fScore[begin_Ver.x][begin_Ver.y] = h(begin_Ver, end_Ver);
	gScore[begin_Ver.x][begin_Ver.y] = 0;
	
	priority_queue<pair<int, Cell>, vector<pair<int, Cell>>, PairWithCellGreater> q;
	q.push(pair<int, Cell>(fScore[begin_Ver.x][begin_Ver.y], begin_Ver));

	while (!q.empty())
	{
		pair<int, Cell> curVer = q.top();
		q.pop();

	}
	////приоритетная очередь рассматриваемых вершин
	//priority_queue<pair<int, Cell>, vector<pair<int, Cell>>, PairWithCellGreater> q;
	///*set <int> copyq;*/
	////вставляем в очередь начальную вершину
	//q.push(pair<int, Cell>(MAP.GetHeight(begin_Ver), begin_Ver));
	//
	///*copyq.insert(MAP.GetHeight(begin_Ver));*/
	//way->Ver = begin_Ver;

	//bool find_way = false;//флаг конца поиска
	//while (!find_way) 
	//{
	//	//вынемаем минимальную вершину из очереди
	//	pair<int, Cell> curVer = q.top();
	//	//добавляем длину ребра к длине пути
	//	if (MAP.GetHeight(curVer.second) == 0)
	//		way_length++;
	//	else if (curVer.second.x != begin_Ver.x || curVer.second.y != begin_Ver.y)
	//		way_length += MAP.GetHeight(curVer.second);
	//	//добавление вершины в путь
	//	if (curVer.second.x != begin_Ver.x || curVer.second.y != begin_Ver.y)
	//		way->add(curVer.second);
	//	q.pop();
	//	/*copyq.erase(curVer.first);*/

	//	list<pair<int, Cell>>* nbrs = MAP.neighbors(curVer.second);//список соседних вершин к текущей
	//	//прибавляем эвристику к каждой вершине
	//	for (list<pair<int, Cell>>* cur = nbrs; cur; cur = cur->next)
	//	{	
	//		bool gonext = false;
	//		for (list<Cell>* curinway = way; curinway; curinway = curinway->next)
	//		{
	//			if (curinway->Ver.x == cur->Ver.second.x && curinway->Ver.y == cur->Ver.second.y)
	//				gonext = true;
	//		}
	//		if (gonext)
	//			continue;
	//		//если соседняя вершина конечная, то заканчиваем алгоритм
	//		if (cur->Ver.second.x == end_Ver.x && cur->Ver.second.y == end_Ver.y)
	//		{
	//			if (cur->Ver.first == 0)
	//				way_length++;
	//			else
	//				way_length += cur->Ver.first;

	//			way->add(cur->Ver.second);
	//			find_way = true;
	//			break;
	//		}
	//		cur->Ver.first += h(cur->Ver.second, end_Ver);
	//		if (MAP.GetHeight(cur->Ver.second) == 0)
	//			cur->Ver.first+=way_length;
	//		
	//		
	//		//добавляем соседнюю вершину в очередь
	//		q.push(cur->Ver);
	//	}
	//	

	//	

	//}
	//if (MAP.GetHeight(begin_Ver) == 0)
	//	way_length--;
	//else
	//	way_length -= MAP.GetHeight(begin_Ver);
	return way_length;
}
vector<Cell>* reconstruct_path(vector<vector<Cell>>& cameFrom, Cell current, Cell begin)
{
	vector<Cell>* path = new vector<Cell>;
	(*path).push_back(current);
	Cell cur = current;
	while (cur.x != begin.x || cur.y != begin.y)
	{
		(*path).push_back(cameFrom[cur.x][cur.y]);
		cur = cameFrom[cur.x][cur.y];
	}
	reverse((*path).begin(), (*path).end());
	return path;
}

//*-------------- Эвристические функции ---------------*//
int Euclid(Cell Ver1, Cell Ver2)
{
	return round(sqrt((Ver1.x - Ver2.x) * (Ver1.x - Ver2.x) + (Ver1.y - Ver2.y) * (Ver1.y - Ver2.y)));
}
int Chebyshev(Cell Ver1, Cell Ver2)
{
	return max(abs(Ver1.x - Ver2.x), abs(Ver1.y - Ver2.y));
}
int Manhattan(Cell Ver1, Cell Ver2)
{
	return abs(Ver1.x - Ver2.x) + abs(Ver1.y - Ver2.y);
}
