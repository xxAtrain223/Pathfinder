#include <jni.h>
#include <stdlib.h>
#include <pathfinder.h>

#include "jaci_pathfinder_PathfinderJNI.h"

typedef void (*fitmethod)(Waypoint,Waypoint,Spline*);

void *getField(JNIEnv *env, jobject obj, char *field_name, char *field_signature) {
    jclass cls = (*env)->GetObjectClass(env, obj);
    jfieldID fid = (*env)->GetFieldID(env, cls, field_name, field_signature);
    return (*env)->GetObjectField(env, obj, fid);
}

double getDoubleField(JNIEnv *env, jobject obj, char *field_name) {
    jclass cls = (*env)->GetObjectClass(env, obj);
    jfieldID fid = (*env)->GetFieldID(env, cls, field_name, "D");
    double x = (*env)->GetDoubleField(env, obj, fid);
    return x;
}

int getIntField(JNIEnv *env, jobject obj, char *field_name) {
    jclass cls = (*env)->GetObjectClass(env, obj);
    jfieldID fid = (*env)->GetFieldID(env, cls, field_name, "I");
    int x = (*env)->GetIntField(env, obj, fid);
    return x;
}

void setDoubleField(JNIEnv *env, jobject obj, char *field_name, double value) {
    jclass cls = (*env)->GetObjectClass(env, obj);
    jfieldID fid = (*env)->GetFieldID(env, cls, field_name, "D");
    (*env)->SetDoubleField(env, obj, fid, value);
}

void setIntField(JNIEnv *env, jobject obj, char *field_name, int value) {
    jclass cls = (*env)->GetObjectClass(env, obj);
    jfieldID fid = (*env)->GetFieldID(env, cls, field_name, "I");
    (*env)->SetIntField(env, obj, fid, value);
}

void *getMethodReturn(JNIEnv *env, jobject obj, char *method_name, char *method_signature) {
    jclass cls = (*env)->GetObjectClass(env, obj);
    jmethodID mid = (*env)->GetMethodID(env, cls, method_name, method_signature);
    return (*env)->CallObjectMethod(env, obj, mid);
}

fitmethod getFitMethod(JNIEnv *env, jobject obj) {
    jstring s = (jstring) getMethodReturn(env, obj, "name", "()Ljava/lang/String;");
    const char *nativeString = (*env)->GetStringUTFChars(env, s, 0);
    
    fitmethod method;
    
    if (strcmp(nativeString, "FIT_HERMITE_CUBIC") == 0) {
        method = FIT_HERMITE_CUBIC;
    } else if (strcmp(nativeString, "FIT_HERMITE_QUINTIC") == 0) {
        method = FIT_HERMITE_QUINTIC;
    } else {
        method = FIT_HERMITE_CUBIC;
    }
    
    (*env)->ReleaseStringUTFChars(env, s, nativeString);
    return method;
}

SWERVE_MODE getSwerveMode(JNIEnv *env, jobject obj) {
    jstring s = (jstring) getMethodReturn(env, obj, "name", "()Ljava/lang/String;");
    const char *nativeString = (*env)->GetStringUTFChars(env, s, 0);
    
    SWERVE_MODE mode;
    
    if (strcmp(nativeString, "SWERVE_DEFAULT")) {
        mode = SWERVE_DEFAULT;
    } else {
        mode = SWERVE_DEFAULT;
    }
    
    (*env)->ReleaseStringUTFChars(env, s, nativeString);
    return mode;
}

/*
 * Class:     jaci_pathfinder_PathfinderJNI
 * Method:    generateTrajectory
 * Signature: ([Ljaci/pathfinder/Waypoint;Ljaci/pathfinder/Trajectory/FitMethod;IDDDD)[Ljaci/pathfinder/Trajectory/Segment;
 *
 * RETURN: Trajectory Object
 * PARAMS:
 *  waypoints: Array of Waypoints to generate with
 *  config:    Configuration for Trajectory Generation
 *      - fit, samples, dt, max_velocity, max_acceleration, max_jerk
 */
