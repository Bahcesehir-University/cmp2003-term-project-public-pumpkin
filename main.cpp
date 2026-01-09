#include <bits/stdc++.h>
using namespace std;

static constexpr int TOP_K = 10;

struct ZoneCount { string zone; int count; };
struct SlotCount { string zone; int hour; int count; };

class TripAnalyzer {
private:
    unordered_map<string,int> zoneId;
    vector<string> idToZone;
    vector<int> zoneCount;
    vector<array<int,24>> zoneHourCount;

    static inline void trimSpan(const char*& a, const char*& b) {
        while (a < b && (unsigned char)*a <= ' ') ++a;
        while (b > a && (unsigned char)*(b-1) <= ' ') --b;
    }

    static inline bool parseHourFromDatetime(const char* a, const char* b, int& hourOut) {
        const char* p = a;
        while (p < b && *p != ' ') ++p;
        if (p == b) return false;
        while (p < b && *p == ' ') ++p;
        if (p + 1 >= b) return false;
        if (!(p[0] >= '0' && p[0] <= '9' && p[1] >= '0' && p[1] <= '9')) return false;

        int h = (p[0]-'0')*10 + (p[1]-'0');
        if (h < 0 || h > 23) return false;
        hourOut = h;
        return true;
    }

public:
    void ingestStdin() {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        zoneId.reserve(4096);

        string line;
        while (getline(cin, line)) {
            const char* s = line.c_str();
            const char* end = s + line.size();

            const char* c1 = (const char*)memchr(s, ',', end - s); if (!c1) continue;
            const char* c2 = (const char*)memchr(c1+1, ',', end - (c1+1)); if (!c2) continue;
            const char* c3 = (const char*)memchr(c2+1, ',', end - (c2+1)); if (!c3) continue;
            const char* c4 = (const char*)memchr(c3+1, ',', end - (c3+1)); if (!c4) continue;
            const char* c5 = (const char*)memchr(c4+1, ',', end - (c4+1)); if (!c5) continue;
            (void)c5;

            const char* pA = c1 + 1;
            const char* pB = c2;
            trimSpan(pA, pB);
            if (pA >= pB) continue;

            const char* dA = c3 + 1;
            const char* dB = c4;
            trimSpan(dA, dB);
            if (dA >= dB) continue;

            int hour;
            if (!parseHourFromDatetime(dA, dB, hour)) continue;

            string pickup(pA, pB);

            int id;
            auto it = zoneId.find(pickup);
            if (it == zoneId.end()) {
                id = (int)zoneId.size();
                zoneId.emplace(pickup, id);
                idToZone.push_back(std::move(pickup));
                zoneCount.push_back(0);
                zoneHourCount.push_back({});
            } else {
                id = it->second;
            }

            zoneCount[id]++;
            zoneHourCount[id][hour]++;
        }
    }

    vector<ZoneCount> topZones(int K = TOP_K) {
        vector<int> idx(zoneCount.size());
        iota(idx.begin(), idx.end(), 0);

        sort(idx.begin(), idx.end(), [&](int a, int b) {
            if (zoneCount[a] != zoneCount[b]) return zoneCount[a] > zoneCount[b];
            return idToZone[a] < idToZone[b];
        });

        if ((int)idx.size() > K) idx.resize(K);

        vector<ZoneCount> res;
        res.reserve(idx.size());
        for (int id : idx) res.push_back({idToZone[id], zoneCount[id]});
        return res;
    }

    vector<SlotCount> topBusySlots(int K = TOP_K) {
        struct Item { int z, h, c; };
        vector<Item> items;
        items.reserve(zoneHourCount.size() * 2);

        for (int z = 0; z < (int)zoneHourCount.size(); ++z) {
            for (int h = 0; h < 24; ++h) {
                int c = zoneHourCount[z][h];
                if (c) items.push_back({z, h, c});
            }
        }

        sort(items.begin(), items.end(), [&](const Item& a, const Item& b) {
            if (a.c != b.c) return a.c > b.c;
            if (idToZone[a.z] != idToZone[b.z]) return idToZone[a.z] < idToZone[b.z];
            return a.h < b.h;
        });

        if ((int)items.size() > K) items.resize(K);

        vector<SlotCount> res;
        res.reserve(items.size());
        for (auto &it : items) res.push_back({idToZone[it.z], it.h, it.c});
        return res;
    }
};

int main() {
    TripAnalyzer analyzer;
    analyzer.ingestStdin();

    cout << "TOP_ZONES\n";
    for (auto& z : analyzer.topZones())
        cout << z.zone << "," << z.count << "\n";

    cout << "TOP_SLOTS\n";
    for (auto& s : analyzer.topBusySlots())
        cout << s.zone << "," << s.hour << "," << s.count << "\n";

    return 0;
}
