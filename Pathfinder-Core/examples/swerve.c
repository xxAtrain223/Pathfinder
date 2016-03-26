#include <pathfinder.h>

int main() {
    int POINT_LENGTH = 3;

    Waypoint points[POINT_LENGTH];

    Waypoint p1 = { -4, -1, d2r(45) };      // Waypoint @ x=-4, y=-1, exit angle=45 degrees
    Waypoint p2 = { -1, 2, 0 };             // Waypoint @ x=-1, y= 2, exit angle= 0 radians
    Waypoint p3 = {  2, 4, 0 };             // Waypoint @ x= 2, y= 4, exit angle= 0 radians
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    
    TrajectoryCandidate candidate;
    pathfinder_prepare(points, POINT_LENGTH, FIT_HERMITE_CUBIC, PATHFINDER_SAMPLES_HIGH, 0.001, 15.0, 10.0, 60.0, &candidate);

    int length = candidate.length;
    Segment trajectory[length];
    
    pathfinder_generate(&candidate, seg);
    
    Segment frontLeft[length];
    Segment frontRight[length];
    Segment backLeft[length];
    Segment backRight[length];
    
    double wheelbase_width = 0.6;
    double wheelbase_depth = 0.5;
    
    SWERVE_MODE mode = SWERVE_DEFAULT;

    pathfinder_modify_swerve(trajectory, length, frontLeft, frontRight, backLeft, backRight, wheelbase_width, wheelbase_depth, mode);
    
    // Do something with the trajectories...
    return 0;
}