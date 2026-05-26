const fs = require("fs");
const path = require("path");

const rootDir = path.resolve(__dirname, "..");
const sourceCandidates = [
    path.join(rootDir, "Release", "compile_commands.json"),
    path.join(rootDir, "build", "Release", "compile_commands.json"),
    path.join(rootDir, "build", "compile_commands.json")
];

const sourcePath = sourceCandidates.find((candidate) => fs.existsSync(candidate));

if (!sourcePath) {
    console.error("Unable to locate generated compile_commands.json.");
    process.exit(1);
}

const outputPath = path.join(rootDir, "compile_commands.json");
fs.copyFileSync(sourcePath, outputPath);

console.log(`Wrote ${path.relative(rootDir, outputPath)}.`);

const tempReleaseDir = path.join(rootDir, "Release");
if (sourcePath.startsWith(`${tempReleaseDir}${path.sep}`) && fs.existsSync(tempReleaseDir)) {
    fs.rmSync(tempReleaseDir, { recursive: true, force: true });
}