# Removes all files besides .pas files from the test directory.
# Use with caution.
find ./test -maxdepth 1 -type f ! -iname "*.pas" -delete
