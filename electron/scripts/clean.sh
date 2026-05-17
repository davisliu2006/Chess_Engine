remove_all=false

for arg in "$@"; do
    if [ "$arg" = "--all" ]; then
        remove_all=true
    fi
done

paths_to_remove=(
    "dist"
    "react-app/build"
)

if [ "$remove_all" = true ]; then
    paths_to_remove+=(
        "node_modules"
        "react-app/node_modules"
    )
fi

for path in "${paths_to_remove[@]}"; do
    if [ -e "$path" ]; then
        echo "Removing $path..."
        rm -rf "$path"
    fi
done