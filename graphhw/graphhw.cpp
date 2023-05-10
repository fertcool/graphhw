﻿#include<iostream>
#include "Header.h"
#include <iomanip>
#include <fstream>

//#include <boost/chrono.hpp>
using namespace std;
int main(int argc, char* argv[])
{
	

	setlocale(LC_ALL, "rus");
	if (exist_key(argc, argv, "-h"))//справка
	{
		help(argc, argv);
		system("pause"); 
		return 0;
	}
	if (!once_keyfile(argc, argv))//проверка на 1 ключ ввода в файл
	{
		system("pause");
		return 0;
	}
	string types[3] = { "-e","-m","-l" };
	Graph GRAPH;
	for (auto s : types) 
	{
		if (exist_key(argc, argv, s))
		{
			GRAPH=Graph(argv[exist_key(argc, argv, s)], s);
			break;
		}

	}
	if (GRAPH.list_of_edges()->length()==1)//если данные не введены
		return 0;
	list<int[3]>* dsjdsj = GRAPH.list_of_edges();
	


	int num_pr = num_programm(argc, argv);// "/num_pr" - ключ номера запускаемой программы(алгоритма)

	bool out = 0;//флаг вывода в файл
	ofstream file_out;//поток выаода в файл
	if (exist_key(argc, argv, "-o"))
	{
		file_out = create_file(argv[exist_key(argc, argv, "-o")]);//создание файла
		out = 1;
	}
	switch (num_pr)
	{

		case 1:
			if (out)
				first_task(argc, argv, GRAPH, file_out);
			else
				first_task(argc, argv, GRAPH, cout);
				break;
		case 2:
			if (out)
				second_task(argc, argv, GRAPH, file_out);
			else
				second_task(argc, argv, GRAPH, cout);
			break;
		case 3:
			if (out)
				third_task(argc, argv, GRAPH, file_out);
			else
				third_task(argc, argv, GRAPH, cout);
			break;
		case 4:
			if (out)
				fourth_task(argc, argv, GRAPH, file_out);
			else
				fourth_task(argc, argv, GRAPH, cout);
			break;
		case 5:
			if (out)
				fifth_task(argc, argv, GRAPH, file_out);
			else
				fifth_task(argc, argv, GRAPH, cout);
			break;
		case 6:
			if (out)
				sixth_task(argc, argv, GRAPH, file_out);
			else
				sixth_task(argc, argv, GRAPH, cout);
			break;
		case 7:
			if (out)
				seventh_task(argc, argv, GRAPH, file_out);
			else
				seventh_task(argc, argv, GRAPH, cout);
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		default:
			break;
	}
	//добавить вывод спрвки 
	//вывод в файл(с его созданием)
	system("pause");
}