JNIEXPORT jobjectArray JNICALL Java_jaci_pathfinder_PathfinderJNI_generateTrajectory
    (JNIEnv *env, jclass thisCls, jobjectArray waypoints, jobject fit, jint samples, jdouble dt, jdouble max_velocity, jdouble max_acceleration, jdouble max_jerk) {
    
    int point_length = (*env)->GetArrayLength(env, waypoints);
    int i;
    Waypoint *points = malloc(point_length * sizeof(Waypoint));
    
    for (i = 0; i < point_length; i++) {
        jobject wpobj = (jobject) (*env)->GetObjectArrayElement(env, waypoints, i);
        Waypoint p = {
            getDoubleField(env, wpobj, "x"),
            getDoubleField(env, wpobj, "y"),
            getDoubleField(env, wpobj, "angle")
        };
        points[i] = p;
    }
    
    TrajectoryCandidate cd;
    fitmethod fitm = getFitMethod(env, fit);
    pathfinder_prepare(points, point_length, fitm, samples, dt, max_velocity, max_acceleration, max_jerk, &cd);
    
    int len = cd.length;
    // Segment segs[len];
    Segment *segs = malloc(len * sizeof(Segment));
    
    pathfinder_generate(&cd, segs);
    
    jclass cls = (*env)->FindClass(env, "jaci/pathfinder/Trajectory$Segment");
    jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "(DDDDDDDD)V");
    
    jobjectArray newArray = (*env)->NewObjectArray(env, len, cls, NULL);
    
    for (i = 0; i < len; i++) {
        Segment s = segs[i];
        jobject jseg = (*env)->NewObject(env, cls, constructor, s.dt, s.x, s.y, s.position, s.velocity, s.acceleration, s.jerk, s.heading);
        (*env)->SetObjectArrayElement(env, newArray, i, jseg);
    }
    
    free(points); free(segs);
    
    return newArray;
}

/*
 * Class:     jaci_pathfinder_PathfinderJNI
 * Method:    modifyTrajectoryTank
 * Signature: ([Ljaci/pathfinder/Trajectory/Segment;D)[[Ljaci/pathfinder/Trajectory/Segment;
 *
 * RETURN: Array of trajectories (0 = Left, 1 = Right)
 * PARAMS:
 *  source:             Source Generated Trajectory
 *  wheelbase_width:    The width between individual sides of the wheelbase
 */
JNIEXPORT jobjectArray JNICALL Java_jaci_pathfinder_PathfinderJNI_modifyTrajectoryTank
  (JNIEnv *env, jclass thisCls, jobjectArray source, jdouble wheelbase_width) {
    
    int length = (*env)->GetArrayLength(env, source);
    // Segment segs[length];
    Segment *segs = malloc(length * sizeof(Segment));
    
    int i;
    for (i = 0; i < length; i++) {
        jobject sobj = (jobject) (*env)->GetObjectArrayElement(env, source, i);
        Segment s = {
            getDoubleField(env, sobj, "dt"),
            getDoubleField(env, sobj, "x"),
            getDoubleField(env, sobj, "y"),
            getDoubleField(env, sobj, "position"),
            getDoubleField(env, sobj, "velocity"),
            getDoubleField(env, sobj, "acceleration"),
            getDoubleField(env, sobj, "jerk"),
            getDoubleField(env, sobj, "heading")
        };
        segs[i] = s;
    }
    
    // Segment left[length];
    // Segment right[length];
    
    Segment *left = malloc(length * sizeof(Segment));
    Segment *right = malloc(length * sizeof(Segment));
    
    pathfinder_modify_tank(segs, length, left, right, wheelbase_width);
    
    jclass cls = (*env)->FindClass(env, "jaci/pathfinder/Trajectory$Segment");
    jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "(DDDDDDDD)V");
    
    jobjectArray leftArray = (*env)->NewObjectArray(env, length, cls, NULL);
    jobjectArray rightArray = (*env)->NewObjectArray(env, length, cls, NULL);
    
    for (i = 0; i < length; i++) {
        Segment l = left[i];
        Segment r = right[i];
        
        jobject jlseg = (*env)->NewObject(env, cls, constructor, l.dt, l.x, l.y, l.position, l.velocity, l.acceleration, l.jerk, l.heading);
        jobject jrseg = (*env)->NewObject(env, cls, constructor, r.dt, r.x, r.y, r.position, r.velocity, r.acceleration, r.jerk, r.heading);
        
        (*env)->SetObjectArrayElement(env, leftArray, i, jlseg);
        (*env)->SetObjectArrayElement(env, rightArray, i, jrseg);
    }
    
    free(segs); free(left); free(right);
    
    cls = (*env)->FindClass(env, "[Ljaci/pathfinder/Trajectory$Segment;");
    jobjectArray returnArray = (*env)->NewObjectArray(env, 2, cls, NULL);
    
    (*env)->SetObjectArrayElement(env, returnArray, 0, leftArray);
    (*env)->SetObjectArrayElement(env, returnArray, 1, rightArray);
    
    return returnArray;
}

