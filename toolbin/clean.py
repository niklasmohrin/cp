#!/usr/bin/env python3

from pathlib import Path

root = Path(__file__).parent.parent.resolve()
to_delete = list()

for subdir in root.iterdir():
    compiled_file = subdir / subdir.name
    if compiled_file.is_file():
        to_delete.append(compiled_file)

if len(to_delete) == 0:
    print("All clean :^)")
    exit()

print("Found files:")
print("\n".join(map(str, to_delete)))
response = input("Delete them all? [y/N] ")
if response == "y":
    for f in to_delete:
        f.unlink()
    print("Deleted.")
else:
    print("Aborting.")
