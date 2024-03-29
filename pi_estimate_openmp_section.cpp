/*
 * piestsection.cpp
 *
 *  Created on: Oct 12, 2019
 *      Author: raghak
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <omp.h>
#include <math.h>
#define		num_of_points	   40000000

typedef struct{
	double x;
	double y;
}Point;
using namespace std;

int main(void){
	int i;
	int NUM_THREADS=2;
	int num_of_points_in_circle;
	double pi;
	struct timespec start, stop;
	double time;
	Point * data_point = (Point *) malloc (sizeof(Point)*num_of_points);
	for(i=0; i<num_of_points; i++){
		data_point[i].x=(double)rand()/(double)RAND_MAX;
		data_point[i].y=(double)rand()/(double)RAND_MAX;
	}
	num_of_points_in_circle=0;
	omp_set_num_threads(NUM_THREADS);
	int th_id;
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}

	#pragma omp parallel private(th_id)
	{
		#pragma omp sections private(i) reduction (+:num_of_points_in_circle)
		{
			#pragma omp section
			{
				for(i=0; i<num_of_points/NUM_THREADS; i++)
				{
					if((data_point[i].x-0.5)*(data_point[i].x-0.5)+(data_point[i].y-0.5)*(data_point[i].y-0.5)<=0.25)
					{
							num_of_points_in_circle++;
					}
				}
			}
			#pragma omp section
			{
				for(i=num_of_points/NUM_THREADS; i<num_of_points; i++)
				{
					if((data_point[i].x-0.5)*(data_point[i].x-0.5)+(data_point[i].y-0.5)*(data_point[i].y-0.5)<=0.25)
					{
							num_of_points_in_circle++;
					}
				}
			}
		}

	}

	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}
	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;

	pi =4*(double)(num_of_points_in_circle)/(double)num_of_points;
	cout<<"Estimated pi is "<<pi<<", execution time = "<<time<<" sec\n";
	return 0;
}

