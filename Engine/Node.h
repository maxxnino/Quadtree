#pragma once
#include <vector>
#include <unordered_map>
#include <assert.h>
#include "Graphics.h"
#include "Rect.h"
#include "Player.h"
class Node
{
public:
	Node(RectI rect, int depth)
		:
		rect(rect),
		depth(depth)
	{
		points.emplace_back(VecI((rect.left + rect.right) / 2, rect.top));
		points.emplace_back(VecI((rect.left + rect.right) / 2, rect.bottom));
		points.emplace_back(VecI(rect.left, (rect.top + rect.bottom) / 2));
		points.emplace_back(VecI(rect.right, (rect.top + rect.bottom) / 2));
	}
	void Draw(Graphics& gfx) const;
	void AddID(VecI pos, int targetID, std::unordered_map<int,Player>& players);
	void Update(VecI pos, int targetID, std::unordered_map<int,Player>& players);
	void RemoveID(int targetID);
	void Clear()
	{
		nodes.clear();
		IDs.clear();
	}
	void UpdateCollision(std::unordered_map<int, Player>& players);

private:

	bool Split();
	void RebuildID(std::unordered_map<int, Player>& players);
	int GetNodePosition(VecI pos);
private:
	std::vector<Node> nodes;
	std::unordered_map<int,int> IDs;
	static constexpr int maxDepth = 5;
	int depth = -1;
	RectI rect;
	std::vector<VecI> points;
};