#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

using namespace std;

const int MAX = 50;
const double INF = 1e18;

class DoThiBay {
private:
    int n;
    string tp[MAX];
    double gia[MAX][MAX];
    double hanhLy[MAX][MAX];

public:
    DoThiBay() {
        n = 8;
        string tmp[8] = {"Ha Noi","Ho Chi Minh","Bangkok","Tokyo",
                         "Singapore","Kuala Lumpur","Sydney","New York"};
        for (int i = 0; i < n; ++i) tp[i] = tmp[i];

        for (int i = 0; i < MAX; ++i)
            for (int j = 0; j < MAX; ++j)
                gia[i][j] = hanhLy[i][j] = 0.0;

        gia[0][1] = gia[1][0] = 2.443;
        gia[0][2] = gia[2][0] = 2.11;
        gia[1][4] = gia[4][1] = 2.289;
        gia[1][3] = gia[3][1] = 9.074;
        gia[2][6] = gia[6][2] = 15.828;
        gia[6][7] = gia[7][6] = 33.764;
        gia[3][7] = gia[7][3] = 23.792;
        gia[3][5] = gia[5][3] = 11.185;
        gia[4][6] = gia[6][4] = 13.253;
        gia[4][5] = gia[5][4] = 0.738;



        hanhLy[0][1] = hanhLy[1][0] = 0.2;
        hanhLy[0][2] = hanhLy[2][0] = 0.15;
        hanhLy[1][4] = hanhLy[4][1] = 0.18;
        hanhLy[1][3] = hanhLy[3][1] = 0.5;
        hanhLy[2][6] = hanhLy[6][2] = 0.8;
        hanhLy[6][7] = hanhLy[7][6] = 1.2;
        hanhLy[3][7] = hanhLy[7][3] = 1.0;
        hanhLy[3][5] = hanhLy[5][3] = 0.6;
        hanhLy[4][6] = hanhLy[6][4] = 0.9;
        hanhLy[4][5] = hanhLy[5][4] = 0.05;
    }

    void themTP(const string& name) {
    if (n >= MAX) {
        cout << "Error: City limit reached!\n";
        return;
    }

    tp[n] = name;

    for (int i = 0; i <= n; ++i) {
        gia[i][n] = gia[n][i] = 0.0;
        hanhLy[i][n] = hanhLy[n][i] = 0.0;
    }

    n++;
}


    void themChuyenBay(int a, int b, double g, double hl) {
    if (a < 0 || a >= n || b < 0 || b >= n || a == b || g < 0 || hl < 0) {
        cout << "Error: Invalid flight data.\n";
        return;
    }

    gia[a][b] = gia[b][a] = g;
    hanhLy[a][b] = hanhLy[b][a] = hl;
    
    cout << "Added: " << tp[a] << " <-> " << tp[b] << " | Price: " << g << " | Luggage: " << hl << endl;
}

    void chayDijkstra(int s, int t, int ve, double kg) {
        double hsVe = (ve == 1 ? 1.4 : (ve == 2 ? 2.0 : 1.0));
        double hsKg = kg / 20.0;

        double dist[MAX]; 
        int truoc[MAX];
        dijkstra(s, t, hsVe, hsKg, dist, truoc);

        if (dist[t] >= INF / 2) {
            cout << "No path found!\n";
            return;
        }

        cout << fixed << setprecision(3);
        cout << "Total Cost: " << dist[t] << " tr\n";

        int path[MAX], len = 0;
        for (int cur = t; cur != -1; cur = truoc[cur])
            path[len++] = cur;

        cout << "Route: ";
        for (int i = len - 1; i >= 0; --i) {
            cout << tp[path[i]];
            if (i > 0) cout << " -> ";
        }
        cout << endl;
    }

    double chiPhi(int u, int v, double hsVe, double hsKg) const {
        if (gia[u][v] <= 0) return INF;
        return gia[u][v] * hsVe + hanhLy[u][v] * hsKg;
    }

    void dijkstra(int s, int t, double hsVe, double hsKg, double d[], int truoc[]) const {
        double dist[MAX]; int used[MAX];
        for (int i = 0; i < n; ++i) {
            dist[i] = INF; used[i] = 0; truoc[i] = -1;
        }
        dist[s] = 0;
        for (int step = 0; step < n; ++step) {
            double best = INF; int u = -1;
            for (int i = 0; i < n; ++i)
                if (!used[i] && dist[i] < best) { best = dist[i]; u = i; }
            if (u == -1) break;
            used[u] = 1;
            for (int v = 0; v < n; ++v)
                if (gia[u][v] > 0) {
                    double w = chiPhi(u, v, hsVe, hsKg);
                    if (dist[v] > dist[u] + w) {
                        dist[v] = dist[u] + w;
                        truoc[v] = u;
                    }
                }
        }
        for (int i = 0; i < n; ++i) d[i] = dist[i];
    }
};

int main() {
    DoThiBay g;
    int choice;

    while (cin >> choice) {
        if (choice == 4) { 
            int a, b;
            double p, l;
            if (cin >> a >> b >> p >> l) {
                cout << "A new flight connection has been added"<<endl;
                g.themChuyenBay(a, b, p, l);
            }
        }
        else if (choice == 5) { 
            int s, t, ve;
            double kg;
            if (cin >> s >> t >> ve >> kg) {
                g.chayDijkstra(s, t, ve, kg);
            }
        }
        else if (choice == 2) { 
            string cityName;
            cin.ignore();
            getline(cin, cityName);
            g.themTP(cityName);
        }
        else if (choice == 0) {
            break;
        }
    }

    return 0;
}
