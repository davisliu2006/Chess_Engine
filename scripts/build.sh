if [ $# -lt 1 ]; then
    echo "Missing argument"; return;
fi
file=$1
fileBase=$(basename "$file")
fileBase=${fileBase%.*}
clang++ $file chess/*.cpp -std=c++17 -O2 -o "$fileBase.out"