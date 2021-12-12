﻿#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include<iostream>

using namespace std;

struct AnimDecal {
    uint64_t layer;     //  Слой на находиться декаль.
    uint64_t id;        //  Уникальный Id декали.
    olc::Decal* img;    //  Переменная типа Картинка. 
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

struct Button {
    uint64_t layer;             //  Слой на котором находиться кнопка.
    uint64_t id;                //  Уникальный Id кнопки.
    olc::vf2d pos;              //  Координаты кнопки.
    olc::vf2d size;             //  Размер кнопки.
    std::string text;           //  Текст на кнопке.
    olc::Pixel color;           //  Цвет кнопки.
    olc::Pixel text_color;      //  Цвет текста на кнопке.
    olc::Decal* img;            //  Картинка кнопки.

    bool check(olc::vf2d mouse) {
        auto p = mouse - pos;
        return p.x > 0 
            && p.x < size.x
            && p.y > 0
            && p.y < size.x;    //  Проверка мыши относительно кнопки.
    };
};

class QuantumRunner : public olc::PixelGameEngine
{
private:
    std::vector<Button> m_baton;
    uint64_t m_layer = 0;

    std::vector<AnimDecal> a_decals;
    uint64_t a_layer = 0;

    //Game Character
    olc::Decal* StandChar;   //  Декаль двигающейся картинки.
    olc::Decal* WalkChar;    //  Декаль статической картинки.

    AnimDecal w_ch;        //  Структура типа персонажаs.

    olc::vf2d m_c_p;        //  Изначальные координаты игрового персонажа.
    olc::vf2d m_c_s = { 124, 124 };
        
public:
	QuantumRunner()
	{
		sAppName = "QuantumRunner";	//	Название проэкта.
	}

public:
    olc::vf2d movingChar()
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
        return m_c_p;
    }

    /*void drawAnimDecal(AnimDecal& p, olc::vf2d pos, float deltaT)               //  Функция отрисовки анимированной декали.
    {
        p.update(deltaT);                                                   //  Обновление кадров.
        DrawPartialDecal(pos, p.img, p._currFrame, p.size, p.scale);        //  Ортисовка анимированной декали.
    }*/

    void DrawAnimDecal(float deltaT) {
        for (auto& i : a_decals) {
            if (i.layer == a_layer)
            {
                i.update(deltaT);
                DrawPartialDecal(m_c_p, i.img, i._currFrame, i.size, i.scale);
            }
        }
    }

    void UpdateButtons() {
        for (auto& i : m_baton) {
            if (i.layer == m_layer) {
                switch (i.id)
                {
                    case 1: {
                        if (i.check(GetMousePos()) && GetMouse(0).bReleased) {
                            m_layer = 1;
                        }
                        break;
                    }
                    
                    case 2: {
                        if (i.check(GetMousePos()) && GetMouse(0).bReleased) {
                            olc_Terminate();
                        }
                        break;
                    }
                    

                }
            };
        }
    }

    void DrawButton()                                                           //  Функция отрисовки кнопок.
    {
        for (auto& i : m_baton) {
            if (i.layer == m_layer) {
                FillRect(i.pos, i.size, i.color);
                DrawStringDecal(i.pos + i.size / 2 - GetTextSize(i.text) / 2, i.text, i.text_color);
            };
        };
    }

	bool OnUserCreate() override
	{
        auto temp = new olc::Sprite("./StandingCop.png");           //  Картинка игрового персонажа.
        StandChar = new olc::Decal(temp);

        temp = new olc::Sprite("./WalkingCop.png");                 //  Анимированная картинка игрового персонажа.
        WalkChar = new olc::Decal(temp);

        a_decals.push_back(
            AnimDecal{
                1,
                1,
                WalkChar,
                m_c_p,
                olc::vf2d(62, 67),
                8,
                0.17,
                olc::vf2d(1,0)
            });

        m_baton.push_back(
            Button{
                0,
                1,
                {200, 100},
                {100, 50},
                "Play",
                olc::Pixel(255, 255, 255),
                olc::Pixel(0, 0, 0),
                nullptr
            });

        m_baton.push_back(
            Button{
                0,
                2,
                {200, 200},
                {100, 50},
                "Quit",
                olc::Pixel(255, 255, 255),
                olc::Pixel(0, 0, 0),
                nullptr
            });

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        Clear(olc::Pixel());

        UpdateButtons();
        DrawButton();

        olc::vf2d p1 = movingChar();
        olc::vf2d p2 = GetMousePos();
        olc::vf2d n = (p2 - p1).norm();
        olc::vf2d max_len = GetWindowSize();

        for (auto j = 0.0; (p1 - n * j).mag() < max_len.mag(); j += 1)
            Draw(p1 + n * j, olc::GREEN);

        if (GetKey(olc::Key::W).bHeld || GetKey(olc::Key::S).bHeld || GetKey(olc::Key::A).bHeld || GetKey(olc::Key::D).bHeld)
            DrawAnimDecal(fElapsedTime);
        else
            DrawDecal(p1, StandChar, olc::vf2d(2.0f, 2.0f));

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