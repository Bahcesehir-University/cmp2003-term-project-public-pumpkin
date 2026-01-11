#!/bin/bash

# Transfer Script: Public to Private Repository
# This script helps copy the working implementation from the public repository
# to the private repository.

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== CMP2003 Repository Transfer Script ===${NC}"
echo ""

# Check if we're in the right directory
if [ ! -f "analyzer.cpp" ] || [ ! -f "analyzer.h" ]; then
    echo -e "${RED}Error: This script must be run from the public repository root directory${NC}"
    echo "Please cd to the cmp2003-term-project-public-pumpkin directory first"
    exit 1
fi

# Get the private repository path
echo -e "${YELLOW}Enter the path to the private repository:${NC}"
echo "(e.g., ../cmp2003-term-project-pumpkin or ~/repos/private-repo)"
read -r PRIVATE_REPO_PATH

# Expand tilde and validate path
PRIVATE_REPO_PATH="${PRIVATE_REPO_PATH/#\~/$HOME}"

if [ ! -d "$PRIVATE_REPO_PATH" ]; then
    echo -e "${RED}Error: Directory does not exist: $PRIVATE_REPO_PATH${NC}"
    exit 1
fi

if [ ! -d "$PRIVATE_REPO_PATH/.git" ]; then
    echo -e "${RED}Error: Not a git repository: $PRIVATE_REPO_PATH${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}Found private repository at: $PRIVATE_REPO_PATH${NC}"
echo ""

# List files to be copied
echo -e "${YELLOW}The following files will be copied:${NC}"
echo "  - analyzer.h"
echo "  - analyzer.cpp"
echo "  - main.cpp"
echo "  - makefile"
echo ""

# Ask for confirmation
echo -e "${YELLOW}Do you want to proceed? (yes/no)${NC}"
read -r CONFIRM

if [ "$CONFIRM" != "yes" ] && [ "$CONFIRM" != "y" ]; then
    echo "Transfer cancelled."
    exit 0
fi

# Create backup directory in private repo
BACKUP_DIR="$PRIVATE_REPO_PATH/backup-$(date +%Y%m%d-%H%M%S)"
echo ""
echo -e "${GREEN}Creating backup in: $BACKUP_DIR${NC}"
mkdir -p "$BACKUP_DIR"

# Backup existing files if they exist
for file in analyzer.h analyzer.cpp main.cpp makefile; do
    if [ -f "$PRIVATE_REPO_PATH/$file" ]; then
        cp "$PRIVATE_REPO_PATH/$file" "$BACKUP_DIR/"
        echo "  Backed up: $file"
    fi
done

echo ""
echo -e "${GREEN}Copying working implementation files...${NC}"

# Copy the working files
cp analyzer.h "$PRIVATE_REPO_PATH/"
echo "  ✓ Copied analyzer.h"

cp analyzer.cpp "$PRIVATE_REPO_PATH/"
echo "  ✓ Copied analyzer.cpp"

cp main.cpp "$PRIVATE_REPO_PATH/"
echo "  ✓ Copied main.cpp"

cp makefile "$PRIVATE_REPO_PATH/"
echo "  ✓ Copied makefile"

echo ""
echo -e "${GREEN}Files copied successfully!${NC}"
echo ""

# Test build in private repo
echo -e "${YELLOW}Testing build in private repository...${NC}"
cd "$PRIVATE_REPO_PATH"

if make clean > /dev/null 2>&1 && make > /dev/null 2>&1; then
    echo -e "${GREEN}✓ Build successful!${NC}"
else
    echo -e "${RED}✗ Build failed. Check the errors above.${NC}"
    exit 1
fi

echo ""
echo -e "${YELLOW}Running tests...${NC}"
if make test > /dev/null 2>&1; then
    echo -e "${GREEN}✓ All tests passed!${NC}"
else
    echo -e "${RED}✗ Some tests failed. Run 'make test' manually to see details.${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}=== Transfer Complete! ===${NC}"
echo ""
echo "Next steps:"
echo "  1. cd $PRIVATE_REPO_PATH"
echo "  2. git status"
echo "  3. git add analyzer.h analyzer.cpp main.cpp makefile"
echo "  4. git commit -m 'Import working implementation from public repository'"
echo "  5. git push origin main"
echo ""
echo "After pushing, check GitHub Actions to verify 70/70 points."
echo ""
echo -e "${YELLOW}Backup of old files saved in: $BACKUP_DIR${NC}"
