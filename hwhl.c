#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>

void gpu_fan_control() {
    const char *pwms = "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1_enable";
    const char *pwmf = "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1";

    /* set permissions -rw-r--r-- */
    if (chmod(pwms, 0644) < 0 || chmod(pwmf, 0644) < 0) {
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

    /* submit pwm values directly to func
       without error handling
    */
    FILE *fi_start = fopen(pwms, "w");
    fprintf(fi_start, "1");
    fclose(fi_start);

    FILE *fi_fan = fopen(pwmf, "w");
    fprintf(fi_fan, "%d", val);
    fclose(fi_fan);

    /* restore permissions -r--r--r-- */
    if (chmod(pwms, 0444) < 0 || chmod(pwmf, 0444) < 0) {
        perror("[xx]");
        return;
    }
}

void scrsetup() {
    printf("\e[1r\e[2J");
}

void scrreset(int sig) {
    printf("%s", "\e[1r\e[2J");
    exit(0);
}

int main() {
    signal(SIGINT, scrreset);
    signal(SIGWINCH, scrreset); // do not shake, it's not milk tea

    scrsetup();

    char options;
    do {
        printf("%s\n", "[1] GPU: Set Fan Speed");
        printf("\n%s", "[?]: ");
        scanf(" %c", &options);

        printf("\n");

        switch (options) {
            case '1':
                gpu_fan_control();
                break;

            default:
                scrsetup();
                break;
        }
    } while (options < '1' || options > '2');

    return 0;
}
