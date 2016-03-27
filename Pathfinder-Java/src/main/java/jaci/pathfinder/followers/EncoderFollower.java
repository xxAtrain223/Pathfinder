package jaci.pathfinder.followers;

import jaci.pathfinder.PathfinderJNI;
import jaci.pathfinder.Trajectory;

public class EncoderFollower {

    public class EncoderConfig {
        int initial_position, ticks_per_revolution;
        double wheel_circumference, kp, ki, kd, kv, ka;
    }

    public class FollowerContainer {
        double last_error, heading, output;
        int segment, finished;
    }

    Trajectory trajectory;
    EncoderConfig config;
    FollowerContainer container;

    public EncoderFollower(Trajectory trajectory) {
        this.trajectory = trajectory;
        config = new EncoderConfig();
        container = new FollowerContainer();
    }

    public void setTrajectory(Trajectory trajectory) {
        this.trajectory = trajectory;
        reset();
    }

    public void reset() {
        container = new FollowerContainer();
    }
    
    public void configurePIDVA(double kp, double ki, double kd, double kv, double ka) {
        config.kp = kp; config.ki = ki; config.kd = kd;
        config.kv = kv; config.ka = ka;
    }

    public void configureEncoder(int initial_position, int ticks_per_revolution, double wheel_diameter) {
        config.initial_position = initial_position;
        config.ticks_per_revolution = ticks_per_revolution;
        config.wheel_circumference = Math.PI * wheel_diameter;
    }
    
    public double calculate(int encoder_position) {
        PathfinderJNI.encoderFollow(encoder_position, trajectory.segments, config, container);
        return container.output;
    }

    public double getOutput() {
        return container.output;
    }

    public Trajectory.Segment getSegment() {
        return trajectory.get(container.segment);
    }
    
    public boolean isFinished() {
        return container.finished == 1;
    }

}
