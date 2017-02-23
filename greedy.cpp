#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

struct connection
{
    int cache, latency;
} ;

struct request
{
    int from, video, count;
} ;

struct endpoint
{
    int base_latency;
    vector<connection> conn;
    vector<request> requests;
} ;

int n_videos, n_endpoints, n_caches, cache_size;
vector<int> video_size;
vector<endpoint> endpoints;
vector<request> requests;

int main()
{
    int n_requests;
    scanf("%d %d %d %d %d", &n_videos, &n_endpoints, &n_requests, &n_caches, &cache_size);

    for(int i = 0; i < n_videos; i++)
    {
	int size;
	scanf("%d", &size);
	video_size.push_back(size);
    }

    for(int i = 0; i < n_endpoints; i++)
    {
	endpoint e;
	int n;
	scanf("%d %d", &e.base_latency, &n);
	while(n--)
	{
	    connection c;
	    scanf("%d %d", &c.cache, &c.latency);
	    e.conn.push_back(c);
	}
	endpoints.push_back(e);
    }

    for(int i = 0; i < n_requests; i++)
    {
	request r;
	scanf("%d %d %d", &r.video, &r.from, &r.count);
	requests.push_back(r);
        endpoints[r.from].requests.push_back(r);
    }

    vector<pair<int, int>> blah;
    for(int i = 0; i < video_size.size(); i++)
	blah.push_back(make_pair(video_size[i], i));
    sort(blah.begin(), blah.end());

    vector<int> remaining;
    vector<vector<int>> sol;
    for(int i = 0; i < n_caches; i++) remaining.push_back(cache_size);
    for(int i = 0; i < n_caches; i++)
    {
	vector<int> tmp;
	sol.push_back(tmp);
    }

    int notdone = 0;
    int idx = 0;
    for(auto x : blah)
    {
	bool done = false;
	int num = 0;
	for(int &s : remaining)
	    if(s >= x.first)
	    {
		sol[num].push_back(x.second);
		s -= x.first;
		done = true;
		break;
	    }
	    else num++;
	if(!done) notdone += x.first;
	idx++;
    }

    printf("%d\n", n_caches);
    for(int i = 0; i < n_caches; i++)
    {
	printf("%d", i);
	for(int x : sol[i]) printf(" %d", x);
	printf("\n");
    }
    
    return 0;
}
