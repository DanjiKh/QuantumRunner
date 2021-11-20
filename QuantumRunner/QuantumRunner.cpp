﻿#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include<iostream>

using namespace std;

struct AnimDecal {
    olc::Decal* img;    //  Переменная типа Декаль. 
    olc::vf2d pos;      //  Вектор с коорнидатами.
    olc::vf2d size;     //  Вектор с  размером.
    int max_frame;      //  Макс. кол-во кадров.
    float fps;          //  Количество кадров в секунду.
    olc::vf2d _shift;   //  Смещение.

    int frame = 0;                      //  Счетчик кадров.
    olc::vf2d scale = { 2.0f, 2.0f };   //  Масштаб вектора.

    float _currFps = 0;                 //  Текущий фпс.
    olc::vf2d _currFrame = { 0, 0 };    //  Текущий кадр.

    void update(float deltaT) {                 //  Фунуция обновления кадров.
        _currFrame = size * _shift * frame; //  Расчёт текущего кадра. 
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
private:
    //Game Character
    olc::Decal* StandChar;   //  Декаль двигающейся картинки
    olc::Decal* WalkChar;    //  Декаль статической картинки

    AnimDecal w_ch;        //  Структура типа персонажа

    olc::vf2d m_c_p;        //  Изначальные координаты игрового персонажа.
    olc::vf2d m_c_s = { 124, 124 };
        
public:
	QuantumRunner()
	{
		sAppName = "QuantumRunner";	//	Название проэкта.
	}

public:
    void movingChar()
    {
        if (GetKey(olc::Key::W).bHeld and m_c_p.y >= 0)
            m_c_p.y -= 0.2;
        if (GetKey(olc::Key::S).bHeld)
            m_c_p.y += 0.2;
        if (GetKey(olc::Key::A).bHeld and m_c_p.x >= 0)
            m_c_p.x -= 0.2;
        if (GetKey(olc::Key::D).bHeld)
            m_c_p.x += 0.2;

        if (m_c_p.x >= ScreenWidth() - m_c_s.x)
            m_c_p.x = ScreenWidth() - m_c_s.x;
        if (m_c_p.y >= ScreenHeight() - m_c_s.y)
            m_c_p.y = ScreenHeight() - m_c_s.y;

    }

    void drawAnimDecal(AnimDecal& p, olc::vf2d pos, float deltaT)                          //  Функция отрисовки анимированной декали
    {
        p.update(deltaT);                                                   //  Обновление кадров
        DrawPartialDecal(pos, p.img, p._currFrame, p.size, p.scale);      //  Ортисовка анимированной декали
    }

	bool OnUserCreate() override
	{
        auto temp = new olc::Sprite("./StandingCop.png");           //  Картинка игрового персонажа
        StandChar = new olc::Decal(temp);

        temp = new olc::Sprite("./WalkingCop.png");                 //  Анимированная картинка игрового персонажа
        WalkChar = new olc::Decal(temp);

        w_ch = {
            WalkChar,
            m_c_p,
            olc::vf2d(62, 67),
            8,
            0.17,
            olc::vf2d(1,0)
        };                                                          // Основные данные игрового персонажа

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        movingChar();
        if (GetKey(olc::Key::W).bHeld or GetKey(olc::Key::S).bHeld or GetKey(olc::Key::A).bHeld or GetKey(olc::Key::D).bHeld)
            drawAnimDecal(w_ch, m_c_p, fElapsedTime);
        else
            DrawDecal(m_c_p, StandChar, olc::vf2d(2.0f, 2.0f));

		return true;
	}
};

int main()
{
	QuantumRunner demo;
	if (demo.Construct(720, 560, 2, 2))
		demo.Start();
	return 0;
}


