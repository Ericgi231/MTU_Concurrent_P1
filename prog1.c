/* ----------------------------------------------------------- */
/* NAME : Eric Grant                         User ID: ewgrant  */
/* DUE DATE : 03/19/2000                                       */
/* PROGRAM ASSIGNMENT 1                                        */
/* FILE NAME : prog1.c                                         */
/* PROGRAM PURPOSE :                                           */
/*    Using fork(), wait(), and exit()                         */
/* ----------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

/* ----------------------------------------------------------- */
/* FUNCTION  fibonacci :                                       */
/*     take an index in the fibbonacci sequence and return     */
/*     the number at that index                                */
/* PARAMETER USAGE :                                           */
/*    n is an index in the fibonacci sequence                  */
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
long fibonacci(long n) {
    //base case
    if (n <= 1) {
        return n;
    }
    //recursion
    return fibonacci(n-1) + fibonacci(n-2);
}

/* ----------------------------------------------------------- */
/* FUNCTION  buffonsNeedle :                                   */
/*     take an iteration count and run buffon's needle problem */
/*     that many times, return ratio of throws passed the line */
/* PARAMETER USAGE :                                           */
/*    r is the number of times to run the problem              */
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
double buffonsNeedle(double r) {
    //vars
    time_t t;
    srand((unsigned) time(&t));

    double pi = acos(-1.0);
    double d, a;

    //main loop
    int wins = 0;
    for (int i = 0; i < r; i++) {
        d = (rand() / (double)RAND_MAX);
        a = (rand() / (double)RAND_MAX) * 2 * pi;
        if (d + 1 * sin(a) < 0 || d + 1 * sin(a) > 1) {
            wins++;
        }
    }

    return (double) wins / r;
}

/* ----------------------------------------------------------- */
/* FUNCTION  ellipseArea :                                     */
/*     determine number of points in ellipse                   */
/* PARAMETER USAGE :                                           */
/*    a is the vertical bound                                  */
/*    b is the horizontal bound                                */
/*    s is the iterations to run                               */
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
double ellipseArea(double a, double b, double s) {
    //vars
    time_t t;
    srand((unsigned) time(&t));

    double pi = acos(-1.0);
    double x, y;

    //main loop
    int inside = 0;
    for (int i = 0; i < s; i++) {
        x = rand() / (double)RAND_MAX * a;
        y = rand() / (double)RAND_MAX * b;
        if ((pow(x,2) / pow(a,2) + pow(y,2) / pow(b,2)) <= 1) {
            inside++;
        }
    }

    return inside;
}

/* ----------------------------------------------------------- */
/* FUNCTION  dropBalls :                                       */
/*     drop balls on a peg board and count balls in each bin   */
/*     also print histogram of results                         */
/* PARAMETER USAGE :                                           */
/*    x is the number of bins                                  */
/*    y is the number of balls                                 */
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
void dropBalls(int x, int y) {
    //vars
    time_t t;
    srand((unsigned) time(&t));

    double ball, percent, max;
    long count, starsCount;
    int binNum;
    char buf[100], stars[50];

    //fill bins with 0 balls
    double bins[x];
    for (int n = 0; n < x; n++) {
        bins[n] = 0;
    }

    //drop balls into bins
    for (int i = 0; i < y; i++) {
        ball = (double)(x-1)/2;
        for (int j = 0; j < x-1; j++) {
            if (rand() / (double)RAND_MAX > 0.5) {
                ball += 0.5;
            } else {
                ball -= 0.5;
            }
        }

        bins[(int)ball]++;
    }

    //find bin with largest percent of balls
    max = 0;
    for (int i = 0; i < x; i++)
    {
        count = bins[i];
        percent = ((double)count / (double)y) * 100.0;
        if (percent > max) {
            max = percent;
        }
    }

    //print histogram
    for (int i = 0; i < x; i++)
    {
        binNum = i+1;
        count = bins[i];
        percent = ((double)count / (double)y) * 100.0;
        starsCount = (long)((percent*50.0/max) + .5);
        strcpy(stars, "");
        for (int n = 0; n < starsCount; n++)
        {
            strcat(stars, "*");
        }
        sprintf(buf, "%3d-(%7ld)-(%5.2lf%%)|%s\n",binNum, count, percent, stars);
        write(1, buf, strlen(buf));
    }
    
    return;
}

