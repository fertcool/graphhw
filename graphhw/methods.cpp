#pragma once
#define INF 9999999
#include "Header.h"
#include <iomanip>
#include <fstream>
#include <queue>
#include <stack>

//*-------------- ф-ии класса графа ------------------*//
//возвращает вес ребра по его вершинам
int Graph::weight(int Ver1, int Ver2) 
{
	if (!matrix)
		adjacency_matrix(false);
	return (*matrix)[Ver1-1][Ver2-1];
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
	if (!matrix)
		adjacency_matrix(false);
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
vector<vector<int>>* Graph::adjacency_matrix(bool copy)
{
	if (matrix && copy)//возврат копии матрицы
	{
		vector<vector<int>>* matr_c;//копия матрицы
		int h_matr = matrix->size();
		matr_c = new vector<vector<int>>;
		(*matr_c).resize(h_matr);
		for (int i = 0; i < h_matr; i++)//выделение памяти
		{
			(*matr_c)[i].resize(h_matr);
		}
		for (int i = 0; i < h_matr; i++)//копирование матрицы
		{
			for (int j = 0; j < h_matr; j++)
			{
				(*matr_c)[i][j] = (*matrix)[i][j];
			}
		}
		return matr_c;
	}
	else if (matrix && !copy)//если нужно было только создать матрицу смежности без копии
		return NULL;
	if (edgelist)//заполнение матрицы из списка ребер
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

		matrix = new vector<vector<int>>;
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
			(*matrix)[current->Ver[0]-1][current->Ver[1]-1] = current->Ver[2];
		}
		return adjacency_matrix(copy);
	}
	if (adjlist)//заполнение матрицы из списка смежности
	{

		int h_matr = adjlist->length();//длина матрицы

		matrix = new vector<vector<int>>;
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
				(*matrix)[i][subcurrent->Ver-1] = 1;
			}
			i++;
		}
		return adjacency_matrix(copy);
	}
	return NULL;
};

//возвращает список смежности
list<list<int>*>* Graph::adjacency_list(bool copy)
{
	if (adjlist && copy)//возврат копии списка смежности
	{
		list<list<int>*>* list_adj_c = new list<list<int>*>; //копия списка
		list_adj_c->Ver = new list<int>;
		list<list<int>*>* current_c = list_adj_c; //текущая вершина в копии списка
		list<list<int>*>* current = adjlist; //текущая вершина в обычном списке
		int length = adjlist->length();
		for (int i = 0; i < length ; i++)//цикл по вершинам
		{

			int length_subl = current->Ver->length();//длина подсписка (количество смежных вершин j с текущей вершиной i)
			list<int>* current_c_subl = current_c->Ver;//текущая вершина j в копии списка смежных вершин с текущей i
			list<int>* current_subl = current->Ver;//текущая вершина j в обычном списке смежная с текущей i
			for (int j = 0; j < length_subl; j++)//цикл по смежным вершинам j с текущей i
			{
				current_c_subl->Ver = current_subl->Ver;//копирование
				if (j != length_subl - 1)//создание следующего элемента в копии списка смежных вершин с текущей i
				{
					current_c_subl->next = new list<int>;
					current_c_subl = current_c_subl->next;//итерация
					current_subl = current_subl->next;
				}
			}
			if (i != length - 1)//создание следующего элемента в копии списка 
			{
				current_c->next = new list<list<int>*>;
				current_c->next->Ver = new list<int>;
				current_c = current_c->next;//итерация
				current = current->next;
			}

 		}
		return list_adj_c;
	}
	else if (adjlist && !copy)//если нужно было только создать список смежности (без копирования)
		return NULL;
	if (!matrix)//создание матрицы смежности
		adjacency_matrix(false);
	int lsize = matrix->size();//размер списка (матрицы)
	adjlist = new list<list<int>*>; //выделение памяти под список списков вершин
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
	return adjacency_list(copy);
};

