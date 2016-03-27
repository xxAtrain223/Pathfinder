package jaci.pathfinder.followers;

import jaci.pathfinder.PathfinderJNI;
import jaci.pathfinder.Trajectory;

public class EncoderFollower {

    int encoder_offset, encoder_tick_count;
    double wheel_circumference;

    double kp, ki, kd, kv, ka;

    double last_error, heading;

    int segment;
    Trajectory trajectory;

    public EncoderFollower(Trajectory traj) {
        this.trajectory = traj;
    }

    public void setTrajectory(Trajectory traj) {
        this.trajectory = traj;
        reset();
    }

    public void configurePIDVA(double kp, double ki, double kd, double kv, double ka) {
        this.kp = kp; this.ki = ki; this.kd = kd;
        this.kv = kv; this.ka = ka;
    }

    public void configureEncoder(int initial_position, int ticks_per_revolution, double wheel_diameter) {
        encoder_offset = initial_position;
        encoder_tick_count = ticks_per_revolution;
        wheel_circumference = Math.PI * wheel_diameter;
    }

    public void reset() {
        last_error = 0; segment = 0;
    }

    public double calculate(int encoder_tick) {
        // Number of Revolutions * Wheel Circumference
        double distance_covered = ((double)(encoder_tick - encoder_offset) / encoder_tick_count)
                * wheel_circumference;
        if (segment < trajectory.length()) {
            Trajectory.Segment seg = trajectory.get(segment);
            double error = seg.position - distance_covered;
            double calculated_value =
                    kp * error +                                    // Proportional
                    kd * ((error - last_error) / seg.dt) +          // Derivative
                    (kv * seg.velocity + ka * seg.acceleration);    // V and A Terms
            last_error = error;
            heading = seg.heading;
            segment++;

            return calculated_value;
        } else return 0;
    }

    public Trajectory.Segment getSegment() {
        return trajectory.get(segment);
    }

    public boolean isFinished() {
        return segment >= trajectory.length();
    }

}
