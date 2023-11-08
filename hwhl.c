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
            FILE *fh = fopen(path[i], "r");

            if (fscanf(fh, "%d", &val[i]) == 1) {
                val[i] /= 1000;
            } else {
                fprintf(stderr, "%s\n", "[x]");
                exit(1);
            }
            fclose(fh);
        }

	printf("\033[2J\033[1r");

        for (int i = 0; i < 3; i++) {
            printf("%-12s %d°C\n", label[i], val[i]);
        }

        if (val[1] >= 85) {
            if (system("poweroff") == -1) {
                exit(1);
            }
            break;
        }

        sleep(4);
    }
}

void fancGPU(const char *str) {
    const char *path[] = {
        "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1_enable",
        "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1",
    };

    char *end;
    short val = strtol(str, &end, 10);

    /* eliminate char string input,
       set out-of-range integer values */
    if (*end != '\0' || val < 0 || val > 255) {
        fprintf(stderr, "%s\n", "[x]");
        exit(1);
    }

    for (int i = 0; i < 2; i++) {
        if (chmod(path[i], 0644) < 0) {
            fprintf(stderr, "%s\n", "[x]");
            exit(1);
        }
        FILE *fh = fopen(path[i], "w");
        fprintf(fh, i == 0 ? "1" : "%d", val);

        fclose(fh);
        chmod(path[i], 0444);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        tempGPU();

    /* lock to appropriate use
       of arguments with hwhl -c */
    } else if (argc == 3
	    && argv[1][0] == '-'
	    && argv[1][1] == 'c'
            && (argv[1][2] == ' '
	    || argv[1][2] == '\0')) {
        fancGPU(argv[2]);
    } else {
        fprintf(stderr, "%s\n", "[-c 0-255]");
        return 1;
    }

    return 0;
}
