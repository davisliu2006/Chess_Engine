$removeAll = $args -contains "--all" -or $args -contains "-all"

$pathsToRemove = @(
    "dist",
    "react-app/build"
)

if ($removeAll) {
    $pathsToRemove += @(
        "node_modules",
        "react-app/node_modules"
    )
}

foreach ($path in $pathsToRemove) {
    if (Test-Path $path) {
        Write-Host "Removing $path..."
        Remove-Item -Path $path -Recurse -Force
    }
}