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
	if (FType == "-l")//���� ������ ���������
	{

		ifstream fin(FPath); //����� �� �����
		adjlist = new list<list<int>*>; //��������� ������ ��� ������ ������� ������
		adjlist->Ver = new list<int>; //��������� ������ ��� ������ ������
		list<list<int>*>* current = adjlist;//��������� �� ������� ������ � ���������
		while (!fin.eof())
		{
			string str; //������� ������ � ���������
			getline(fin, str); //���������� ������ � ���������
			if (str == "") break; //����� ���� ����� ������

			istringstream strstream(str); //����� �� ������
			list<int>* current_sub = current->Ver; //��������� �� ������� ����������� �������
			while (!strstream.eof()) //���� ���������� 
			{
				strstream >> current_sub->Ver;//���������� ����� �� �������� �������
				current_sub->next = new list<int>; //��������� ������ 
				current_sub = current_sub->next; //��������

			}
			current->next = new list<list<int>*>;//��������� ������ 
			current->next->Ver = new list<int>;
			current = current->next;//��������
			

		}
	}
	if (FType == "-e")//���� ������ �����
	{
		ifstream fin(FPath); //����� �� �����
		edgelist = new list<int[3]>; //��������� ������ ��� ������ �����

		list<int[3]>* current = edgelist;//��������� �� ������� �����
		while (!fin.eof())
		{
			string str; //������� �������� �����
			getline(fin, str); //���������� �����
			if (str == "") break; //����� ���� ����� ������

			istringstream strstream(str); //����� �� ������
			for (int i = 0; i < 3; i++) //���� ���������� 
			{
				strstream >> current->Ver[i];//���������� ������� ��� ���� �����
				if (strstream.eof() && i == 1) //���� ��� ����� �� ������, �� ������������ ��� � 1
				{
					current->Ver[2] = 1;
					break;
				}
			}
			
			current->next = new list<int[3]>; //��������� ������
			current = current->next;//��������
		}
	}
	if (FType == "-m")//���� ������� ���������
	{
		//���������� ������ �������
		ifstream in_ch(FPath); //����� �� �����
		int h_matr = 0;
		string str_ch;
		string substr_ch;//����������� ����������
		getline(in_ch, str_ch);
		istringstream strstream_ch(str_ch); //����� �� ������
		while (!strstream_ch.eof()) //���� ���������� 
		{
			strstream_ch >> substr_ch;
			if (!strstream_ch.eof())
				h_matr++;
		}
		string str;
		string substr;
		ifstream fin(FPath); //����� �� �����

		matrix = new vector<vector<int>>;
		(*matrix).resize(h_matr);
		for (int i = 0; i < h_matr; i++)//�� ��������
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