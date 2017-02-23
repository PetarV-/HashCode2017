#include <cstdio>
#include <vector>

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

    printf("%d videos, %d caches", n_videos, n_caches);
    
    return 0;
}
