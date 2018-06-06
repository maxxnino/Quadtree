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
	enum Position
	{
		TopLeft = 1,
		TopRight = 2,
		BottomLeft = 4,
		BottomRight = 8,
		None = 0
	};
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
	void AddID(RectI targetRect, int targetID, std::unordered_map<int,Player>& players);
	void Update(RectI targetRect, int targetID, std::unordered_map<int,Player>& players);
	void RemoveID(int targetID);
	void Clear()
	{
		nodes.clear();
		IDs.clear();
	}
	void UpdateCollision(std::unordered_map<int, Player>& players);

	void SwitchAdd(int nodeID, RectI targetRect, int targetID, std::unordered_map<int, Player>& players);
	void SwitchRemove(int nodeID, int targetID);
	RectI GetRect() const
	{
		return rect;
	}
private:
	bool Split();
	void RebuildID(std::unordered_map<int, Player>& players);
	int GetNodePosition(RectI targetRect);
private:
	std::vector<Node> nodes;
	std::unordered_map<int,int> IDs;
	static constexpr int maxDepth = 6;
	int depth;
	RectI rect;
	std::vector<VecI> points;
};