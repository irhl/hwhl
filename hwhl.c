#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

void tempGPU() {
    const char *path[] = {
        "/sys/class/drm/card0/device/hwmon/hwmon0/temp1_input",
        "/sys/class/drm/card0/device/hwmon/hwmon0/temp2_input",
        "/sys/class/drm/card0/device/hwmon/hwmon0/temp3_input",
    };

    const char *label[] = {
        "gpu_edge", "gpu_junk", "gpu_mem",
    };

    signal(SIGINT, SIG_DFL);

    while (1) {
        int val[3];

        for (int i = 0; i < 3; i++) {
            FILE *file = fopen(path[i], "r");

            if (file) {
                if (fscanf(file, "%d", &val[i]) != 1) {
                    perror("[xx]");
                    exit(1);
                }
                fclose(file);
            }
        }

	printf("\033[2J\033[1r");

        for (int i = 0; i < 3; i++) {
            printf("%-12s %d°C\n", label[i], val[i] / 1000);
        }

        fflush(stdout);
        sleep(4);
    }
}

void fancGPU() {
    const char *path[] = {
        "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1_enable",
        "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1",
    };

    printf("[0-255]: ");

    int val;

    if (scanf("%d", &val) != 1 || val < 0 || val > 255) {
        printf("[x]\n");
        exit(1);
    }

    for (int i = 0; i < 2; i++) {
        if (chmod(path[i], 0644) < 0) {
            perror("[xx]");
            exit(1);
        }

        /* submit pwm values directly to function
           without any proper error handling */
        FILE *file = fopen(path[i], "w");

        if (i == 0) {
            fprintf(file, "1");
        } else {
            fprintf(file, "%d", val);
        }

        fclose(file);

        chmod(path[i], 0444);

	tempGPU();
    }
}

int main() {
    printf("\033[2J\033[1r");
    printf("%s\n", "[1] GPU: Fan Control");
    printf("%s\n", "[2] GPU: Fan Monitor");
    printf("\n%s", "[?]: ");

    char options;
    do {
	scanf(" %c", &options);
	printf("\n");

        switch (options) {
            case '1':
                fancGPU();
                break;
            case '2':
                tempGPU();
                break;

            default:
                printf("\033[2J\033[1r");
                break;
        }
    } while (options < '1' || options > '2');

    return 0;
}
