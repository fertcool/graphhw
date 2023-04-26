#pragma once
#define INF 9999999
#include "Header.h"
#include <iomanip>
#include <fstream>

//*-------------- �-�� ������ ����� ------------------*//
//���������� ��� ����� �� ��� ��������
int Graph::weight(int Ver1, int Ver2) 
{
	if (!matrix)
		adjacency_matrix(false);
	return (*matrix)[Ver1-1][Ver2-1];
};

//���������� �� ����� �� ������ ��������
bool Graph::is_edge(int Ver1, int Ver2)
{
	if (weight(Ver1, Ver2))
		return true;
	return false;
};

//���� ��������������� ��� ���
bool Graph::is_directed() 
{
	if (!matrix)
		adjacency_matrix(false);
	int msize = matrix->size();
	for (int i = 0; i < msize - 1; i++)//���� �� ��������� ���� ������� ���������
	{
		for (int j = i + 1; j < msize; j++)
		{
			if (!((*matrix)[i][j] && (*matrix)[j][i]) && !((*matrix)[i][j] == 0 && (*matrix)[j][i] == 0))
				return true;
		}
	}
	return false;

};

//���������� ������� ���������
vector<vector<int>>* Graph::adjacency_matrix(bool copy)
{
	if (matrix && copy)//������� ����� �������
	{
		vector<vector<int>>* matr_c;//����� �������
		int h_matr = matrix->size();
		matr_c = new vector<vector<int>>;
		(*matr_c).resize(h_matr);
		for (int i = 0; i < h_matr; i++)//��������� ������
		{
			(*matr_c)[i].resize(h_matr);
		}
		for (int i = 0; i < h_matr; i++)//����������� �������
		{
			for (int j = 0; j < h_matr; j++)
			{
				(*matr_c)[i][j] = (*matrix)[i][j];
			}
		}
		return matr_c;
	}
	else if (matrix && !copy)//���� ����� ���� ������ ������� ������� ��������� ��� �����
		return NULL;
	if (edgelist)//���������� ������� �� ������ �����
	{
		//���������� ������� �������(������������ ������� � ������ �����)
		int h_matr = 0;
		list<int[3]>* current = edgelist;
		for (current; current; current = current->next)//���������� ������� �������(������������ ������� � ������ �����)
		{
			if (h_matr < current->Ver[0])
				h_matr = current->Ver[0];
		}

		matrix = new vector<vector<int>>;
		(*matrix).resize(h_matr);
		for (int i = 0; i < h_matr; i++)//��������� ������
		{
			(*matrix)[i].resize(h_matr);
		}
		for (int i = 0; i < h_matr; i++)//���������� ������� 0
		{
			for (int j = 0; j < h_matr; j++)
			{
				(*matrix)[i][j] = 0;
			}
		}

		current = edgelist;
		for (current; current; current = current->next)//���� ���������� �������
		{
			(*matrix)[current->Ver[0]-1][current->Ver[1]-1] = current->Ver[2];
		}
		return adjacency_matrix(copy);
	}
	if (adjlist)//���������� ������� �� ������ ���������
	{

		int h_matr = adjlist->length();//����� �������

		matrix = new vector<vector<int>>;
		(*matrix).resize(h_matr);
		for (int i = 0; i < h_matr; i++)//��������� ������
		{
			(*matrix)[i].resize(h_matr);
		}

		for (int i = 0; i < h_matr; i++)//���������� ������� 0
		{
			for (int j = 0; j < h_matr; j++)
			{
				(*matrix)[i][j] = 0;
			}
		}

		list<list<int>*>* current = adjlist;
		int i = 0;
		for (current; current; current = current->next)//���� ���������� �������
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

//���������� ������ ���������
list<list<int>*>* Graph::adjacency_list(bool copy)
{
	if (adjlist && copy)//������� ����� ������ ���������
	{
		list<list<int>*>* list_adj_c = new list<list<int>*>; //����� ������
		list_adj_c->Ver = new list<int>;
		list<list<int>*>* current_c = list_adj_c; //������� ������� � ����� ������
		list<list<int>*>* current = adjlist; //������� ������� � ������� ������
		int length = adjlist->length();
		for (int i = 0; i < length ; i++)//���� �� ��������
		{

			int length_subl = current->Ver->length();//����� ��������� (���������� ������� ������ j � ������� �������� i)
			list<int>* current_c_subl = current_c->Ver;//������� ������� j � ����� ������ ������� ������ � ������� i
			list<int>* current_subl = current->Ver;//������� ������� j � ������� ������ ������� � ������� i
			for (int j = 0; j < length_subl; j++)//���� �� ������� �������� j � ������� i
			{
				current_c_subl->Ver = current_subl->Ver;//�����������
				if (j != length_subl - 1)//�������� ���������� �������� � ����� ������ ������� ������ � ������� i
				{
					current_c_subl->next = new list<int>;
					current_c_subl = current_c_subl->next;//��������
					current_subl = current_subl->next;
				}
			}
			if (i != length - 1)//�������� ���������� �������� � ����� ������ 
			{
				current_c->next = new list<list<int>*>;
				current_c->next->Ver = new list<int>;
				current_c = current_c->next;//��������
				current = current->next;
			}

 		}
		return list_adj_c;
	}
	else if (adjlist && !copy)//���� ����� ���� ������ ������� ������ ��������� (��� �����������)
		return NULL;
	if (!matrix)//�������� ������� ���������
		adjacency_matrix(false);
	int lsize = matrix->size();//������ ������ (�������)
	adjlist = new list<list<int>*>; //��������� ������ ��� ������ ������� ������
	adjlist->Ver = new list<int>; //��������� ������ ��� ������ ������
	list<list<int>*>* current = adjlist;
	for (int i = 0; i < lsize; i++)
	{
		list<int>* current_sub = current->Ver;//��������� �� ������� ������ ������
		for (int j = 0; j < lsize; j++)
		{
			if ((*matrix)[i][j])
			{
				current_sub->Ver = j + 1; //���������� ������� � ������
				current_sub->next = new list<int>;//��������
				current_sub = current_sub->next;
			}
		}
		current->Ver->del();//������� ������ �������
		current->next = new list<list<int>*>;//��������
		current->next->Ver = new list<int>;
		current = current->next;
	}
	adjlist->del();//������� ������ �������
	return adjacency_list(copy);
};

//���������� ������ ������ ������� Ver
list<int>* Graph::adjacency_list(int Ver)
{
	if (!adjlist)//�������� ������ ���������
		adjacency_list(false);
	list<int>* adjlist_c = new list<int>;//����� ������ ������� ������
	list<int>* current_c = adjlist_c; // ������� ������� � ����� ������
	list<list<int>*>* current = adjlist;//������� ������� � �������� ������
	for (int i = 0; i < Ver - 1; i++)//���� �������� ��������� �� ������ ������ ������� ������
	{
		current = current->next;
	}
	list<int>* current_sub = current->Ver;//������� ������� j ������� � Ver
	for (current_sub;current_sub;current_sub = current_sub->next)//���� �����������
	{
		current_c->Ver = current_sub->Ver;//�����������
		if (current_sub->next)//��������� ������ ��� ���������� ��������
		{
			current_c->next = new list<int>;
			current_c = current_c->next;
		}
	}
	
	return adjlist_c;
}
//���������� ������ ������ �������� � Ver
list<int>* Graph::adjacency_list_in(int Ver)
{
	if (!matrix)//�������� ������ ���������
		adjacency_matrix(false);
	list<int>* adjlistin_c = new list<int>;//����� ������ ������� ������
	int init = adjlistin_c->Ver;//����������� ����� � ������
	list<int>* current_c = adjlistin_c; // ������� ������� � ����� ������
	
	int length = matrix->size();//����� ������ (���������� ������)
	for (int i = 0; i<length; i++) //���� �� ���� �������� ��������� ������
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
	if (adjlistin_c->Ver == init)//���� ������ ������ ����
		return NULL;
	return adjlistin_c;
}

//���������� ������ �����
list<int[3]>* Graph::list_of_edges(bool copy)
{
	if (edgelist && copy)//������� ����� ������ �����
	{
		list<int[3]>* edgelist_c = new list<int[3]>;//����� ������ �����
		list<int[3]>* current_c = edgelist_c;//������� ����� � �����
		list<int[3]>* current = edgelist;//������� ����� � �������� ������
		int length = edgelist->length();//����� ������ �����
		for (int i = 0; i < length; i++)//���� �����������
		{
			current_c[0] = current[0];
			current_c[1] = current[1];//�����������
			current_c[2] = current[2];
			if (i != length - 1)//��������� ������ ��� ���������� ��������
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
		
	if (!matrix)//�������� ������� ���������
		adjacency_matrix(false);
	int lsize = matrix->size();//������ ������ (�������)
	edgelist = new list<int[3]>;//��������� ������ ��� �����
	list<int[3]>* current = edgelist; //������� �����
	for (int i = 0; i < lsize; i++)
	{
		for (int j = 0; j < lsize; j++)
		{
			if ((*matrix)[i][j]) //���� ������� �������
			{
				current->Ver[0] = i + 1;//1 �������
				current->Ver[1] = j + 1;//2 �������
				current->Ver[2] = (*matrix)[i][j];//��� �����
				current->next = new list<int[3]>;
				current = current->next;
			}
		}
	}
	edgelist->del();
	return list_of_edges(copy);

};

//���������� ������ ���� ����� �c������� �� Ver
list<int[3]>* Graph::list_of_edges(int Ver)
{
	if (!matrix)//�������� ������� ���������
		adjacency_matrix(false);
	list<int[3]>* list_out = new list<int[3]>;//��������� ������ ��� ������ ��������� �����
	list<int[3]>* current_out = list_out;//������� �����, ��������� �� Ver
	int lmatr = matrix->size();//������ ������� ���������
	
	for (int i = 0; i < lmatr; i++)//���� ��������� ������� � Ver ������ � ���������� ���������� ����� � ������ �����
	{
		if ((*matrix)[Ver - 1][i])
		{
			current_out->Ver[0] = Ver;//���������� 1 �������
			current_out->Ver[1] = i + 1;//���������� ��������� �������
			current_out->Ver[2] = (*matrix)[Ver - 1][i];//���������� ���� �����

			current_out->next = new list<int[3]>;//��������� ������
			current_out = current_out->next;//��������
		}
	}
	list_out->del();
	return list_out;

};


//�����������
Graph::Graph(string FPath, string FType)
{
	adjlist = NULL;
	edgelist = NULL;
	matrix = NULL;
	if (FType == "-l")//���� ������ ���������
	{
		//����������� ��� �������� ������ ���������//

		ifstream fin(FPath); //����� �� �����
		adjlist = new list<list<int>*>; //��������� ������ ��� ������ ������� ������
		adjlist->Ver = new list<int>; //��������� ������ ��� ������ ������
		list<list<int>*>* current = adjlist;//��������� �� ������� ������ � ���������
		while (!fin.eof())
		{
			string str; //������� ������ � ���������
			getline(fin, str); //���������� ������ � ���������
			if (str == "") //����� ���� ����� �����(������ ������� ������)
			{
				adjlist->del();
				break;
			}

			istringstream strstream(str); //����� �� ������
			list<int>* current_sub = current->Ver; //��������� �� ������� ����������� �������
			
			//����� ������ ������ ���� ������� ����� 
			//(��� �������, ����� ������ ����������� ���� � ���������� ������ ����� ������ �����)
			int check_space;//�������� �� ������ � ����� ������
			strstream >> current_sub->Ver;//���������� ����� �� �������� �������
	
			while (!strstream.eof()) //���� ���������� 
			{
				strstream >> check_space;//���������� ����� �� �������� �������
				if (strstream.eof())//���� ����� ������, �� �������
					break;
				current_sub->next = new list<int>;//��������� ������ 
				current_sub = current_sub->next;//��������
				current_sub->Ver = check_space;//���������� ����� �� �������� �������
			}
			if (fin.eof())//����� ���� ����� (����� ������ ������ �� ��������)
				break;
			current->next = new list<list<int>*>;//��������� ������ 
			current->next->Ver = new list<int>;
			current = current->next;//��������
			
		}
		

	}
	if (FType == "-e")//���� ������ �����
	{
		//����������� ��� �������� ������ ���������//

		ifstream fin(FPath); //����� �� �����
		edgelist = new list<int[3]>; //��������� ������ ��� ������ �����

		list<int[3]>* current = edgelist;//��������� �� ������� �����
		
		while (!fin.eof())
		{
			string str; //������� �������� �����
			getline(fin, str); //���������� �����
			if (str == "") //����� ���� ����� �����(������ ������� ������)
			{
				edgelist->del();
				break;
			}
			istringstream strstream(str); //����� �� ������

			string check_space;//�������� �� ������ � ����� ������
			//strstream >> current->Ver[0]; //���������� ������� ��� ���� �����

			for (int i = 0; i < 2; i++) //���� ���������� ���� ������
			{
				strstream >> current->Ver[i]; //���������� ������� ��� ���� �����
				
			}
			strstream >> check_space;//�������� � ���������� ���� (���� �����, �� 1)
			if (check_space == "")
				current->Ver[2] = 1;
			else
				current->Ver[2] = stoi(check_space);

			if (fin.eof())//����� ���� ����� (����� ������ ������ �� ��������)
				break;
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
		for (int i = 0; i < h_matr; i++)//��������� ������
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


//*-------------- �-�� �������� ------------------*//

//������������� ����� � ������ (����������� (������ + 1) ��� (0))
int exist_key(int argc, char* argv[], string key)
{
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == key)
			return i + 1;
	}
	return 0;
}
//���� ������ 1 ���� ����� �����
bool once_keyfile(int argc, char* argv[])
{
	bool ex_e = exist_key(argc, argv, "-e");
	bool ex_m = exist_key(argc, argv, "-m");
	bool ex_l = exist_key(argc, argv, "-l");
	if (ex_e && ex_m || ex_e && ex_l || ex_m && ex_l)
	{
		cout << "\n������� ��������� ������ ����� �����!!!\n" << endl;
		return false;
	}
	return true;
}
//�������
void help(int argc, char* argv[])
{
	
	cout << "����� ������: �������� �.�.\n" << endl;
	cout << "������: M30-211�-21\n" << endl;
	cout << "������ ������ � �����������: \n" << endl;
	cout << "������� ������ ��� ������ ��������� �������� ���������� ������� � �����������:" << endl;
	cout << "-e \"edges_list_file_path\"" << endl;
	cout << "-l \"adjacency_list_file_path\"" << endl;
	cout << "-m \"adjacency_matrix_file_path\"\n" << endl;
	cout << "������������ ����� ����������� ������ ���� �� ���� ������!\n" << endl;
	cout << "���������� ������ ��������� �� �����, ���� � ���� ��� �������� ���������� �����: " << endl;
	cout << "-o \"output_file_path\"\n" << endl;	
	
	cout << "�������������� ����� ��� ���������: \n" << endl;
	cout << "�4:" << endl;
	cout << "�������� ��� ������� ������� ���������� �������: " << endl;
	cout << "-k � �������� ��������" << endl;	
	cout << "-p � �������� �����" << endl;
	cout << "-b � �������� �������" << endl;
	cout << "-s � ������ ������������ ����� ����������� ���������\n" << endl;
	cout << "�5:" << endl;
	cout << "��������� ������� ������� ��������� ������:" << endl;
	cout << "-n begin_vertex_number - ������ ���� �������� ������������." << endl;
	cout << "�������� ������� ������� ��������� ������:" << endl;
	cout << "-d end_vertex_number - ������ ���� �������� ������������.\n" << endl;
	cout << "�6:" << endl;
	cout << "�������� ��� ������� ������� ���������� �������:" << endl;
	cout << "-d � �������� ��������" << endl;
	cout << "-b � �������� ��������-�����-����" << endl;
	cout << "-t � �������� ������\n" << endl;
	cout << "��������� ������� ������� ��������� ������:" << endl;
	cout << "-n begin_vertex_number - ������ ���� �������� ������������\n" << endl;
	cout << "�8:" << endl;
	cout << "������� ���� ��� ������ ��������� ������� ��������� ������:" << endl;
	cout << "-m \"map_file_path\"\n" << endl;
	cout << "��������� ������� ������� ��������� ������:" << endl;
	cout << "-n begin_vertex_x begin_vertex_y - ������ ���� �������� ������������.\n" << endl;
	cout << "�������� ������� ������� ��������� ������:" << endl;
	cout << "-d end_vertex_x end_vertex_y - ������ ���� �������� ������������.\n" << endl;
	cout << "�9:" << endl;
	cout << "�������� ��� ������� ������� ���������� �������:" << endl;
	cout << "-b � ����� ������ � ������" << endl;
	cout << "-a � ����� ���������� �������\n" << endl;
	cout << "��������� ������� ������� ��������� ������:" << endl;
	cout << "-n begin_vertex_number - ������ ���� �������� ������������" << endl;
}
//����� ���������
int num_programm(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (atoi(argv[i]) < 0)
			return abs(atoi(argv[i]));
	}
	return 0;
}
//������� ����, ���� �� �� ����������, � ���������� ��� �����
ofstream create_file(string filename)
{
	ofstream file(filename);
	return file;
}

//*-------------- �-�� ��� �������� ------------------*//
//��� 1 �������
//������ �������
void print_matrix(vector<vector<int>>* matrix, ostream& stream_out)
{
	int length = matrix->size();
	for (int i = 0; i < length; i++)//����� ������� ����� ���������� ����������
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
//������ �������
void print_vector(vector<int>* vec, ostream& stream_out)
{
	int length = vec->size();
	stream_out << "[";
	for (int i = 0; i < length-1; i++)
	{
		stream_out << (*vec)[i]<<", ";
	}
	stream_out << (*vec)[length-1] << "]"<<endl;
}
void first_task(int argc, char* argv[], Graph GRAPH, ostream& stream_out)
{
	
	vector<vector<int>>* matrix = GRAPH.adjacency_matrix();//������� ���������
	int length = matrix->size();//������ �������
	int count;//�������
	bool is_directed = GRAPH.is_directed();//���� �����������������
	vector<int>* deg_in = new vector<int>;//������ �������� ����� ������
	deg_in->resize(length);
	//����� �������� ������
	if (is_directed)//���� ���� ���������������
	{
		
		stream_out << "deg+ = ";
		for (int j = 0; j < length; j++)//���� ���������� �������� ����� (����� ��������� �������)
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
	vector<int>* deg_out = new vector<int>;//������ �������� ����� ������
	deg_out->resize(length);
	for (int i = 0; i < length; i++)//���� ���������� �������� ������ (����� ��������� �������)
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
	
	//������� ������� ����������
	Floyd_Warshall(matrix);

	stream_out << endl;
	print_matrix(matrix, stream_out);
	
	//����� ����������������
	int max;
	bool inf = false;
	vector<int>* exc = new vector<int>;//������ ����������������
	exc->resize(length);
	for (int i = 0; i < length; i++)
	{
		max = 0;
		for (int j = 0; j < length; j++)
		{
			if ((*matrix)[i][j] > max)
			{
				max = (*matrix)[i][j];
				if (max == INF)//���� ���� ������������� �� �������
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
	if (!inf)//������� ���������������, ���� ��� ����������
	{
		stream_out << "Eccentricity:" << endl;
		print_vector(exc, stream_out);
	}
	if (is_directed)//����� �� ��������� ���� ���� ���������������
		return;
	//���������� ��������
	max = 0;
	for (int i = 0; i < length; i++)
	{
		if ((*exc)[i] > max)
			max = (*exc)[i];
	}
	stream_out << "D = " << max<<endl;
	//���������� �������
	int min = INF;
	for (int i = 0; i < length; i++)
	{
		if ((*exc)[i] < min)
			min = (*exc)[i];
	}
	stream_out << "R = " << min << endl;
	//���������� ����������� ������
	vector<int>* Z = new vector<int>;
	stream_out << "Z = ";
	for (int i = 0; i < length; i++)
	{
		if ((*exc)[i] == min)
			Z->push_back(i + 1);
	}
	print_vector(Z, stream_out);
	//���������� ������������ ������
	vector<int>* P = new vector<int>;
	stream_out << "P = ";
	for (int i = 0; i < length; i++)
	{
		if ((*exc)[i] == max)
			P->push_back(i + 1);
	}
	print_vector(P, stream_out);
}
//*-------------- ��������� ------------------*//
//�������� ������
vector<vector<int>>* Floyd_Warshall(vector<vector<int>>* matrix)
{
	int length = matrix->size();
	for (int i = 0; i < length; i++)//���������� ��������������� ������������ ������
	{
		for (int j = 0; j < length; j++)
		{
			if ((*matrix)[i][j] == 0 && i!=j)
				(*matrix)[i][j] = INF;
		}
	}
	for (int k = 0; k < length; k++)//�������� ������
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
