#include <pathfinder.h>

int main() {
    int POINT_LENGTH = 3;

    Waypoint* points = (Waypoint*)malloc(sizeof(Waypoint) * POINT_LENGTH);

    Waypoint p1 = { -4, -1, d2r(45) };      // Waypoint @ x=-4, y=-1, exit angle=45 degrees
    Waypoint p2 = { -1, 2, 0 };             // Waypoint @ x=-1, y= 2, exit angle= 0 radians
    Waypoint p3 = {  2, 4, 0 };             // Waypoint @ x= 2, y= 4, exit angle= 0 radians
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    
    TrajectoryCandidate candidate;
    pathfinder_prepare(points, POINT_LENGTH, FIT_HERMITE_CUBIC, PATHFINDER_SAMPLES_HIGH, 0.001, 15.0, 10.0, 60.0, &candidate);

    int length = candidate.length;
    Segment *trajectory = malloc(length * sizeof(Segment));
    
    pathfinder_generate(&candidate, trajectory);
    
    Segment* leftTrajectory = (Segment*)malloc(sizeof(Segment) * length);
    Segment* rightTrajectory = (Segment*)malloc(sizeof(Segment) * length);
    
    double wheelbase_width = 0.6;

    pathfinder_modify_tank(trajectory, length, leftTrajectory, rightTrajectory, wheelbase_width);

    // Do something with the trajectories...
    // We'll print out the segments

    int i;
    for (i = 0; i < length; i++) {
        Segment s = trajectory[i];
        printf("Segment: %d\n", i);
        printf("Time Step: %f\n", s.dt);
        printf("Coords: (%f, %f)\n", s.x, s.y);
        printf("Position (Distance): %f\n", s.position);
        printf("Velocity: %f\n", s.velocity);
        printf("Acceleration: %f\n", s.acceleration);
        printf("Jerk (Acceleration per Second): %f\n", s.jerk);
        printf("Heading (radians): %f\n", s.heading);
        printf("\n");
    }
    
    free(trajectory);
    return 0;
}
