import os

OUTPUT_FILE = "/mnt/c/Users/soumi/Desktop/codes.txt"

extensions = {
    ".cpp",
    ".h",
    ".hpp",
    ".c",
    ".cc"
}

with open(OUTPUT_FILE, "w", encoding="utf-8") as outfile:
    for root, dirs, files in os.walk("."):
        for file in files:
            _, ext = os.path.splitext(file)

            if ext in extensions:
                path = os.path.join(root, file)

                outfile.write("=" * 80 + "\n")
                outfile.write(f"FILE: {path}\n")
                outfile.write("=" * 80 + "\n\n")

                try:
                    with open(path, "r", encoding="utf-8") as infile:
                        outfile.write(infile.read())
                except Exception as e:
                    outfile.write(f"[ERROR READING FILE: {e}]")

                outfile.write("\n\n")

print(f"Dumped project codes into {OUTPUT_FILE}")