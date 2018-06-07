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
		assert(newNodePos != Position::None);
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

	if(newNodePos)
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
		assert(nodePos != Position::None);

		const int prevNodePos = nodePos;
		nodePos = GetNodePosition(targetRect);
		assert(nodePos != Position::None);

		const int addPos = nodePos & ~prevNodePos;
		const int removePos = prevNodePos & ~nodePos;

		if (addPos) SwitchAdd(addPos, targetRect, targetID, players);

		if (removePos) SwitchRemove(removePos, targetID);
	}
	else
	{
		//assert(IDs.find(targetID) == IDs.end());
		AddID(targetRect,targetID, players);
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
		assert(IDs.at(targetID) != Position::None);
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
		assert(pos != 0);
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

void Node::SwitchAdd(int nodePos, RectI targetRect, int targetID, std::unordered_map<int, Player>& players)
{
	assert(nodePos != Position::None);
	if (nodePos & Position::TopLeft)
	{
		nodes[0].AddID(targetRect, targetID, players);
	}
	if (nodePos & Position::TopRight)
	{
		nodes[1].AddID(targetRect, targetID, players);
	}
	if (nodePos & Position::BottomLeft)
	{
		nodes[2].AddID(targetRect, targetID, players);
	}
	if (nodePos & Position::BottomRight)
	{
		nodes[3].AddID(targetRect, targetID, players);
	}
}

void Node::SwitchRemove(int nodePos, int targetID)
{
	assert(nodePos != Position::None);
	if (nodePos & Position::TopLeft)
	{
		nodes[0].RemoveID(targetID);
	}
	if (nodePos & Position::TopRight)
	{
		nodes[1].RemoveID(targetID);
	}
	if (nodePos & Position::BottomLeft)
	{
		nodes[2].RemoveID(targetID);
	}
	if (nodePos & Position::BottomRight)
	{
		nodes[3].RemoveID(targetID);
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