//возвращает список вершин смежных Ver
list<int>* Graph::adjacency_list(int Ver)
{
	if (!adjlist)//сохдание списка смежности
		adjacency_list(false);

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
	if (!matrix)//сохдание списка смежности
		adjacency_matrix(false);
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

//возвращает список ребер
list<int[3]>* Graph::list_of_edges(bool copy)
{
	if (edgelist && copy)//возврат копии списка ребер
	{
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
	}
	else if (edgelist && !copy)
		return NULL;
		
	if (!matrix)//создание матрицы смежности
		adjacency_matrix(false);
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
	return list_of_edges(copy);

};

//возвращает список всех ребер иcходящих из Ver
list<int[3]>* Graph::list_of_edges(int Ver)
{
	if (!matrix)//создание матрицы смежности
		adjacency_matrix(false);
	list<int[3]>* list_out = new list<int[3]>;//выделение памяти для списка исходящих ребер
	int init = list_out->Ver[0];//изначальный мусор в списке
	list<int[3]>* current_out = list_out;//текущее ребро, исходящее из Ver
	int lmatr = matrix->size();//размер матрицы смежности
	
	for (int i = 0; i < lmatr; i++)//цикл просмотра смежных с Ver вершин и добавления исходящего ребра в список ребер
	{
		if ((*matrix)[Ver - 1][i])
		{
			current_out->Ver[0] = Ver;//добавление 1 вершины
			current_out->Ver[1] = i + 1;//добавление исходящей вершины
			current_out->Ver[2] = (*matrix)[Ver - 1][i];//добавление веса ребра

			current_out->next = new list<int[3]>;//выделение памяти
			current_out = current_out->next;//итерация
		}
	}
	list_out->del();
	if (list_out->Ver[0] == init)//если список вершин пуст
		return NULL;
	return list_out;

};


//конструктор
Graph::Graph(string FPath, string FType)
{
	adjlist = NULL;
	edgelist = NULL;
	matrix = NULL;
	if (FType == "-l")//ввод списка смежности
	{
		//реализовано без удаления лишних элементов//

		ifstream fin(FPath); //поток по файлу
		adjlist = new list<list<int>*>; //выделение памяти под список списков вершин
		adjlist->Ver = new list<int>; //выделение памяти под список вершин
		list<list<int>*>* current = adjlist;//указатель на текущий список с вершинами
		while (!fin.eof())
		{
			string str; //текущая строка с вершинами
			getline(fin, str); //считывание строки с вершинами
			if (str == "") //выход если конец файла(лишняя паустая строка)
			{
				adjlist->del();
				break;
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
		

	}
	if (FType == "-e")//ввод списка ребер
	{
		//реализовано без удаления лишних элементов//

		ifstream fin(FPath); //поток по файлу
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

		matrix = new vector<vector<int>>;
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
	if (!inf)//выводим эксцентриситеты, если они существуют
	{
		stream_out << "Eccentricity:" << endl;
		print_vector(exc, stream_out);
	}
	if (is_directed)//выход из программы если граф ориентированный
		return;
	//нахождение диаметра
	max = 0;
	for (int i = 0; i < length; i++)
	{
		if ((*exc)[i] > max)
			max = (*exc)[i];
	}
	stream_out << "D = " << max<<endl;
	//нахождение радиуса
	int min = INF;
	for (int i = 0; i < length; i++)
	{
		if ((*exc)[i] < min)
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
void second_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out)
{
	list<list<int>*>* adjlist = GRAPH.adjacency_list();//список смежности
	int length = adjlist->length();//длина списка(количество вершин)
	int num_comp = 1;//номер компоненты
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
		for (size_t i = 0; i < length; i++)
			used[i] = 0;
		
		Graph Graph_Inv(edgelist);//инвертированный граф
		vector<int>* order = TopologicalSort(Graph_Inv);//вектор топологической сортировки инвертированного графа
		num_comp = 1;
		for (size_t i = 0; i < order->size(); i++)//цикл DFS по вершинам обычного графа в порядке топологической сортировки инвертированного
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
//поиск в глубину(возвращает вектор заполненный последоваетльно пройденными вершинами)
vector<int>* TopologicalSort(Graph GRAPH)
{
	int length = GRAPH.adjacency_list()->length();
	vector<int> used(length);
	vector<int>* sortedgraph = new vector<int>;
	for (int i = 0; i < length; i++)
	{
		if (!used[i])
		{
			DFS(GRAPH, &used, i+1, 1,sortedgraph);
			
		}
			
	}
	reverse(sortedgraph->begin(), sortedgraph->end());
	return sortedgraph;
}
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
