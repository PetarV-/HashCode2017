#include <cstdio>
#include <vector>
#include <set>
#include <tuple>
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

// Extra state for this guy
vector<vector<int>> vid_req;
vector<map<int, int>> cache_pairs;

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

vector<int> req_cid;
vector<long long> req_lat;

long long build_shit(vector<set<int>> sol)
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

    req_cid = vector<int>(requests.size());
    req_lat = vector<int>(requests.size());
    long long ret = 0LL;
    for (int i=0;i<requests.size();i++)
    {
        int eid = requests[i].from;
        int vid = requests[i].video;
        long long opt = endpoints[eid].base_latency;
        req_cid[i] = -1;
        req_lat[i] = opt;
        for (int j=0;j<endpoints[eid].conn.size();j++)
        {
            auto curr_conn = endpoints[eid].conn[j];
            int cid = curr_conn.cache;
            int lat = curr_conn.latency;
            if (lat >= opt) continue;
            if (sol[cid].count(vid)) 
            {
                req_cid[i] = cid;
                req_lat[i] = lat;
                opt = lat;
            }
        }
        ret += opt * requests[i].count;
    }

    return ret;
}

vector<long long> get_weights(vector<set<int>> sol, int cache_id)
{
    long long orig = build_shit(sol);
    vector<long long> ret(n_videos);
    for (int i=0;i<n_videos;i++)
    {
        assert(video_size[i] <= cache_size);
        long long gainz = 0LL;

        for (int j=0;j<vid_req[i].size();j++)
        {
            int rq = vid_req[i][j];
            int ept = requests[rq].from;
            int cnt = requests[rq].count;

            if (!cache_pairs[ept].count(cache_id)) continue;
            int lat = cache_pairs[ept][cache_id];

            prev_cid = req_cid[rq];
            prev_lat = req_lat[rq];
            if (lat >= prev_lat) continue;
            gainz += (prev_lat - lat) * cnt;
        }

        ret[i] = gainz;
    }
    return ret;
}

long long maxKnapsack[60000];
vector <int> solveKnapsack (vector <tuple <int,int,long long> > item, int cacheSize) //item video_id, video_size, video_score
{
    for (int i=0; i<=cacheSize; i++)
        maxKnapsack[i]=-1000000000;
    maxKnapsack[0]=0;

    int topUse=0;
    long long maxSol,capSol;
    maxSol=-1000;
    capSol=-1;

    for (auto video : item)
    {
        for (int use=topUse; use>=0; use--)
        {
            int nextUse=use+get<1>(video);
            if (nextUse>cacheSize)
                continue;
            int nextValue=maxKnapsack[use]+get<2>(video);
            if (maxKnapsack[nextUse]<nextValue)
            {
                maxKnapsack[nextUse]=nextValue;
                if (nextValue>maxSol)
                {
                    maxSol=nextValue;
                    capSol=nextUse;
                }
            }
        }
    }

    vector <int> solution;
    reverse(item.begin(),item.end());
    for (auto video : item)
    {
        int nextCap=capSol-get<1>(video);
        if (nextCap<0)
            continue;
        if (maxKnapsack[nextCap]+get<2>(video)==maxKnapsack[capSol])
        {
            solution.push_back(get<0>(video));
            capSol=nextCap;
        }
    }
    return solution;
}

int main()
{
    int n_requests;
    scanf("%d %d %d %d %d", &n_videos, &n_endpoints, &n_requests, &n_caches, &cache_size);

    vid_req = vector<vector<int>>(n_videos);
    cache_pairs = vector<map<int, int>>(n_endpoints);

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
        cache_pairs[i][c.cache] = c.latency;
	}
	endpoints.push_back(e);
    }

    for(int i = 0; i < n_requests; i++)
    {
	request r;
	scanf("%d %d %d", &r.video, &r.from, &r.count);
    vid_req[r.video].push_back(i);
	requests.push_back(r);
        endpoints[r.from].requests.push_back(r);
    }

    printf("%d videos, %d caches", n_videos, n_caches);

    return 0;
}
