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
	{
		rect.left = (int)pos.x;
		rect.top = (int)pos.y;
		rect.right = rect.left + size;
		rect.bottom = rect.top + size;
	}
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
	inline void UpdateRect()
	{
		rect.left = (int)pos.x;
		rect.top = (int)pos.y;
		rect.right = rect.left + size;
		rect.bottom = rect.top + size;
	}
	void Update(float dt)
	{
		pos += vel * dt;
		UpdateRect();
	}
	void ClamScreen()
	{
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
		UpdateRect();
	}
	void CorrectCollision(Player& otherBox)
	{
		VecF velNormal = vel - otherBox.vel;
		if (velNormal.x > 0.0f)
		{
			float px = rect.right - otherBox.rect.left;
			if (velNormal.y > 0.0f)
			{
				float py = rect.bottom - otherBox.rect.top;
				//px/vel.x > py/vel.y
				//top collision
				if (abs(velNormal.y) * px > velNormal.x * py)
				{
					pos.y -= py;
					//otherBox.pos.y -= py * (otherBox.vel.y - velNormal.y) / velNormal.y;
					vel.y -= velNormal.y;
					otherBox.vel.y += velNormal.y;
				}
				//left collision
				else
				{
					pos.x -= px;
					//otherBox.pos.x -= px * (otherBox.vel.x - velNormal.x) / velNormal.x;
					vel.x -= velNormal.x;
					otherBox.vel.x += velNormal.x;
				}
			}
			else
			{
				float py = otherBox.rect.bottom - rect.top;
				//px/vel.x > py/vel.y
				//bottom collision
				if (abs(velNormal.y) * px > abs(velNormal.x) * py)
				{
					pos.y += py;
					//otherBox.pos.y += py * (otherBox.vel.y - velNormal.y) / velNormal.y;
					vel.y -= velNormal.y;
					otherBox.vel.y += velNormal.y;
				}
				//left collision
				else
				{
					pos.x -= px;
					//otherBox.pos.x -= px * (otherBox.vel.x - velNormal.x) / velNormal.x;
					vel.x -= velNormal.x;
					otherBox.vel.x += velNormal.x;
				}
			}
		}
		else
		{
			float px = otherBox.rect.right - rect.left;
			if (velNormal.y > 0.0f)
			{
				float py = rect.bottom - otherBox.rect.top;
				//px/vel.x > py/vel.y
				//top collision
				if (abs(velNormal.y) * px > abs(velNormal.x) * py)
				{
					pos.y -= py;
					//otherBox.pos.y -= py * (otherBox.vel.y - velNormal.y) / velNormal.y;
					vel.y -= velNormal.y;
					otherBox.vel.y += velNormal.y;
				}
				//right collision
				else
				{
					pos.x += px;
					//otherBox.pos.x += px * (otherBox.vel.x - velNormal.x) / velNormal.x;
					vel.x -= velNormal.x;
					otherBox.vel.x += velNormal.x;
				}
			}
			else
			{
				float py = otherBox.rect.bottom - rect.top;
				//px/vel.x > py/vel.y
				//bottom collision
				if (abs(velNormal.y) * px > abs(velNormal.x) * py)
				{
					pos.y += py;
					//otherBox.pos.y += py * (otherBox.vel.y - velNormal.y) / velNormal.y;
					vel.y -= velNormal.y;
					otherBox.vel.y += velNormal.y;
				}
				//right collision
				else
				{
					pos.x += px;
					//otherBox.pos.x += px * (otherBox.vel.x - velNormal.x) / velNormal.x;
					vel.x -= velNormal.x;
					otherBox.vel.x += velNormal.x;
				}
			}
		}
		UpdateRect();
		otherBox.UpdateRect();
	}
	RectI GetRect() const
	{
		return rect;
	}
	const inline VecI GetPos() const
	{
		return { (int)pos.x,(int)pos.y };
	}

	VecF pos;
	VecF vel;
	RectI rect;
private:
	static constexpr int size = 20;
	bool isCollied = false;
};