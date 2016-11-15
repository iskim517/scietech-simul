#pragma once

#include <vector>
#include <functional>
#include <set>
#include <limits>
#include <utility>

template <typename Node, typename Weight>
std::vector<Weight> dijkstra(
	const std::vector<Node> &nodes,
	std::function<std::pair<bool, Weight>(const Node &, const Node &)> distFunc,
	int start)
{
	std::vector<char> visit(nodes.size());
	std::vector<Weight> weight(nodes.size(), std::numeric_limits<Weight>::max());

	weight[start] = 0;

	for (int i = 0; i < nodes.size(); i++)
	{
		int sel = -1;
		for (int j = 0; j < nodes.size(); j++)
		{
			if (visit[j]) continue;
			if (sel == -1 || weight[sel] > weight[j]) sel = j;
		}

		visit[sel] = 1;

		for (int j = 0; j < nodes.size(); j++)
		{
			if (visit[j]) continue;
			auto dist = distFunc(nodes[i], nodes[j]);
			if (dist.first == false) continue;

			if (weight[j] > weight[sel] + dist.second)
				weight[j] = weight[sel] + dist.second;
		}
	}

	return weight;
}
