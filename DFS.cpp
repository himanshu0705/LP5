#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

const int MAX = 100000;
vector <int> graph[MAX];
bool visited[MAX];

void dfs(int node)
{
    stack <int> s;
    s.push(node);
    while(!s.empty())
    {
        int currNode = s.top();
        s.pop();
        if(!visited[currNode])
        {
            visited[currNode] = true;
            cout<<"\t"<<currNode;
            for(int i = graph[currNode].size()-1; i>=0; i--)
            {
                int adjNode = graph[currNode][i];
                if(!visited[adjNode])
                {
                    s.push(adjNode);
                }
            }
        }
    }
}

int main()
{
    int n,m,start;
    cout<<"\nEnter number of Nodes, Edges & starting node : \n";
    cin>>n>>m>>start;
    cout<<"\nEnter pair of nodes representing edges : \n";
    for(int i=0; i<m; i++)
    {
        int u,v;
        cin>>u>>v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    #pragma omp parallel for
    for(int i=0; i<n; i++)
    {
        visited[i] = false;
    }
    dfs(start);
    return 0;
}