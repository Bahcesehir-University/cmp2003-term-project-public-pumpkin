# Transfer Guide: Public to Private Repository

## Overview
This guide explains how to transfer the working codebase from the **public** repository  
(`cmp2003-term-project-public-pumpkin`) to the **private** repository  
(`cmp2003-term-project-pumpkin`).

The public repository contains a fully functional implementation that achieves **70/70 points** on all autograding tests.

---

## Prerequisites
- Access to both repositories (public and private)
- Git installed locally
- GitHub account with write access to the private repository

---

## Method 1: Manual File Copy (Recommended for Safety)

### Step 1: Clone Both Repositories
```bash
# Clone the public repository (source)
git clone https://github.com/Bahcesehir-University/cmp2003-term-project-public-pumpkin.git public-repo
cd public-repo

# Clone the private repository (destination) in a separate directory
cd ..
git clone https://github.com/Bahcesehir-University/cmp2003-term-project-pumpkin.git private-repo
cd private-repo
```

### Step 2: Copy Essential Files
Copy these working files from the public repository to the private repository:

```bash
# From the private-repo directory, copy files from public-repo:
cp ../public-repo/analyzer.h .
cp ../public-repo/analyzer.cpp .
cp ../public-repo/main.cpp .
cp ../public-repo/makefile .
```

**Do NOT copy** (these should remain as-is in the private repo):
- `test_trip_analyzer.cpp` (grading file - read-only)
- `.github/workflows/classroom.yml` (autograding configuration)
- `catch_amalgamated.cpp` / `catch_amalgamated.hpp` (testing framework)
- `README.md` (assignment instructions)

### Step 3: Verify the Build
```bash
# In the private-repo directory
make clean
make
make test
```

You should see all tests pass with 70/70 points.

### Step 4: Commit and Push
```bash
git add analyzer.h analyzer.cpp main.cpp makefile
git commit -m "Import working implementation from public repository"
git push origin main
```

### Step 5: Verify Autograding
1. Go to the GitHub Actions tab in the private repository
2. Check that the autograding workflow runs
3. Confirm it shows 70/70 points

---

## Method 2: Git Remote Strategy (Advanced)

### Step 1: Add Public Repo as Remote
```bash
cd private-repo
git remote add public https://github.com/Bahcesehir-University/cmp2003-term-project-public-pumpkin.git
git fetch public
```

### Step 2: Cherry-Pick or Merge Specific Files
```bash
# View the commit from public repo that has the working code
git log public/main --oneline

# Cherry-pick the specific commit or checkout specific files
git checkout public/main -- analyzer.h analyzer.cpp main.cpp makefile

# Commit the changes
git commit -m "Import working implementation from public repository"
git push origin main
```

---

## Files Overview

### Core Implementation Files (MUST transfer):
1. **analyzer.h** (547 bytes)
   - Declares TripAnalyzer class interface
   - Defines ZoneCount and SlotCount structures
   - Uses Pimpl (pointer to implementation) pattern

2. **analyzer.cpp** (5,358 bytes)
   - Complete implementation of TripAnalyzer
   - Efficient data structures (unordered_map, vector)
   - Handles malformed data gracefully
   - Optimized for performance (O(n) ingestion, O(m log m) sorting)

3. **main.cpp** (4,569 bytes)
   - Driver program for manual testing
   - Reads from stdin
   - Prints top zones and top busy slots

4. **makefile** (1,874 bytes)
   - Build configuration
   - Test targets (A1-A3, B1-B3, C1-C3)
   - Compilation flags: `-std=c++17 -O2 -Wall -Wextra`

### Reference Files (already in private repo, DO NOT replace):
- `test_trip_analyzer.cpp` - Official grading tests
- `catch_amalgamated.cpp/hpp` - Testing framework
- `.github/workflows/classroom.yml` - CI/CD configuration
- `README.md` - Assignment documentation
- `SmallTrips.csv` - Sample data

---

## Key Implementation Details

### Data Structures Used:
- `unordered_map<string, int>` - Fast zone ID lookup (O(1))
- `vector<string>` - Zone ID to name mapping
- `vector<int>` - Zone trip counts
- `vector<array<int, 24>>` - Per-zone hourly counts

### Algorithm Complexity:
- **Ingestion**: O(n) where n = number of rows
- **topZones**: O(m log m) where m = unique zones
- **topBusySlots**: O(m × 24 × log(m × 24)) ≈ O(m log m)

### Robustness Features:
- Skips malformed CSV rows without crashing
- Handles missing commas gracefully
- Validates hour values (0-23)
- Handles both 3-column and 6-column CSV formats
- Case-sensitive zone handling

---

## Verification Steps

After transferring files to the private repository:

1. **Build Test**:
   ```bash
   make clean && make
   ```
   Should compile without errors.

2. **Unit Tests**:
   ```bash
   make test
   ```
   Should pass all 9 test cases (105 assertions).

3. **Individual Test Categories**:
   ```bash
   make A1  # Empty file test
   make A2  # Dirty data test
   make A3  # Boundary hours
   make B1  # Tie-breaker zones
   make B2  # Tie-breaker slots
   make B3  # Case sensitivity
   make C1  # High cardinality
   make C2  # High volume
   make C3  # Mixed workload
   ```

4. **GitHub Actions**:
   - Push to main branch
   - Navigate to Actions tab
   - Wait for "Autograding Tests" to complete
   - Verify: **Points: 70/70**

---

## Common Issues and Solutions

### Issue 1: Compilation Errors
**Solution**: Ensure you're using C++17:
```bash
g++ --version  # Should be GCC 7+ or Clang 5+
```

### Issue 2: Tests Fail Locally But Pass in Public Repo
**Solution**: 
- Verify you copied all 4 core files exactly
- Check for hidden characters or encoding issues
- Ensure `makefile` uses Unix line endings (LF, not CRLF)

### Issue 3: GitHub Actions Timeout
**Solution**: 
- The implementation is already optimized
- If tests timeout, there may be infrastructure issues
- Try re-running the workflow

### Issue 4: Different Test Results
**Solution**: 
- Ensure the private repo has the same test files
- Do NOT modify `test_trip_analyzer.cpp`
- Check that `catch_amalgamated.cpp/hpp` versions match

---

## Summary

To successfully transfer the working codebase:

1. ✅ Copy 4 files: `analyzer.h`, `analyzer.cpp`, `main.cpp`, `makefile`
2. ✅ Do NOT modify test files or workflows
3. ✅ Build and test locally first
4. ✅ Commit and push to the private repository
5. ✅ Verify GitHub Actions shows 70/70 points

The implementation in the public repository is production-ready and passes all tests. Simply copying these files to the private repository should result in the same 70/70 score.

---

## Contact

If you encounter issues during the transfer, check:
- GitHub Actions logs in both repositories
- Build output for compiler warnings
- Test output for specific failures

For questions about the assignment requirements, refer to `README.md` in the repository.
