#define _CRT_SECURE_NO_DEPRECATE 

#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include "omp.h"

using namespace std;


const int N = 360; //Размер матрицы 

/*void CreatAFile(int n,char filename[50])
{

	ofstream f(filename);
	if (!f.is_open()) 
		std::cerr << "Can't open first file!" << std::endl;

		int i, j;
		int a;
		for (i = 0; i < n; i++) // цикл по строкам a
		{
			for (j = 0; j < n; j++) // цикл по столбцам
			{
				if (j <= i)
				{
					a = 1 + rand() % 20;
					f<< a<<"";
				}
				else
				{
					a = 0;
					f << a;
				}
			}
			f << "\n";
		}
		f.close();

}*/


void create_A_file(int n, FILE *filename)
{
	int i, j;
	int a;
	for (i = 0; i<n; i++) // цикл по строкам a
	{
		for (j = 0; j <n; j++) // цикл по столбцам
		{
			if (j <= i)
			{
				a = 1 + rand() % 20;
				fprintf(filename, "%5d", a);
			}
			else
			{
				a = 0;
				fprintf(filename, "%5d", a);
			}
		}
		fprintf(filename, "\n");
	}
}

void create_B_file(int n, FILE *filename)
{
	int i, j;
	int a;
	for (i = 0; i<n; i++) // цикл по строкам a
	{
		for (j = 0; j <n; j++) // цикл по столбцам
		{
			if (j >= i)
			{
				a = 1 + rand() % 20;
				fprintf(filename, "%5d", a);
			}
			else
			{
				a = 0;
				fprintf(filename, "%5d", a);
			}
		}
		fprintf(filename, "\n");
	}
}

void file_to_arr(int* &a, const int n, FILE *filename)
{
	int i, j,tmp;
	while (!feof(filename)) {
		for (i = 0; i<n; i++) // цикл по строкам a
		{
			for (j = 0; j<n; j++) // цикл по столбцам
			{
				fscanf(filename, "%5d",&a[i*n + j]); //
				//a[i*n + j] = tmp;
				 //(a+i*n + j)
			}
		}
	}
}

//Процедура, из нижнетреугольной матрицы делает симметричную
void make_symmetrical(int* &a,int n)
{
	for (int i(0); i < n; i++)
		for (int j(i + 1); j < n; j++)
			a[i*n + j] = a[j*n + i];
}


/*
void PrintVector(int n,const int *a) //Вывод массива на экран
{

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j) {
			cout.width(5);
			cout << *(a + i*n + j);
		}
		cout << endl;
	}
}*/

void print_arr(const int a[],int n) //Вывод массива на экран
{

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j) {
			cout.width(5);
			cout << a[i*n + j];
		}
		cout << endl;
	}
}

void print_vec(const int a[], int n)
{
	for (int i = 0; i < n; ++i)
	{		
			cout.width(5);
			cout << a[i];
	}
	cout << endl;
}

//Из считанной строки А делаем строку по блочным столбцам
void сreate_avec(int block, int block_arr[], const int arr[], int n)
{
	int ind_arr(0), ind_barr(0);
	for (int j = 0; j<n / block; j++)
	{
		for (int i = j*block; i<n; i++)
		{
			for (int s = 0; s<block; s++)
			{
				ind_arr = i*n + j*block + s;
				block_arr[ind_barr] = arr[ind_arr];
				ind_barr++;
			}
		}
	}
}

//Из считанной строки B делаем строку по блочным столбцам
void сreate_bvec(int block, int block_arr[], const int arr[], int n)
{
	int ind_arr(0), ind_barr(0);
	for (int j = 0; j<n / block; j++)
	{
		for (int i = 0; i<(j+1)*block; i++)
		{
			for (int s = 0; s<block; s++)
			{
				ind_arr = i*n + j*block + s;
				block_arr[ind_barr] = arr[ind_arr];
				ind_barr++;
			}
		}
	}
}

//Функция перемножения двух блоков (А и В)
void block_pred(int Block[], const int A[], const int B[], int b_s)
{
	int k, c;
	for (int i = 0; i < b_s; ++i)
	{
		for (int j = 0; j < b_s; ++j)
		{
			c = 0;
			for (k = 0; k < b_s; ++k)
			{				
				c += A[i*b_s + k]*B[k*b_s + j];
			}
			Block[i*b_s + j] = c;
		}
	}
}

