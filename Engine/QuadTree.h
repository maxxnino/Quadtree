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
	void AddTarget(RectI rect, int ID, std::unordered_map<int, Player>& players)
	{
		node.AddID(rect,ID, players);
	}
	void UpdateTarget(RectI rect, int ID, std::unordered_map<int, Player>& players)
	{
		node.Update(rect,ID, players);
	}
	void Update(std::unordered_map<int, Player>& players)
	{
		node.UpdateCollision(players);
	}
	void Clear()
	{
		node.Clear();
	}
	void Remove(int ID)
	{
		node.RemoveID(ID);
	}
private:
	Node node;
};