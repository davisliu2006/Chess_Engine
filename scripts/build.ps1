if ($args.length -lt 1) {
    echo "Missing arguement"; return;
}
$file = $args[0]
$fileBase = (Get-Item $file).Basename 
clang++ $args[0] chess/*.cpp -std=c++17 -O2 -o "$fileBase.exe"