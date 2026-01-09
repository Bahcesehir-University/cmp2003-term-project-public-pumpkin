// analyzer.h
#pragma once
#include <string>
#include <vector>

struct ZoneCount {
    std::string zone;
    int count;
};

struct SlotCount {
    std::string zone;
    int hour;
    int count;
};

class TripAnalyzer {
public:
    void ingestStdin();
    void ingestFile(const std::string& filename);
    std::vector<ZoneCount> topZones(int K = 10);
    std::vector<SlotCount> topBusySlots(int K = 10);

private:
    // hidden implementation details are in analyzer-1.cpp
    struct Impl;
    Impl* pImpl = nullptr;
};
