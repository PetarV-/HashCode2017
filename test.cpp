#include <cstdio>
#include <vector>
#include <set>

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

long long get_latency(vector<set<int>> sol)
{
	if (sol.size() > n_caches) return -2; // using more caches than there exist!
	
	for (int i=0;i<sol.size();i++)
	{
		int tot = 0;
		for (auto it = sol[i].begin(); it != sol[i].end(); it++)
		{
			tot += video_size[(*it)];
			if (tot > cache_size) return -1; // exceeding size of cache!
		}
	}
	
	long long ret = 0LL;
	for (int i=0;i<requests.size();i++)
	{
		int eid = requests[i].from;
		int vid = requests[i].video;
		long long opt = endpoints[eid].base_latency;
		for (int j=0;j<endpoints[eid].conn.size();j++)
		{
			auto curr_conn = endpoints[eid].conn[j];
			int cid = curr_conn.cache;
			int lat = curr_conn.latency;
			if (lat >= opt) continue;
			if (sol[cid].count(vid)) opt = lat;
		}
		ret += opt * requests[i].count;
	}

	return ret;
}

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

    printf("%d videos, %d caches", n_videos, n_caches);

    FILE *f = fopen("trending_petar.out", "r");

    fscanf(f, "%*d");
    vector<set<int>> sol(n_caches, set<int>());
    long long tot = get_latency(sol);
    for (int i=0;i<n_caches;i++)
    {
    	int szz;
    	fscanf(f, "%d", &szz);
    	for (int j=0;j<szz;j++)
    	{
    		int xx;
    		fscanf(f, "%d", &xx);
    		sol[i].insert(xx);
    	}
    }

    long long now = get_latency(sol);
    long long total_req = 0LL;
    for (int i=0;i<requests.size();i++)
    {
    	total_req += requests[i].count;
    }
    double saved = (tot - now) * 1.0 / total_req;
    printf("\n%lld\n", now);
    printf("\n %lld\n%.5lf\n", tot, saved);

    return 0;
}
