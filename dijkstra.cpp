#include <queue>
#include <tuple>
#include <algorithm>
#include "dijkstra.h"
using namespace std;

vector<int> dijkstra(const vector<pair<int,int>> graph[], int size, int start)
{
	vector<int> weight(size, 1e9);
	weight[start] = 0;

	priority_queue<pair<int,int>> pq;
	pq.emplace(0, start);

	while (pq.empty() == false)
	{
		int v; int w;
		w = -pq.top().first;
		v = pq.top().second;
		pq.pop();

		if (weight[v] < w) continue;

		for (auto &&edge : graph[v])
		{
			int nv = edge.first;
			int nw = w + edge.second;

			if (weight[nv] > nw)
			{
				weight[nv] = nw;
				pq.emplace(-nw, nv);
			}
		}
	}

	return weight;
}
