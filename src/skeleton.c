#include "skeleton.h"

point skeleton_get_foot(skeleton skel) {
    return skel.joints[NUM_JOINTS - 1];
}