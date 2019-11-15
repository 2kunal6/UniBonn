#include<iostream>
#include<vector>
#include<stack>

using namespace std;

vector<int> par, dsu_2ecc, dsu_cc, dsu_cc_size;
int bridges,n;
int lca_iteration;
vector<int> last_visit;
vector<vector<bool> > g;

void init() {
    par.resize(n);
    dsu_2ecc.resize(n);
    dsu_cc.resize(n);
    dsu_cc_size.resize(n);
    lca_iteration = 0;
    last_visit.assign(n, 0);
    for (int i=0; i<n; ++i) {
        dsu_2ecc[i] = i;
        dsu_cc[i] = i;
        dsu_cc_size[i] = 1;
        par[i] = -1;
    }
    bridges = 0;
}

int find_2ecc(int v) {
    if (v == -1)
        return -1;
    return dsu_2ecc[v] == v ? v : dsu_2ecc[v] = find_2ecc(dsu_2ecc[v]);
}

int find_cc(int v) {
    v = find_2ecc(v);
    return dsu_cc[v] == v ? v : dsu_cc[v] = find_cc(dsu_cc[v]);
}

void make_root(int v) {
    v = find_2ecc(v);
    int root = v;
    int child = -1;
    while (v != -1) {
        int p = find_2ecc(par[v]);
        par[v] = child;
        dsu_cc[v] = root;
        child = v;
        v = p;
    }
    dsu_cc_size[root] = dsu_cc_size[child];
}

void merge_path (int a, int b) {
    ++lca_iteration;
    vector<int> path_a, path_b;
    int lca = -1;
    while (lca == -1) {
        if (a != -1) {
            a = find_2ecc(a);
            path_a.push_back(a);
            if (last_visit[a] == lca_iteration)
                lca = a;
            last_visit[a] = lca_iteration;
            a = par[a];
        }
        if (b != -1) {
            path_b.push_back(b);
            b = find_2ecc(b);
            if (last_visit[b] == lca_iteration)
                lca = b;
            last_visit[b] = lca_iteration;
            b = par[b];
        }

    }

    for (int v=0; v<path_a.size();v++) {
        dsu_2ecc[v] = lca;
        if (v == lca)
            break;
        --bridges;
    }
    for (int v=0;v<path_b.size();v++) {
        dsu_2ecc[v] = lca;
        if (v == lca)
            break;
        --bridges;
    }
}

void add_edge(int a, int b) {
    a = find_2ecc(a);
    b = find_2ecc(b);
    if (a == b)
        return;

    int ca = find_cc(a);
    int cb = find_cc(b);

    if (ca != cb) {
        ++bridges;
        if (dsu_cc_size[ca] > dsu_cc_size[cb]) {
            swap(a, b);
            swap(ca, cb);
        }
        make_root(a);
        par[a] = dsu_cc[a] = b;
        dsu_cc_size[cb] += dsu_cc_size[a];
    } else {
        merge_path(a, b);
    }
}
int joincc() {
	vector<bool> vis(n,0);
	int prev=-1,ans=0;
	for(int i=0;i<n;i++) {
		if(vis[i]==1)continue;
		stack<int> s;
		s.push(i);
		if(prev!=-1) {
			add_edge(prev,i);
			add_edge(i, prev);
			prev=i;
		}
		while(!s.empty()) {
			int val=s.top();
			s.pop();
			vis[val]=1;
			for(int j=0;j<n;j++) {
				if(g[val][j]==1 && vis[j]==0) {
					vis[j]=1;
					s.push(j);
				}
			}
		}
		ans++;
	}
	return ans;
}
int main() {
	int x,y,m,icc;
	cin>>n>>m;
	init();
	for(int i=0;i<n;i++) {
		vector<bool> temp;
		for(int j=0;j<n;j++)temp.push_back(0);
		g.push_back(temp);
	}

	for(int i=0;i<m;i++) {
		cin>>x>>y;
		add_edge(x,y);
		g[x][y]=1;
		g[y][x]=1;
	}
	icc=joincc();
	cout<<bridges-icc-1<<endl;
	return 0;
}
