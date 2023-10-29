#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

void tempGPU() {
    const char *pwmr = "/sys/class/drm/card0/device/hwmon/hwmon0/temp1_input";

    // restore defaults
    signal(SIGINT, SIG_DFL);

    while (1) {
        FILE *f3 = fopen(pwmr, "r");

        if (f3) {
            char buffer[16];
            if (fscanf(f3, "%15s", buffer) == 1) {
                printf("\033[1r\033[2J");
                printf("[*]: %d°C\n", atoi(buffer) / 1000);
            } else {
                perror("[xx]");
                break;
            }
            fclose(f3);
        }
        usleep(4000000);
    }
}

void fancGPU() {
    const char *pwme = "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1_enable";
    const char *pwm1 = "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1";

    /* set permissions -rw-r--r-- */
    if (chmod(pwme, 0644) < 0 || chmod(pwm1, 0644) < 0) {
        perror("[xx]");
        return;
    }

    /* declare val var, val stores values */
    int val;
    printf("[0-255]: ");

    /* prompt user input & error input handling,
       an error is triggered if the value is out of range
    */
    if (scanf("%d", &val) != 1 || val < 0 || val > 255) {
        printf("[x]\n");
        return;
    }

    /* submit pwm values directly to fn
       without error handling
    */
    FILE *f1 = fopen(pwme, "w");
    FILE *f2 = fopen(pwm1, "w");

    fprintf(f1, "1");
    fclose(f1);

    fprintf(f2, "%d", val);
    fclose(f2);

    /* restore permissions -r--r--r-- */
    if (chmod(pwme, 0444) < 0 || chmod(pwm1, 0444) < 0) {
        perror("[xx]");
        return;
    }
}

int main() {
    char options;
    do {
        printf("\033[1r\033[2J");
        printf("%s\n", "[1] GPU: Fan Control");
	printf("\n%s", "[?]: ");

	scanf(" %c", &options);
	printf("\n");

        switch (options) {
            case '1':
                fancGPU();
                tempGPU();
                break;

            default:
                printf("\033[1r\033[2J");
                break;
        }
    } while (options < '1' || options > '2');

    return 0;
}
