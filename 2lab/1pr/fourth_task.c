#define _CRT_SECURE_NO_WARNINGS
#define SPEED_OF_LIGHT 299792458.0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
	double height = 600;
	double v_sat = 7.56;
	double f_0 = 2.18 * pow(10, 9);
	double r = 6371;
	double a;
	
	printf("Enter elevation angle alpha (0..90 degrees): ");
	scanf("%lf", &a);

	if (a < 0.0 || a > 90.0) {
		printf("ERROR: angle must be between 0 and 90 degrees\n");
	}

	v_sat = v_sat * 1000.0; //переведём в метры в секунду
	height *= 1000.0;
	r *= 1000.0;
	double radians = a * (M_PI / 180);
	double dist = sqrt(pow(r, 2) * pow(sin(radians), 2) + 2*r*height + height*height) - r * sin(radians);
	double v_rad = v_sat * (r * cos(radians) / (r * sin(radians) + dist));
	double b = v_rad/SPEED_OF_LIGHT;
	double t = b * pow(10, 9);
	double f_d = (f_0*sqrt((1 + b) / (1 - b)) - f_0) / 1000;
	dist /= 1000.0;

	printf("Distance: %lf\n", dist);
	printf("Radial velocity (vr): %lf m/s\n", v_rad);
	printf("Time drift (dT): %lf ns/s\n", t);
	printf("Doppler shift (fd): %lf\n", f_d);
	printf("PROFILE: ");

	if ((f_d > 42.0 || t > 18) && dist > 1000) {
		printf("CRITICAL ERROR\n");
	}
	else if (f_d > 25.0 && t > 10) {
		printf("HIGH DYNAMIC MODE\n");
	}
	else if (f_d > 3.0 || fabs(t) > 1) {
		printf("NORMAL ACCELERATION\n");
	}
	else {
		printf("ZENITH MODE\n");
	}

	return EXIT_SUCCESS;
}