/* ----------------------------------------------------------- */
/* FUNCTION  main :                                            */
/*     the main function                                       */
/* PARAMETER USAGE :                                           */
/*    argc is the number of arguments taken                    */
/*    argv[] is the list of arguments                          */
/* FUNCTION CALLED :                                           */
/*    n/a                                                      */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
    //Argument handling
    //
    //return if not correct amount of args
    if (argc != 8) {
        printf("Invalid Arguments");
        return 0;
    }

    int nums[7];
    //process arguments
    for (int i = 0; i < argc-1; i++) {
        nums[i] = atoi(argv[i+1]);
    }

    //vars
    //
    pid_t pid;
    char buf[100];
    int status;

    //parent printing
    //
    sprintf(buf, "Main Process Started\n");
    write(1, buf, strlen(buf));

    sprintf(buf, "Fibonacci Input\t\t\t= %d\n", nums[0]);
    write(1, buf, strlen(buf));
    
    sprintf(buf, "Buffon's Needle Iterations\t= %d\n", nums[1]);
    write(1, buf, strlen(buf));

    sprintf(buf, "Total random Number Pairs\t= %d\n", nums[2]);
    write(1, buf, strlen(buf));

    sprintf(buf, "Semi-Major Axis Length\t\t= %d\n", nums[3]);
    write(1, buf, strlen(buf));

    sprintf(buf, "Semi-Minor Axis Length\t\t= %d\n", nums[4]);
    write(1, buf, strlen(buf));

    sprintf(buf, "Number of Bins\t\t\t= %d\n", nums[5]);
    write(1, buf, strlen(buf));

    sprintf(buf, "Number of Ball Droppings\t= %d\n", nums[6]);
    write(1, buf, strlen(buf));

    //fib
    //
    pid = fork();
    if (pid > 0) {
        sprintf(buf, "Fibonacci Process Created\n");
        write(1, buf, strlen(buf));
    } else if (pid == 0) {
        sprintf(buf, "   Fibonacci Process Started\n");
        write(1, buf, strlen(buf));
        
        sprintf(buf, "   Input Number %d\n", nums[0]);
        write(1, buf, strlen(buf));
        
        sprintf(buf, "   Fibonacci Number f(%d) is %ld\n", nums[0], fibonacci(nums[0]));
        write(1, buf, strlen(buf));
        
        sprintf(buf, "   Fibonacci Process Exits\n");
        write(1, buf, strlen(buf));
        
        exit(0);
    }

    //buffons needle
    //
    pid = fork();
    if (pid > 0) {
        sprintf(buf, "Buffon's Needle Process Created\n");
        write(1, buf, strlen(buf));
    } else if (pid == 0) {
        sprintf(buf, "      Buffon's Needle Process Started\n");
        write(1, buf, strlen(buf));
        
        sprintf(buf, "      Input Number %d\n", nums[1]);
        write(1, buf, strlen(buf));
        
        sprintf(buf, "      Estimated Probability is %.5lf\n", buffonsNeedle(nums[1]));
        write(1, buf, strlen(buf));
        
        sprintf(buf, "      Buffon's Needle Process Exits\n");
        write(1, buf, strlen(buf));
        
        exit(0);
    }

    //ellipse
    //
    pid = fork();
    if (pid > 0) {
        sprintf(buf, "Ellipse Area Process Created\n");
        write(1, buf, strlen(buf));
    } else if (pid == 0) {
        sprintf(buf, "         Ellipse Area Process Started\n");
        write(1, buf, strlen(buf));
        
        sprintf(buf, "         Total random Number Pairs %d\n", nums[4]);
        write(1, buf, strlen(buf));
        
        sprintf(buf, "         Semi-Major Axis Length %d\n", nums[2]);
        write(1, buf, strlen(buf));
        
        sprintf(buf, "         Semi-Minor Axis Length %d\n", nums[3]);
        write(1, buf, strlen(buf));
        
        long value =  ellipseArea(nums[2], nums[3], nums[4]);
        double pi = acos(-1.0);
        double estArea = (((double)value / nums[4]) * nums[2] * nums[3]) * 4;
        double area = (pi * nums[2] * nums[3]);

        sprintf(buf, "         Total Hits %ld\n", value);
        write(1, buf, strlen(buf));

        sprintf(buf, "         Estimated Area is %.5lf\n", estArea);
        write(1, buf, strlen(buf));

        sprintf(buf, "         Actual Area is %.5lf\n", area);
        write(1, buf, strlen(buf));

        sprintf(buf, "         Ellipse Area Process Exits\n");
        write(1, buf, strlen(buf));

        exit(0);
    }

    //balls
    //
    pid = fork();
    if (pid > 0) {
        sprintf(buf, "Simple Pinball Process Created\n");
        write(1, buf, strlen(buf));
    } else if (pid == 0) {
        sprintf(buf, "Simple Pinball Process Started\n");
        write(1, buf, strlen(buf));
        
        sprintf(buf, "Number of Bins %d\n", nums[5]);
        write(1, buf, strlen(buf));
        
        sprintf(buf, "Number of Ball Droppings %d\n", nums[6]);
        write(1, buf, strlen(buf));

        //printing done inside function because array of data needed
        dropBalls(nums[5], nums[6]);

        sprintf(buf, "Simple Pinball Process Exits\n");
        write(1, buf, strlen(buf));

        exit(0);
    }

    //wait to exit
    //
    sprintf(buf, "Main Process Waits\n");
    write(1, buf, strlen(buf));

    for (int i = 0; i < 4; i++) {
        wait(&status);
    }

    sprintf(buf, "Main Process Exits\n");
    write(1, buf, strlen(buf));

    return 0;
}