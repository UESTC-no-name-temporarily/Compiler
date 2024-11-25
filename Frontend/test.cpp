#include <bits/stdc++.h>
#include <unistd.h>
#include <dirent.h>
char cmd[1005], filename[105];

int main() {
    // 打开 ../test_cases 目录
    DIR *test_cases_dir = opendir("../test_cases");
    if (!test_cases_dir) {
        perror("Failed to open ../test_cases directory");
        return 1;
    }

    while (true) {
        dirent *d = readdir(test_cases_dir);
        if (!d) {
            puts("Test done!");
            break;
        }
        if (d->d_type == DT_DIR) continue;

        int len = strlen(d->d_name);
        if (len < 3 || strcmp(".sy", d->d_name + len - 3)) continue;

        strncpy(filename, d->d_name, len - 3);
        filename[len - 3] = '\0';

        snprintf(cmd, sizeof(cmd), "./main < ../test_cases/%s > ../test_cases_output/%s.out", d->d_name, filename);
        printf("Testing %s\n", d->d_name);

        if (system(cmd)) {
            printf("Failed in %s\n", d->d_name);
            closedir(test_cases_dir); // 关闭目录
            return 1;
        } else {
            puts("OK");
        }
    }

    closedir(test_cases_dir); // 确保关闭目录
    return 0;
}
