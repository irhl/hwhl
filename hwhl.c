#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#define pwm_but "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1_enable"
#define pwm_fan "/sys/class/drm/card0/device/hwmon/hwmon0/pwm1"

void gpu_fan_control() {
    printf("%s\n", "[#]: 0-255");
    printf("\n%s", "[?]: ");

    int val;
    scanf("%d", &val);

    FILE *fd;

    fd = fopen(pwm_but, "w");
    fprintf(fd, "1");

    fd = fopen(pwm_fan, "w");
    fprintf(fd, "%d", val);

    fclose(fd);
}

void gpu_fan_start() {
    printf("%s\n", "Brrrrff");

    FILE *fd;

    fd = fopen(pwm_but, "w");
    fprintf(fd, "1");

    fd = fopen(pwm_fan, "w");
    fprintf(fd, "80");

    fclose(fd);
}

void setup_screen() {
    /* '\e[1r': 0:0
       '\e[2J': clear screen */
    char* x = "\e[1r\e[2J";

    printf("%s", x);
    printf("%s\n", "[1] GPU: Fan Start");
    printf("%s\n", "[2] GPU: Fan Control");
    printf("\n%s", "[?]: ");
}

int main(int argc, char **argv) {
    char mode[] = "664";
    char buf1[100] = pwm_but;
    char buf2[100] = pwm_fan;

    int i; i = strtoul(mode, 0, 8);

    if (chmod (buf1,i) < 0) {
        fprintf(stderr, "\n[!] Sudo Access\n",
		argv[0], buf1, mode, errno, strerror(errno));
        exit(1);
    }

    else if (chmod (buf2,i) < 0) {
        fprintf(stdout, argv[0], buf2, mode, errno, strerror(errno));
        exit(1);
    }

    setup_screen();

    char options;
    do { scanf("%c", &options);

    switch(options) {
        case '1':
            gpu_fan_start();
            break;

        case '2':
            gpu_fan_control();
            break;

        default:
            setup_screen();
	    break;
        }
    }

    while(options<'1' || options>'6');

    return 0;
}
