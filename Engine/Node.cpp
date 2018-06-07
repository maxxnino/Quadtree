#include "Node.h"

void Node::Draw(Graphics & gfx) const
{
	if (!nodes.empty())
	{
		gfx.DrawLine(points[0], points[1], Colors::Green);
		gfx.DrawLine(points[2], points[3], Colors::Green);
		for (const auto& n : nodes)
		{
			n.Draw(gfx);
		}
	}
}

void Node::RebuildID(std::unordered_map<int, Player>& players)
{
	for (auto& i : IDs)
	{
		const RectI targetRect = players.at(i.first).GetRect();
		const int newNodePos = GetNodePosition(targetRect);
		assert(newNodePos);
		i.second = newNodePos;
		SwitchAdd(newNodePos, targetRect, i.first, players);
	}
}

void Node::AddID(RectI targetRect, int targetID, std::unordered_map<int, Player>& players)
{
	const int newNodePos = GetNodePosition(targetRect);
	IDs.emplace(targetID, newNodePos);
	if (Split())
	{
		RebuildID(players);
		return;
	}
	
	if(newNodePos != 0)
	{
		assert(!nodes.empty());
		SwitchAdd(newNodePos, targetRect, targetID, players);
	}
}

void Node::Update(RectI targetRect,int targetID, std::unordered_map<int, Player>& players)
{
	if (!nodes.empty())
	{
		auto& nodePos = IDs.at(targetID);
		assert(nodePos);

		const int prevNodePos = nodePos;
		nodePos = GetNodePosition(targetRect);
		assert(nodePos);

		const int middle = nodePos & prevNodePos;
		if (middle) SwitchUpdate(middle, targetRect, targetID, players);

		const int addPos = nodePos & ~prevNodePos;
		if (addPos) SwitchAdd(addPos, targetRect, targetID, players);

		const int removePos = prevNodePos & ~nodePos;
		if (removePos) SwitchRemove(removePos, targetID);
	}
}

void Node::RemoveID(int targetID)
{
	if (nodes.empty())
	{
		IDs.erase(targetID);
	}
	else
	{
		assert(IDs.at(targetID));
		SwitchRemove(IDs.at(targetID), targetID);
		IDs.erase(targetID);
		if (IDs.size() < 3)
		{
			nodes.clear();
			for (auto& i : IDs)
			{
				i.second = Position::None;
			}
		}
	}
}

int Node::GetNodePosition(RectI targetRect)
{
	int pos = Position::None;
	if (!nodes.empty())
	{
		if (nodes[0].GetRect().isOverLap(targetRect))
		{
			pos |= Position::TopLeft;
		}
		if (nodes[1].GetRect().isOverLap(targetRect))
		{
			pos |= Position::TopRight;
		}
		if (nodes[2].GetRect().isOverLap(targetRect))
		{
			pos |= Position::BottomLeft;
		}
		if (nodes[3].GetRect().isOverLap(targetRect))
		{
			pos |= Position::BottomRight;
		}
		assert(pos);
	}
	return pos;
}


void Node::UpdateCollision(std::unordered_map<int, Player>& players)
{
	if (!nodes.empty())
	{
		for (auto& n : nodes)
		{
			n.UpdateCollision(players);
		}
	}
	else
	{
		if (IDs.size() > 1)
		{
			for (auto it = IDs.begin(); it != IDs.end(); it++)
			{
				auto au = it;
				au++;
				for (; au != IDs.end(); au++)
				{
					auto& p1 = players.at(it->first);
					auto& p2 = players.at(au->first);
					if (p1.GetRect().isOverLap(p2.GetRect()))
					{
						p1.Collision();
						p2.Collision();
					}
				}
			}
		}
	}
}

void Node::SwitchAdd(int nodePos, RectI targetRect, int targetID, std::unordered_map<int, Player>& players)
{
	assert(nodePos);
	for (int i = 0; i < 4; i++)
	{
		if (nodePos & (1 << i))
		{
			nodes[i].AddID(targetRect, targetID, players);
		}
	}
}

void Node::SwitchRemove(int nodePos, int targetID)
{
	assert(nodePos);
	for (int i = 0; i < 4; i++)
	{
		if (nodePos & (1 << i))
		{
			nodes[i].RemoveID(targetID);
		}
	}
}

inline void Node::SwitchUpdate(int nodePos, RectI targetRect, int targetID, std::unordered_map<int, Player>& players)
{
	assert(nodePos);
	for (int i = 0; i < 4; i++)
	{
		if (nodePos & (1 << i))
		{
			nodes[i].Update(targetRect, targetID, players);
		}
	}
}

bool Node::Split()
{
	const int newDepth = depth + 1;
	if (newDepth < maxDepth)
	{
		if (IDs.size() > 2 && nodes.empty())
		{
			nodes.emplace_back(Node({ rect.left,rect.top,points[0].x,points[2].y }, newDepth));
			nodes.emplace_back(Node({ points[0].x,rect.top,rect.right,points[2].y }, newDepth));
			nodes.emplace_back(Node({ rect.left,points[2].y,points[0].x,rect.bottom }, newDepth));
			nodes.emplace_back(Node({ points[0].x,points[2].y,rect.right,rect.bottom }, newDepth));
			return true;
		}
	}
	return false;
}
