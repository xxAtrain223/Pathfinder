package jaci.pathfinder;

/**
 * The main class of the Pathfinder Library. The Pathfinder Library is used for Motion Profile and Trajectory Generation.
 *
 * This class contains some of the most common methods you will use when doing Motion Profiling
 *
 * @author Jaci
 */
public class Pathfinder {

    /**
     * Convert degrees to radians. This is included here for static imports. In this library, all angle values are
     * given in radians
     */
    public static double d2r(double degrees) {
        return Math.toRadians(degrees);
    }

    /**
     * Generate a motion profile trajectory using the given waypoints and configuration.
     * @param waypoints     An array of waypoints (setpoints) for the trajectory path to intersect
     * @param config        The configuration of the trajectory, including max velocity, acceleration, jerk
     *                      and other values such as time scale and fit method
     * @return              The generated trajectory (an array of segments)
     */
    public static Trajectory generate(Waypoint[] waypoints, Trajectory.Config config) {
        return PathfinderJNI.generateTrajectory(waypoints, config);
    }

}