/*
 * Class:     jaci_pathfinder_PathfinderJNI
 * Method:    modifyTrajectorySwerve
 * Signature: ([Ljaci/pathfinder/Trajectory/Segment;DDLjaci/pathfinder/modifiers/SwerveModifier/Mode;)[[Ljaci/pathfinder/Trajectory/Segment;
 * 
 * RETURN: Array of trajectories (0 = Front Left, 1 = Front Right, 2 = Back Left, 3 = Back Right)
 * PARAMS:
 *  source:             Source Generated Trajectory
 *  wheelbase_width:    The width between individual sides of the wheelbase
 *  wheelbase_depth:    The depth between the front and back of the wheelbases
 *  mode:               The mode to use to generate the new trajectories
 */
JNIEXPORT jobjectArray JNICALL Java_jaci_pathfinder_PathfinderJNI_modifyTrajectorySwerve
  (JNIEnv *env, jclass thisCls, jobjectArray source, jdouble wheelbase_width, jdouble wheelbase_depth, jobject mode) {
    
    int length = (*env)->GetArrayLength(env, source);
    // Segment segs[length];
    Segment *segs = malloc(length * sizeof(Segment));
    
    int i;
    for (i = 0; i < length; i++) {
        jobject sobj = (jobject) (*env)->GetObjectArrayElement(env, source, i);
        Segment s = {
            getDoubleField(env, sobj, "dt"),
            getDoubleField(env, sobj, "x"),
            getDoubleField(env, sobj, "y"),
            getDoubleField(env, sobj, "position"),
            getDoubleField(env, sobj, "velocity"),
            getDoubleField(env, sobj, "acceleration"),
            getDoubleField(env, sobj, "jerk"),
            getDoubleField(env, sobj, "heading")
        };
        segs[i] = s;
    }
    
    // Segment fl[length];
    // Segment fr[length];
    // Segment bl[length];
    // Segment br[length];
    
    Segment *fl = malloc(length * sizeof(Segment));
    Segment *fr = malloc(length * sizeof(Segment));
    Segment *bl = malloc(length * sizeof(Segment));
    Segment *br = malloc(length * sizeof(Segment));
    
    SWERVE_MODE smode = getSwerveMode(env, mode);
    pathfinder_modify_swerve(segs, length, fl, fr, bl, br, wheelbase_width, wheelbase_depth, smode);
    
    jclass cls = (*env)->FindClass(env, "jaci/pathfinder/Trajectory$Segment");
    jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "(DDDDDDDD)V");
    
    jobjectArray fla = (*env)->NewObjectArray(env, length, cls, NULL);
    jobjectArray fra = (*env)->NewObjectArray(env, length, cls, NULL);
    jobjectArray bla = (*env)->NewObjectArray(env, length, cls, NULL);
    jobjectArray bra = (*env)->NewObjectArray(env, length, cls, NULL);
    
    for (i = 0; i < length; i++) {
        Segment sfl = fl[i];
        Segment sfr = fr[i];
        Segment sbl = bl[i];
        Segment sbr = br[i];
        
        jobject jfl = (*env)->NewObject(env, cls, constructor, sfl.dt, sfl.x, sfl.y, sfl.position, sfl.velocity, sfl.acceleration, sfl.jerk, sfl.heading);
        jobject jfr = (*env)->NewObject(env, cls, constructor, sfr.dt, sfr.x, sfr.y, sfr.position, sfr.velocity, sfr.acceleration, sfr.jerk, sfr.heading);
        jobject jbl = (*env)->NewObject(env, cls, constructor, sbl.dt, sbl.x, sbl.y, sbl.position, sbl.velocity, sbl.acceleration, sbl.jerk, sbl.heading);
        jobject jbr = (*env)->NewObject(env, cls, constructor, sbr.dt, sbr.x, sbr.y, sbr.position, sbr.velocity, sbr.acceleration, sbr.jerk, sbr.heading);
        
        (*env)->SetObjectArrayElement(env, fla, i, jfl);
        (*env)->SetObjectArrayElement(env, fra, i, jfr);
        (*env)->SetObjectArrayElement(env, bla, i, jbl);
        (*env)->SetObjectArrayElement(env, bra, i, jbr);
    }
    
    free(segs); free(fl); free(fr); free(bl); free(br);
    
    cls = (*env)->FindClass(env, "[Ljaci/pathfinder/Trajectory$Segment;");
    jobjectArray returnArray = (*env)->NewObjectArray(env, 4, cls, NULL);
    
    (*env)->SetObjectArrayElement(env, returnArray, 0, fla);
    (*env)->SetObjectArrayElement(env, returnArray, 1, fra);
    (*env)->SetObjectArrayElement(env, returnArray, 2, bla);
    (*env)->SetObjectArrayElement(env, returnArray, 3, bra);
    
    return returnArray;
}

