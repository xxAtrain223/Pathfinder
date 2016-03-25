package jaci.pathfinder;

public class Pathfinder {

    public static double d2r(double degrees) {
        return Math.toRadians(degrees);
    }

    public static Trajectory generate(Waypoint[] waypoints, Trajectory.Config config) {
        return PathfinderJNI.generateTrajectory(waypoints, config);
    }

}
