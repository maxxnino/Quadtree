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
		const VecI pos = players.at(i.first).GetPos();
		const Position nodeID = GetNodePosition(pos);
		assert(nodeID != Position::None);
		i.second = nodeID;
		nodes[nodeID].AddID(pos, i.first, players);
	}
}

void Node::AddID(VecI pos, int targetID, std::unordered_map<int, Player>& players)
{
	const Position nodeID = GetNodePosition(pos);
	IDs.emplace(targetID, nodeID);
	if (Split())
	{
		RebuildID(players);
	}
	else if(!nodes.empty())
	{
		assert(nodeID != Position::None);
		nodes[nodeID].AddID(pos, targetID, players);
	}
}

void Node::Update(VecI pos, int targetID, std::unordered_map<int, Player>& players)
{
	if (!nodes.empty())
	{
		const Position nodeID = GetNodePosition(pos);
		assert(nodeID != Position::None);
		auto& id = IDs.at(targetID);
		if (id == Position::None)
		{
			id = nodeID;
			nodes[nodeID].AddID(pos, targetID, players);
		}
		else
		{
			nodes[id].RemoveID(targetID);
			id = nodeID;
			nodes[nodeID].AddID(pos, targetID, players);
		}
	}
	else
	{
		auto id = IDs.find(targetID);
		if (id == IDs.end())
		{
			AddID(pos, targetID, players);
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
		nodes[id].RemoveID(targetID);
		IDs.erase(targetID);
		if (IDs.size() < 2)
		{
			nodes.clear();
			for (auto& i : IDs)
			{
				i.second = Position::None;
			}
		}
	}
}

Node::Position Node::GetNodePosition(VecI pos)
{
	if (!nodes.empty())
	{
		if (pos.x < (rect.left + rect.right) / 2)
		{
			if (pos.y < (rect.top + rect.bottom) / 2)
			{
				return Position::TopLeft;
			}
			else
			{
				return Position::BottomLeft;
			}
		}
		else
		{
			if (pos.y < (rect.top + rect.bottom) / 2)
			{
				return Position::TopRight;
			}
			else
			{
				return Position::BottomRight;
			}
		}
	}
	return Position::None;
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
