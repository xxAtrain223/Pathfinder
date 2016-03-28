#include "pathfinder.h"

// CONVERSIONS //
// We're using the manual stuff instead of fwrite on a double array so these files
//    can be preserved on different endianess systems.

void intToBytes(int n, char *bytes) {
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = n & 0xFF;
}

int bytesToInt(char *bytes) {
    int value = 0;
    int i;
    for (i = 0; i < 4; i++) {
        int shift = (4 - 1 - i) * 8;
        value += (bytes[i] & 0x000000FF) << shift;
    }
    return value;
}

void longToBytes(unsigned long long n, char *bytes) {
    bytes[0] = (n >> 56) & 0xFF;
    bytes[1] = (n >> 48) & 0xFF;
    bytes[2] = (n >> 40) & 0xFF;
    bytes[3] = (n >> 32) & 0xFF;
    bytes[4] = (n >> 24) & 0xFF;
    bytes[5] = (n >> 16) & 0xFF;
    bytes[6] = (n >> 8) & 0xFF;
    bytes[7] = n & 0xFF;
}

unsigned long long bytesToLong(char *bytes) {
    unsigned long long value = 0;
    int i;
    for (i = 0; i < 8; i++) {
        int shift = (8 - 1 - i) * 8;
        value += (unsigned long long)(((unsigned long long)(bytes[i] & 0xFF)) << shift);
    }
    return value;
}

double longToDouble(unsigned long long l) {
    double result;
    memcpy(&result, &l, 8);
    return result;
}

unsigned long long doubleToLong(double d) {
    unsigned long long result;
    memcpy(&result, &d, 8);
    return result;
}

void doubleToBytes(double n, char *bytes) {
    longToBytes(doubleToLong(n), bytes);
}

double bytesToDouble(char *bytes) {
    return longToDouble(bytesToLong(bytes));
}

// FUNCTIONS //

void pathfinder_serialize(FILE *fp, Segment *trajectory, int trajectory_length) {
    char buf_1[4];
    intToBytes(trajectory_length, buf_1);
    fwrite(buf_1, 1, 4, fp);
        
    char buf[8];
    int i;
    for (i = 0; i < trajectory_length; i++) {
        Segment s = trajectory[i];
        
        doubleToBytes(s.dt, buf);
        fwrite(buf, 1, 8, fp);
                
        doubleToBytes(s.x, buf);
        fwrite(buf, 1, 8, fp);
        
        doubleToBytes(s.y, buf);
        fwrite(buf, 1, 8, fp);
        
        doubleToBytes(s.position, buf);
        fwrite(buf, 1, 8, fp);
        
        doubleToBytes(s.velocity, buf);
        fwrite(buf, 1, 8, fp);
        
        doubleToBytes(s.acceleration, buf);
        fwrite(buf, 1, 8, fp);
        
        doubleToBytes(s.jerk, buf);
        fwrite(buf, 1, 8, fp);
        
        doubleToBytes(s.heading, buf);
        fwrite(buf, 1, 8, fp);
    }
}

int pathfinder_deserialize_pre(FILE *fp) {
    char buf_1[4];
    fread(buf_1, 1, 4, fp);
    return bytesToInt(buf_1);
}

void pathfinder_deserialize(FILE *fp, Segment *target, int trajectory_length) {
    char buf[8];
    
    int i;
    for (i = 0; i < trajectory_length; i++) {
        fread(buf, 1, 8, fp);
        double dt = bytesToDouble(buf);
                
        fread(buf, 1, 8, fp);
        double x = bytesToDouble(buf);
        
        fread(buf, 1, 8, fp);
        double y = bytesToDouble(buf);
        
        fread(buf, 1, 8, fp);
        double position = bytesToDouble(buf);
        
        fread(buf, 1, 8, fp);
        double velocity = bytesToDouble(buf);
        
        fread(buf, 1, 8, fp);
        double acceleration = bytesToDouble(buf);
        
        fread(buf, 1, 8, fp);
        double jerk = bytesToDouble(buf);
        
        fread(buf, 1, 8, fp);
        double heading = bytesToDouble(buf);
        
        Segment s = { dt, x, y, position, velocity, acceleration, jerk, heading };
        target[i] = s;
    }
}