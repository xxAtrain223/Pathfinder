#ifndef PATHFINDER_IO_H_DEF
#define PATHFINDER_IO_H_DEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void intToBytes(int n, char *bytes);
int bytesToInt(char *bytes);
void longToBytes(unsigned long long n, char *bytes);
unsigned long long bytesToLong(char *bytes);
double longToDouble(unsigned long long l);
unsigned long long doubleToLong(double d);
void doubleToBytes(double n, char *bytes);
double bytesToDouble(char *bytes);

void pathfinder_serialize(FILE *fp, Segment *trajectory, int trajectory_length);

int pathfinder_deserialize_pre(FILE *fp);
void pathfinder_deserialize(FILE *fp, Segment *target, int trajectory_length);

#endif