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
		const int nodeID = GetNodePosition(targetRect);
		assert(nodeID != Position::None);
		i.second = nodeID;
		SwitchAdd(nodeID, targetRect, i.first, players);
	}
}

void Node::AddID(RectI targetRect, int targetID, std::unordered_map<int, Player>& players)
{
	const int nodeID = GetNodePosition(targetRect);
	IDs.emplace(targetID, nodeID);
	if (Split())
	{
		RebuildID(players);
	}
	else if(!nodes.empty())
	{
		assert(nodeID != Position::None);
		SwitchAdd(nodeID, targetRect, targetID, players);
	}
}

void Node::Update(RectI targetRect,int targetID, std::unordered_map<int, Player>& players)
{
	if (!nodes.empty())
	{
		const int nodeID = GetNodePosition(targetRect);
		assert(nodeID != Position::None);
		auto& id = IDs.at(targetID);
		if (id == Position::None)
		{
			id = nodeID;
			SwitchAdd(nodeID, targetRect, targetID, players);
		}
		else
		{
			int middle = id & nodeID;
			int removeID = id ^ middle;
			int addID = nodeID ^ middle;
			if (removeID)
			{
				SwitchRemove(removeID, targetID);
			}
			
			if (addID)
			{
				SwitchAdd(addID, targetRect, targetID, players);
			}
			id = nodeID;
		}
	}
	else
	{
		auto id = IDs.find(targetID);
		if (id == IDs.end())
		{
			AddID(targetRect,targetID, players);
		}
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
		auto& id = IDs.at(targetID);
		assert(id != Position::None);
		SwitchRemove(id, targetID);
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
			auto it = IDs.begin();
			for (size_t i = 0; i < IDs.size() - 1; i++)
			{
				auto& p1 = players.at(it->first);
				it++;
				auto& p2 = players.at(it->first);
				if (p1.GetRect().isOverLap(p2.GetRect()))
				{
					p1.Collision();
					p2.Collision();
				}
			}
		}
	}
}

void Node::SwitchAdd(int nodeID, RectI targetRect, int targetID, std::unordered_map<int, Player>& players)
{
	assert(nodeID != Position::None);
	for (int i = 0; i < 4; i++)
	{
		if (nodeID & (1 << i))
		{
			nodes[i].AddID(targetRect, targetID, players);
		}
	}
}

void Node::SwitchRemove(int nodeID, int targetID)
{
	assert(nodeID != Position::None);
	for (int i = 0; i < 4; i++)
	{
		if (nodeID & (1 << i))
		{
			nodes[i].RemoveID(targetID);
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
