#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "../myfuncs.h"

/* Mathematical functions repersentator and integrator. Integrates a declared function between the two bounds (optionally entered via terminal). It also makes a rude representation of the function in the console. */

/* Mathematical functions */


double f1(double x) {
    return 2*pow(x, 2);
}

double f2(double x) {
    return sin(exp(sqrt(x/(pow(x, 3)-7))));
}

double f3(double x){
    return sin(pow(x, 2)/4);
}

double integ(double (*f) (double), double from, double to, int difs);

/* Main function takes 3 extra optional arguments for the bounds of the integral and the numeric divisions. */
int main(int argc, char *argv[]) {
    clock_t start_time = clock();

    double x1, x2;
    if (argc == 3) {
        x1 = strtod(argv[1], NULL);
        x2 = strtod(argv[2], NULL);
        printf("argus");
    }
    else {
        x1 = 0;
        x2 = 10;
    }

    double integral = integ(f3, x1, x2, 50000);

    printf("Result: %.3f\n", integral);

    double execution_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
    printf("Execution time: %f", execution_time);

    return 0;
}

/* Terminal representation size may be adjusted with GRAPHIC_WIDTH and  GRAPHIC_HEIGHT. */
#define GRAPHIC_WIDTH 181
#define GRAPHIC_HEIGHT 21

/* Range function. Takes an array (double *range) and fills it with (int len) equispaced doubles between (double start) and (double end). */
void range(double *range, double start, double end, int len) {

    double lenf, dif, val;
    lenf = (double) (len - 1);
    dif = (end-start)/(lenf);
    val = start;
    for (int i = 0; (*(range+i) = val) <= end; val += dif, i++) ;
}

/* Takes the pointer to a mathematical function (double (*f)(double)) and fills the given double array (double *image) with the image of the doubles in the array (double *range) with given length (int len). */
void image(double (*f)(double), double *range, double *image, int len) {

    for (double *pr = range, *pi = image; pr - range < len; *pi++ = f(*pr++)) ;
}

/* Fills a GRAPHIC_WIDTHxGRAPHIC_HEIGHT null 2D array (int graphic[][]) with characters so, when printed, will create a graphic based on the (int len) coordinates (double *xvals), (double *yvals). Includes axes. */
double createGraphic(int graphic[][GRAPHIC_WIDTH], double *xvals, double *yvals, int len) {

    double topx = maxel(xvals, len);
    double topy = maxel(yvals, len);
    double bottomx = minel(xvals, len);
    double bottomy = minel(yvals, len);
    int scale_index = len/GRAPHIC_WIDTH;
    int row0 = GRAPHIC_HEIGHT-1 - floor((-bottomy)/(topy-bottomy)*(GRAPHIC_HEIGHT-1) + .5);
    printf("x: [%f, %f]\ny: [%f, %f]\n", bottomx, topx, bottomy, topy);

    for (int i = 0; i < GRAPHIC_HEIGHT; i++)  {
        **(graphic + i) = 179;
        for (int j = 1; j < GRAPHIC_WIDTH; *(*(graphic + i) + j++) = ' ') ;
    }
    *(*(graphic + row0)) = 197;
    for (int j = 1; j < GRAPHIC_WIDTH; *(*(graphic + row0) + j++) = 196) ;

    for (int pi = 0; pi < len; pi+=scale_index) {
        double x = *(xvals+pi);
        double y = *(yvals+pi);
        int gx = floor((x-bottomx)/(topx-bottomx)*(GRAPHIC_WIDTH-1) + .5);
        int gy = GRAPHIC_HEIGHT-1 - floor((y-bottomy)/(topy-bottomy)*(GRAPHIC_HEIGHT-1) + .5);

        // printf("pi: %d -> (%f, %f) > (%d, %d)\n",pi, x, y , gx, gy);
        
        graphic[gy][gx] = 254;
    }

    return topx;
}

/* Takes the mathematical function (double (*f)(double)), the x and y values to represent (double *xvals, double *yvals) and their lenght (int len) and represents them on the terminal. */
int represent(double (*f)(double), double *xvals, double *yvals, int len) {

    int graphic[GRAPHIC_HEIGHT][GRAPHIC_WIDTH];

    double topx = createGraphic(graphic, xvals, yvals, len);
    double topy = f(topx);

    printf("y\n");
    printf("^\n");
    for (int i = 0; i < GRAPHIC_HEIGHT; i++) {
        for (int j = 0; j< GRAPHIC_WIDTH; j++) {
            putchar(graphic[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

/* Takes the mathematical function (double (*f)(double)), the limits of the integral (double from, double to) and the number of divisions of the numerical aproximation. Uses the midpoint rule. It also represents the function. */
double integ(double (*f) (double), double from, double to, int difs) {

    int len = difs+1;
    double diff = (to - from)/difs;

    double xvalues[len], yvalues[len];

    range(xvalues, from, to, len);
    image(f, xvalues, yvalues, len);
    represent(f, xvalues, yvalues, len);

    double result = .0;

    for (int i = 0; i < len; i++) {
        result += diff*yvalues[i];
    }

    return result;
}
