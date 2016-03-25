package jaci.pathfinder;

public class Trajectory {

    public static class Config {
        public FitMethod fit;
        public int sample_count;
        public double dt, max_velocity, max_acceleration, max_jerk;

        public Config(FitMethod fit, int samples, double dt, double max_velocity, double max_acceleration, double max_jerk) {
            this.fit = fit;
            this.sample_count = samples;
            this.dt = dt;
            this.max_velocity = max_velocity;
            this.max_acceleration = max_acceleration;
            this.max_jerk = max_jerk;
        }
    }

    public static class Segment {
        public double dt, x, y, position, velocity, acceleration, jerk, heading;

        public Segment(double dt, double x, double y, double position, double velocity, double acceleration, double jerk, double heading) {
            this.dt = dt;
            this.x = x;
            this.y = y;
            this.position = position;
            this.velocity = velocity;
            this.acceleration = acceleration;
            this.jerk = jerk;
            this.heading = heading;
        }

        public Segment copy() {
            return new Segment(dt, x, y, position, velocity, acceleration, jerk, heading);
        }
    }

    public static enum FitMethod {
        HERMITE_CUBIC, HERMITE_QUINTIC;
    }

    public Segment[] segments;

    public Trajectory(Segment[] segments) {
        this.segments = segments;
    }

    public Trajectory(int length) {
        this.segments = new Segment[length];
    }

    public Segment get(int index) {
        return segments[index];
    }

    public int length() {
        return segments.length;
    }

    public Trajectory copy() {
        Trajectory toCopy = new Trajectory(length());
        for (int i = 0; i < length(); i++) {
            toCopy.segments[i] = get(i).copy();
        }
        return toCopy;
    }

}
