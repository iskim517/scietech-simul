#include <cstdio>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "dijkstra.h"
using namespace std;

vector<pair<int,int>> graph[10];

/* Example graph of:
 https://upload.wikimedia.org/wikipedia/commons/5/57/Dijkstra_Animation.gif
*/

int main()
{
	graph[1] = {{2, 7}, {3, 9}, {6, 14}};
	graph[2] = {{1, 7}, {3, 10}, {4, 15}};
	graph[3] = {{1, 9}, {2, 10}, {4, 11}, {6, 2}};
	graph[4] = {{2, 15}, {3, 11}, {5, 6}};
	graph[5] = {{4, 6}, {6, 9}};
	graph[6] = {{1, 14}, {3, 2}, {5, 9}};

	auto ret = dijkstra(graph, 7, 1);

	for (int i = 1; i <= 6; i++)
	{
		printf("%d\n", ret[i]);
	}
}