/*
 * Class:     jaci_pathfinder_PathfinderJNI
 * Method:    encoderFollow
 * Signature: (I[Ljaci/pathfinder/Trajectory/Segment;Ljaci/pathfinder/followers/EncoderFollower/EncoderConfig;Ljaci/pathfinder/followers/EncoderFollower/FollowerContainer;)V
 */
JNIEXPORT void JNICALL Java_jaci_pathfinder_PathfinderJNI_encoderFollow
  (JNIEnv *env, jclass thiscls, jint encoder_position, jobjectArray trajectory, jobject config, jobject container) {
    
    int length = (*env)->GetArrayLength(env, trajectory);
    int segID = getIntField(env, container, "segment");
    double last_error = getDoubleField(env, container, "last_error");
    
    EncoderFollower follower = { last_error, 0, 0, segID, 0 };

    if (segID >= length) {
        follower.finished = 1;
        follower.output = 0;
        
        jobject sobj = (jobject) (*env)->GetObjectArrayElement(env, trajectory, segID - 1);
        follower.heading = getDoubleField(env, sobj, "heading");        // Don't spazz out the gyro
    } else {
        jobject sobj = (jobject) (*env)->GetObjectArrayElement(env, trajectory, segID);
        Segment s = {
            getDoubleField(env, sobj, "dt"),
            getDoubleField(env, sobj, "x"),
            getDoubleField(env, sobj, "y"),
            getDoubleField(env, sobj, "position"),
            getDoubleField(env, sobj, "velocity"),
            getDoubleField(env, sobj, "acceleration"),
            getDoubleField(env, sobj, "jerk"),
            getDoubleField(env, sobj, "heading")
        };
        
        EncoderConfig c = {
            getIntField(env, config, "initial_position"),
            getIntField(env, config, "ticks_per_revolution"),
            getDoubleField(env, config, "wheel_circumference"),
            getDoubleField(env, config, "kp"),
            getDoubleField(env, config, "ki"),
            getDoubleField(env, config, "kd"),
            getDoubleField(env, config, "kv"),
            getDoubleField(env, config, "ka")
        };
        
        pathfinder_follow_encoder2(c, &follower, s, length, (int)encoder_position);
    }
    
    setDoubleField(env, container, "last_error", follower.last_error);
    setDoubleField(env, container, "heading", follower.heading);
    setDoubleField(env, container, "output", follower.output);
    setIntField(env, container, "segment", follower.segment);
    setIntField(env, container, "finished", follower.finished);
}