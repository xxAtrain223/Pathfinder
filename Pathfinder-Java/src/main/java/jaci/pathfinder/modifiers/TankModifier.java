package jaci.pathfinder.modifiers;

import jaci.pathfinder.PathfinderJNI;
import jaci.pathfinder.Trajectory;

public class TankModifier {

    Trajectory source, left, right;

    public TankModifier(Trajectory source) {
        this.source = source;
    }

    public TankModifier modify(double wheelbase_width) {
        Trajectory[] trajs = PathfinderJNI.modifyTrajectoryTank(source, wheelbase_width);
        left = trajs[0];
        right = trajs[1];
        return this;
    }

    public Trajectory getSourceTrajectory() {
        return source;
    }

    public Trajectory getLeftTrajectory() {
        return left;
    }

    public Trajectory getRightTrajectory() {
        return right;
    }

}
