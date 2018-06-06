#pragma once
#include "Node.h"
class QuadTree
{
public:
	QuadTree(RectI rect)
		:
		node(rect, 0)
	{}
	void Draw(Graphics& gfx) const
	{
		node.Draw(gfx);
	}
	void AddTarget(VecI Pos, int ID, std::unordered_map<int, Player>& players)
	{
		node.AddID(Pos, ID, players);
	}
	void UpdateTarget(VecI Pos, int ID, std::unordered_map<int, Player>& players)
	{
		node.Update(Pos, ID, players);
	}
	void Update(std::unordered_map<int, Player>& players)
	{
		node.UpdateCollision(players);
	}
	void Clear()
	{
		node.Clear();
	}
private:
	Node node;
};