//Функция перемножения двух блоков (А транспонированного и В)
//Используется , когда нужен блок, симметричный блоку А
void block_pred_trans(int Block[], const int A[], const int B[], int b_s)
{
	int k, c;
	for (int i = 0; i < b_s; ++i)
	{
		for (int j = 0; j < b_s; ++j)
		{
			c = 0;
			for (k = 0; k < b_s; ++k)
			{
				c += A[k*b_s + i] * B[k*b_s + j];
			}
			Block[i*b_s + j] = c;
		}
	}
}

//Функция вывода на экран результатирующей матрицы (хранится в блочном виде, по строкам)
void print_res(const int a[],int b_s, int n) //Вывод массива на экран
{
	int m = n / b_s;
	for (int i = 0; i < m; ++i)
		for (int ib(0); ib < b_s; ib++)
		{
			for (int j = 0; j < m; ++j)

				for (int jb(0); jb < b_s; jb++)
				{
					cout.width(5);
					cout << a[i*b_s*n + j*b_s*b_s + ib*b_s + jb];
				}
			cout << endl;
		}
}

//get_block вызывается для выделения памяти для блока и заполнения блока значениями из вектора
int* get_block(const int* a, int sz_b)
{
	int* res = new int[sz_b*sz_b];

	for (int i = 0; i < sz_b*sz_b; ++i)
		res[i] = a[i];

	return res;
}

//Во всех следующих функциях блочного перемножения сначала массивы arr_a и arr_b записываются в вектора a и b
//Для хранения в нижнетреугольном и верхнетреугольном виде соответственно, по блочным столбцам

//Функция блочного последовательного перемножения двух матриц
int* block_multiplicate(const int *arr_a, const int *arr_b,const int block_size)
{
	int *c = new int[N*N];
	int m = N / block_size; //Размерность блочной матрицы
	int size_a = ((m*m - m) / 2 + m)*block_size*block_size; //Кол-во эл-тов матрицы А 
	int *a = new int[size_a];
	сreate_avec(block_size, a, arr_a, N);

	int size_b = ((m*m - m) / 2 + m)*block_size*block_size;
	int *b = new int[size_b];
	сreate_bvec(block_size, b, arr_b, N);

	int *block_a = new int[block_size*block_size];
	int *block_b = new int[block_size*block_size];
	int *block = new int[block_size*block_size];
	int k;
	double t1, t2;
	t1 = omp_get_wtime();



	for (int i = 0; i < m; ++i)
		for (int j = 0; j < m; ++j)
		{
			int *block_c = c + i*m*block_size*block_size + j*block_size*block_size;
			for (int ib = 0; ib < block_size*block_size; ib++)
				block_c[ib] = 0;

			for (k = 0; k <= j; ++k)
			{
				int aind = (k < i) ? (2 * m*k - k*k - k) / 2 : (2 * m*i - i*i - i) / 2 + (k - i);
				int bind = (j*j + j) / 2 + k;
				for (int ib = 0; ib < block_size*block_size; ib++)
				{
					block_a[ib] = a[(i + aind)*block_size*block_size + ib];
					block_b[ib] = b[(bind)*block_size*block_size + ib];
				}
				if (k < i)
					block_pred(block, block_a, block_b, block_size);
				else
					block_pred_trans(block, block_a, block_b, block_size);				
				for (int ib = 0; ib < block_size*block_size; ib++)
					block_c[ib] += block[ib];
			}
			

		}
	delete[] block_a;
	delete[] block_b;
	delete[] block;

	/*cout << "\n ______________block_size  " << block_size << "__________________________ \n";
	print_res(c, block_size, N);
	cout << "\n _______________________________________________________ \n";*/

	t2 = omp_get_wtime();;


	FILE* file = fopen("time.txt", "a");
	fprintf(file, "%f\n", (t2 - t1));
	fclose(file);
	return c;
}

