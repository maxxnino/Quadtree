/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	quadTree({ 0,0,Graphics::ScreenWidth,Graphics::ScreenHeight })
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();
	for (auto& p : players)
	{
		if (isUpdate)
		{
			p.second.Update(dt);
			quadTree.UpdateTarget(p.second.GetRect(),p.first, players);
		}
		
	}
	while (!wnd.mouse.IsEmpty())
	{
		auto e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			std::uniform_int_distribution<int> xPos(0, Graphics::ScreenWidth - 20);
			std::uniform_int_distribution<int> yPos(0, Graphics::ScreenHeight - 20);
			std::uniform_int_distribution<int> speed(50, 100);
			std::uniform_int_distribution<int> dir(0, 1);
			int dirX = 1;
			int dirY = 1;
			for (size_t i = 0; i < 10; i++)
			{
				if (dir(rng) == 1)
				{
					dirX = -1;
				}
				if (dir(rng) == 1)
				{
					dirY = -1;
				}
				players.emplace(curID, Player({ (float)xPos(rng),(float)yPos(rng) }, { (float)(dirX * speed(rng)), (float)(dirY * speed(rng)) }));
				quadTree.AddTarget(players.at(curID).GetRect(),curID, players);
				curID += 1;
			}
		}
		if (e.GetType() == Mouse::Event::Type::RPress)
		{
			isUpdate = !isUpdate;
		}
	}
	
	while (!wnd.kbd.KeyIsEmpty())
	{
		auto e = wnd.kbd.ReadKey();
		if (e.GetCode() == 'A' && e.IsPress())
		{
			quadTree.Clear();
			players.clear();
		}
		if (e.GetCode() == 'S' && e.IsPress())
		{
			if (curID > 0)
			{
				curID--;
				quadTree.Remove(curID);
				players.erase(curID);
			}
		}
	}
	
	quadTree.Update(players);
	for (auto& p : players)
	{
		p.second.ClamScreen();
	}
}

void Game::ComposeFrame()
{
	quadTree.Draw(gfx);
	for (const auto& p : players)
	{
		p.second.Draw(gfx);
	}
}
