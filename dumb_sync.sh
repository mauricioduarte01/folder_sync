#!/bin/bash

# Set the source and destination directories
SOURCE_DIR="/home/bladerunner/Documents/folder1"
DEST_DIR="/home/bladerunner/Documents/folder2"

# Sync the files from source to destination, excluding specific files
rsync -avz --delete --exclude='subdir/CMakeLists.txt' "$SOURCE_DIR/" "$DEST_DIR/"

