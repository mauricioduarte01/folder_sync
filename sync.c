#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void sync_folders(const char* source_dir, const char* destination_dir) {
    DIR* dir;
    struct dirent* entry;
    char source_path[256];
    char destination_path[256];

    // Open the source directory
    dir = opendir(source_dir);
    if (dir == NULL) {
        printf("Failed to open source directory: %s\n", source_dir);
        return;
    }

    // Iterate over the files in the source directory
    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".." directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct the source and destination paths
        snprintf(source_path, sizeof(source_path), "%s/%s", source_dir, entry->d_name);
        snprintf(destination_path, sizeof(destination_path), "%s/%s", destination_dir, entry->d_name);

        // Check if the file exists in the destination directory
        FILE* destination_file = fopen(destination_path, "rb");
        if (destination_file != NULL) {
            // File exists in the destination directory

            // Close the destination file
            fclose(destination_file);
        } else {
            // File does not exist in the destination directory
            // Copy the file from source to destination
            FILE* source_file = fopen(source_path, "rb");
            destination_file = fopen(destination_path, "wb");

            if (source_file == NULL || destination_file == NULL) {
                printf("Failed to copy file: %s\n", entry->d_name);
                if (source_file != NULL) {
                    fclose(source_file);
                }
                continue;
            }

            char buffer[1024];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
                fwrite(buffer, 1, bytes_read, destination_file);
            }

            // Close the files
            fclose(source_file);
            fclose(destination_file);
        }
    }

    // Close the source directory
    closedir(dir);

    // Open the destination directory to handle deletions
    dir = opendir(destination_dir);
    if (dir == NULL) {
        printf("Failed to open destination directory: %s\n", destination_dir);
        return;
    }

    // Iterate over the files in the destination directory
    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".." directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct the source and destination paths
        snprintf(destination_path, sizeof(destination_path), "%s/%s", destination_dir, entry->d_name);
        snprintf(source_path, sizeof(source_path), "%s/%s", source_dir, entry->d_name);

        // Check if the file exists in the source directory
        FILE* source_file = fopen(source_path, "rb");
        if (source_file == NULL) {
            // File does not exist in the source directory
            // Delete the file from the destination directory
            if (remove(destination_path) != 0) {
                printf("Failed to delete file: %s\n", entry->d_name);
            }
        } else {
            // File exists in the source directory
            fclose(source_file);
        }
    }

    // Close the destination directory
    closedir(dir);
}

int main() {
    const char* source_dir = "/home/bladerunner/Documents/C_testapp/folder1";
    const char* destination_dir = "/home/bladerunner/Documents/C_testapp/folder2";

    sync_folders(source_dir, destination_dir);

    printf("Folder synchronization completed.\n");

    return 0;
}
