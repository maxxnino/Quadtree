#pragma once
#include "Rect.h"
#include "Graphics.h"
class Player
{
public:
	Player(VecF pos, VecF vel)
		:
		pos(pos),
		vel(vel)
	{}
	void Draw(Graphics& gfx) const
	{
		if (isCollied)
		{
			gfx.DrawRectDim(pos, size, size, Colors::Red);
		}
		else
		{
			gfx.DrawRectDim(pos, size, size, Colors::Blue);
		}
	}
	void Update(float dt)
	{
		pos += vel * dt;
		if (pos.x < 0.0f)
		{
			pos.x = 0.0f;
			vel.x = -vel.x;
		}
		else if (pos.x + size >= Graphics::ScreenWidth)
		{
			pos.x = Graphics::ScreenWidth - size;
			vel.x = -vel.x;
		}

		if (pos.y < 0.0f)
		{
			pos.y = 0.0f;
			vel.y = -vel.y;
		}
		else if (pos.y + size >= Graphics::ScreenHeight)
		{
			pos.y = Graphics::ScreenHeight - size;
			vel.y = -vel.y;
		}
		isCollied = false;
	}
	void Collision()
	{
		isCollied = true;
	}
	RectI GetRect() const
	{
		return { (int)pos.x , (int)pos.y ,(int)pos.x + size, (int)pos.y + size };
	}
	const inline VecI GetPos() const
	{
		return { (int)pos.x,(int)pos.y };
	}
private:
	VecF pos;
	VecF vel;
	static constexpr int size = 10;
	bool isCollied = false;
};