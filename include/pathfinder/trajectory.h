#ifndef PATHFINDER_TRAJECTORY_H_DEF
#define PATHFINDER_TRAJECTORY_H_DEF

int pathfinder_prepare(Waypoint *path, int path_length, void (*fit)(Waypoint,Waypoint,Spline*), int sample_count, double dt,
        double max_velocity, double max_acceleration, double max_jerk, TrajectoryCandidate *cand);
int pathfinder_generate(TrajectoryCandidate *c, Segment *segments);

void pf_trajectory_copy(Segment *src, Segment *dest, int length);

TrajectoryInfo pf_trajectory_prepare(TrajectoryConfig c);
void pf_trajectory_create(TrajectoryInfo info, TrajectoryConfig c, Segment *seg);
void pf_trajectory_fromSecondOrderFilter(int filter_1_l, int filter_2_l, 
        double dt, double u, double v, double impulse, int len, Segment *t);

#endif