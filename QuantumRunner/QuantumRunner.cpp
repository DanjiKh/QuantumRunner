// Запуск программы : CTRL + F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include<iostream>

using namespace std;

class QuantumRunner : public olc::PixelGameEngine
{
public:
	QuantumRunner()
	{
		
		sAppName = "QuantumRunner";
	}

public:
	bool OnUserCreate() override
	{
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		return true;
	}
};

int main()
{
	QuantumRunner demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}