//Функция блочного параллельного внутри блока перемножения двух матриц
int* block_multiplicate_paral(const int *arr_a, const int *arr_b, const int block_size)
{
	int *c = new int[N*N];
	int m = N / block_size; //Размерность блочной матрицы
	int size_a = ((m*m - m) / 2 + m)*block_size*block_size; //Кол-во эл-тов матрицы А 
	int *a = new int[size_a];
	сreate_avec(block_size, a, arr_a, N);

	int size_b = ((m*m - m) / 2 + m)*block_size*block_size;
	int *b = new int[size_b];
	сreate_bvec(block_size, b, arr_b, N);

	int k;
	double t1, t2;
	t1 = omp_get_wtime();



	for (int i = 0; i < m; ++i)
		for (int j = 0; j < m; ++j)
		{
			int *block_c = c + i*m*block_size*block_size + j*block_size*block_size;
			for (int ib = 0; ib < block_size*block_size; ib++)
				block_c[ib] = 0;
#pragma omp parallel num_threads(8) 
			{
#pragma omp for  schedule(static)
				for (k = 0; k <= j; ++k)
				{
					int aind = (k < i) ? (2 * m*k - k*k - k) / 2 : (2 * m*i - i*i - i) / 2 + (k - i);
					int bind = (j*j + j) / 2 + k;
					int *block_a = a + (i + aind)*block_size*block_size;
					int *block_b = b + (bind)*block_size*block_size;
					int *block = new int[block_size*block_size];					
					if (k < i)
						block_pred(block, block_a, block_b, block_size);
					else
						block_pred_trans(block, block_a, block_b, block_size);					
					for (int ib = 0; ib < block_size*block_size; ib++)
						block_c[ib] += block[ib];
					delete[] block;
				}
			}
		}

	/*cout << "\n ______________block_size  " << block_size << "__________________________ \n";
	print_res(c, block_size, N);
	cout << "\n _______________________________________________________ \n";*/

	t2 = omp_get_wtime();;


	FILE* file = fopen("time_parallel.txt", "a");
	fprintf(file, "%f\n", (t2 - t1));
	fclose(file);
	return c;
}

//Функция блочного параллельного внутри блока перемножения двух матриц с выделением памяти для блоков
int* block_multiplicate_paral_divide(const int *arr_a, const int *arr_b, const int block_size)
{
	int *c = new int[N*N];
	int m = N / block_size; //Размерность блочной матрицы
	int size_a = ((m*m - m) / 2 + m)*block_size*block_size; //Кол-во эл-тов матрицы А 
	int *a = new int[size_a];
	сreate_avec(block_size, a, arr_a, N);
	
	int size_b = ((m*m - m) / 2 + m)*block_size*block_size;
	int *b = new int[size_b];
	сreate_bvec(block_size, b, arr_b, N);
	
	
	int k;
	double t1, t2;
	t1 = omp_get_wtime();



	for (int i = 0; i < m; ++i)
		for (int j = 0; j < m; ++j)
		{
			int *block_c = new int[block_size*block_size];
			for (int ib = 0; ib < block_size*block_size; ib++)
				block_c[ib] = 0;
#pragma omp parallel num_threads(8) 
			{
#pragma omp for  schedule(static)
				for (k = 0; k <= j; ++k)
				{
					int aind = (k < i) ? (2 * m*k - k*k - k) / 2 : (2 * m*i - i*i - i) / 2 + (k - i);
					int bind = (j*j + j) / 2 + k;
					int *block_a = get_block(a+ (i + aind)*block_size*block_size,block_size);
					int *block_b = get_block(b+ bind*block_size*block_size, block_size);
					int *block = new int[block_size*block_size];					
					
					if (k < i)
						block_pred(block, block_a, block_b, block_size);
					else
						block_pred_trans(block, block_a, block_b, block_size);					
					for (int ib = 0; ib < block_size*block_size; ib++)
						block_c[ib] += block[ib];
					delete[] block_a;
					delete[] block_b;
					delete[] block;
				}
			}
			for (int ib = 0; ib < block_size*block_size; ib++)
				c[(i*m + j)*block_size*block_size + ib] = block_c[ib];
			delete[] block_c;
		}

	/*cout << "\n ______________block_size  " << block_size << "__________________________ \n";
	print_res(c, block_size, N);
	cout << "\n _______________________________________________________ \n";*/

	t2 = omp_get_wtime();;


	FILE* file = fopen("time_parallel_divide.txt", "a");
	fprintf(file, "%f\n", (t2 - t1));
	fclose(file);
	return c;
}

