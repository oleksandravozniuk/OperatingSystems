// lab6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include<stdio.h>
#include <string>
#include <dos.h>
#include <windows.h>

using namespace std;

void Draw(void)
{
	for (int i = 0; i < 100; i++)
	{
		//process of drawing
		Sleep(1);
	}

	return;
}


int main(void)
{
	printf("\n Inside main()\n");
	
	int i = 0;

	for (; i < 0xffffff; i++);
	
		Draw();
		

	return 0;
}