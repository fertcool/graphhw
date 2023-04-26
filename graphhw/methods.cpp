#pragma once
#include "Header.h"

//float Graph::weight(int Ver1, int Ver2){};
//bool Graph::is_edge(int Ver1, int Ver2){};
//bool Graph::is_directed() {};
//int** Graph::adjacency_matrix(){};
//list Graph::adjacency_list(int Ver){};
//list Graph::list_of_edges(){};
//list Graph::list_of_edges(int Ver) {};
Graph::Graph(string FPath, string FType)
{
	if (FType == "-l")//ввод списка смежности
	{

		ifstream fin(FPath); //поток по файлу
		adjlist = new list<list<int>*>; //выделение памяти под список списков вершин
		adjlist->Ver = new list<int>; //выделение памяти под список вершин
		list<list<int>*>* current = adjlist;//указатель на текущий список с вершинами
		while (!fin.eof())
		{
			string str; //текущая строка с вершинами
			getline(fin, str); //считывание строки с вершинами
			if (str == "") break; //выход если конец строки

			istringstream strstream(str); //поток по строке
			list<int>* current_sub = current->Ver; //указатель на текущую считываемую вершину
			while (!strstream.eof()) //цикл считывания 
			{
				strstream >> current_sub->Ver;//считывание числа из строчной вершины
				current_sub->next = new list<int>; //выделение памяти 
				current_sub = current_sub->next; //итерация

			}
			current->next = new list<list<int>*>;//выделение памяти 
			current->next->Ver = new list<int>;
			current = current->next;//итерация
			

		}
	}
	if (FType == "-e")//ввод списка ребер
	{
		ifstream fin(FPath); //поток по файлу
		edgelist = new list<int[3]>; //выделение памяти под список ребер

		list<int[3]>* current = edgelist;//указатель на текущий ребро
		while (!fin.eof())
		{
			string str; //строчка текущего ребра
			getline(fin, str); //считывание ребра
			if (str == "") break; //выход если конец строки

			istringstream strstream(str); //поток по строке
			for (int i = 0; i < 3; i++) //цикл считывания 
			{
				strstream >> current->Ver[i];//считывание вершины или веса ребра
				if (strstream.eof() && i == 1) //если вес ребра не указан, то приравниваем его к 1
				{
					current->Ver[2] = 1;
					break;
				}
			}
			
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
		for (int i = 0; i < h_matr; i++)//мб поменять
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