//Функция блочного параллельного строк перемножения двух матриц
int* block_multiplicate_paral_str(const int *arr_a, const int *arr_b, const int block_size)
{
	int *c = new int[N*N];
	int m = N / block_size; //Размерность блочной матрицы
	int size_a = ((m*m - m) / 2 + m)*block_size*block_size; //Кол-во эл-тов матрицы А 
	int *a = new int[size_a];
	сreate_avec(block_size, a, arr_a, N);

	int size_b = ((m*m - m) / 2 + m)*block_size*block_size;
	int *b = new int[size_b];
	сreate_bvec(block_size, b, arr_b, N);

	int k;
	double t1, t2;
	t1 = omp_get_wtime();


#pragma omp parallel num_threads(8) 
	{
#pragma omp for  schedule(static)
		for (int i = 0; i < m; ++i)
			for (int j = 0; j < m; ++j)
			{
				int *block_c = c + i*m*block_size*block_size + j*block_size*block_size;
				for (int ib = 0; ib < block_size*block_size; ib++)
					block_c[ib] = 0;
				for (k = 0; k <= j; ++k)
				{
					int aind = (k < i) ? (2 * m*k - k*k - k) / 2 : (2 * m*i - i*i - i) / 2 + (k - i);
					int bind = (j*j + j) / 2 + k;
					int *block_a = a + (i + aind)*block_size*block_size;
					int *block_b = b + (bind)*block_size*block_size;
					int *block = new int[block_size*block_size];
					if (k < i)
						block_pred(block, block_a, block_b, block_size);
					else
						block_pred_trans(block, block_a, block_b, block_size);
					for (int ib = 0; ib < block_size*block_size; ib++)
						block_c[ib] += block[ib];
					delete[] block;
				}
			}
	}

	
	t2 = omp_get_wtime();;


	FILE* file = fopen("timeparallelstring.txt", "a");
	fprintf(file, "%f\n", (t2 - t1));
	fclose(file);
	return c;
}


int main()
{
	setlocale(LC_ALL, "rus");
	FILE *file_a = fopen("fileA.txt", "w");
	create_A_file(N, file_a);
	fclose(file_a);

	FILE *file_b = fopen("fileB.txt", "w");
	create_B_file(N, file_b);
	fclose(file_b);


	file_a = fopen("fileA.txt", "r"); //Загружаем матрицу из файла в массив
	int *arr_a = new int[N*N]; // указатель на массив (a-симметричная матрица,пока что НЕ в нижнетреугольном виде )
	file_to_arr(arr_a, N, file_a);
	fclose(file_a);
	//Тк в файле А записана в нижнетреугольном виде,
	make_symmetrical(arr_a, N);//достраиваем симметричную часть, чтоб при создании вектора блочных столбцов,
	//где А будет уже нижнетреугольной, учесть все элементы диагональных блоков

	//print_arr(arr_a, N);

	file_b = fopen("fileB.txt", "r"); //Загружаем матрицу из файла в массив
	int *arr_b = new int[N*N]; // указатель на массив (b- верхне-треугольная матрица, пока что с нулями)
	for (int i(0); i < N*N; i++)
		arr_b[i] = 0;
	file_to_arr(arr_b, N, file_b);
	fclose(file_b);
	//print_arr(arr_b, N);

	int *c = new int[N*N]; //Массив- результат для последоват. не блочного умножения

	//Найдем время неблочного умножения двух матриц
	double t_nonblock1, t_nonblock2;
	t_nonblock1 = omp_get_wtime();
	block_pred(c, arr_a, arr_b, N);
	t_nonblock2 = omp_get_wtime();
	FILE* file = fopen("time_non_block.txt", "a");
	fprintf(file, "%f\n", (t_nonblock2 - t_nonblock1));
	fclose(file);
	//print_arr(c, N);
	delete[] c;
	

	for (int block_size = 1; block_size <= N; block_size++)
	{
		if (N%block_size == 0)
		{
			
			
			int *c1 = block_multiplicate(arr_a, arr_b, block_size);
			delete[] c1;
			int *c2 = block_multiplicate_paral(arr_a, arr_b, block_size);
			delete[] c2;
			int *c3 = block_multiplicate_paral_divide(arr_a, arr_b, block_size);
			delete[] c3;
			int *c4 = block_multiplicate_paral_str(arr_a, arr_b, block_size);
			delete[] c4;
		
			FILE* file = fopen("block.txt", "a");
			fprintf(file, "%5d\n", (block_size));
			fclose(file);
		}

	}
	delete[] arr_a;
	delete[] arr_b;
	system("pause");
    return 0;
}

