#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include<iostream>

using namespace std;

struct AnimHorizDecal {
    olc::Decal* img;    //  Переменная типа Декаль 
    olc::vf2d pos;      //  Вектор с коорнидатами
    olc::vf2d size;     //  Вектор с  размером
    int max_frame;      //  Макс. кол-во кадров
    float fps;          //  Количество кадров в секунду

    int frame = 0;                      //  Счетчик кадров
    olc::vf2d scale = { 2.0f, 2.0f };   //  Масштаб вектора

    float _currFps = 0;                 //  Текущий фпс
    olc::vf2d _currFrame = { 0, 0 };    //  Текущий кадр 

    olc::vf2d _shift = { 1, 0 };        //  Смещение 

    void update(float deltaT) {                 //  Фунуция обновления кадров
        _currFrame = size * _shift * frame; //  Расчёт текущего кадра 
        if (frame >= max_frame)
            frame = 0;
        if (_currFps > fps) {
            _currFps = 0;
            frame++;
        }
        _currFps += deltaT;
    }
};

class QuantumRunner : public olc::PixelGameEngine
{
public:
	QuantumRunner()
	{
		
		sAppName = "QuantumRunner";	//	Название проэкта.
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


