#include <stdlib.h>

#include "fkin.h"
#include "path.h"
#include "geometry.h"

skeleton fkin(linkage link, decimal theta) {
    // Get the link lengths and give them each a name
    decimal a = link.lengths[0];
    decimal b = link.lengths[1];
    decimal c = link.lengths[2];
    decimal d = link.lengths[3];
    decimal e = link.lengths[4];
    decimal f = link.lengths[5];
    decimal g = link.lengths[6];
    decimal h = link.lengths[7];
    decimal i = link.lengths[8];
    decimal j = link.lengths[9];
    decimal k = link.lengths[10];
    decimal l = link.lengths[11];
    decimal m = link.lengths[12];

    decimal Ax = m * cos(theta);
    decimal Ay = m * sin(theta);

    decimal Bx = -a;
    decimal By = -l;

    decimal dABx = Ax - Bx;
    decimal dABy = Ay - By;

    decimal AB2 = dABx * dABx + dABy * dABy;
    decimal AB = sqrt(AB2);

    decimal alpha = atan2(dABy, dABx);

    decimal cosBeta = (AB2 + b * b - j * j) / (2 * AB * b);

    if (abs(cosBeta) > 1) {
        return BROKEN_SKELETON;
    }

    decimal beta = acos(cosBeta);

    decimal Cx = Bx + b * cos(alpha + beta);
    decimal Cy = By + b * sin(alpha + beta);

    decimal cosGamma = (b * b + d * d - e * e) / (2 * b * d);

    if (abs(cosGamma) > 1) {
        return BROKEN_SKELETON;
    }

    decimal gamma = acos(cosGamma);

    decimal cosDelta = (AB2 + c * c - k * k) / (2 * AB * c);

    if (abs(cosDelta) > 1) {
        return BROKEN_SKELETON;
    }

    decimal delta = acos(cosDelta);

    decimal Dx = Bx + d * cos(alpha + beta + gamma);
    decimal Dy = By + d * sin(alpha + beta + gamma);

    decimal Ex = Bx + c * cos(alpha - delta);
    decimal Ey = By + c * sin(alpha - delta);

    decimal dDEx = Dx - Ex;
    decimal dDEy = Dy - Ey;

    decimal DE2 = dDEx * dDEx + dDEy * dDEy;
    decimal DE = sqrt(DE2);

    decimal epsilon = atan2(dDEy, dDEx);

    decimal cosZeta = (DE2 + g * g - f * f) / (2 * DE * g);

    if (abs(cosZeta) > 1) {
        return BROKEN_SKELETON;
    }

    decimal zeta = acos(cosZeta);

    decimal cosEta = (g * g + i * i - h * h) / (2 * g * i);

    if (abs(cosEta) > 1) {
        return BROKEN_SKELETON;
    }

    decimal eta = acos(cosEta);

    decimal Fx = Ex + g * cos(epsilon + zeta);
    decimal Fy = Ey + g * sin(epsilon + zeta);

    decimal Gx = Ex + i * cos(epsilon + zeta + eta);
    decimal Gy = Ey + i * sin(epsilon + zeta + eta);

    // Check if any joints are below the foot point
    if (Ay < Gy || By < Gy || Cy < Gy || Dy < Gy || Ey < Gy || Fy < Gy) {
        return BROKEN_SKELETON;
    }

    point A = (point){.x = Ax, .y = Ay};
    point B = (point){.x = Bx, .y = By};
    point C = (point){.x = Cx, .y = Cy};
    point D = (point){.x = Dx, .y = Dy};
    point E = (point){.x = Ex, .y = Ey};
    point F = (point){.x = Fx, .y = Fy};
    point G = (point){.x = Gx, .y = Gy};

    segment AC = (segment){.start = A, .end = C};
    segment AE = (segment){.start = A, .end = E};
    segment BC = (segment){.start = B, .end = C};
    segment BD = (segment){.start = B, .end = D};
    segment BE = (segment){.start = B, .end = E};
    segment CD = (segment){.start = C, .end = D};
    segment DF = (segment){.start = D, .end = F};
    segment EF = (segment){.start = E, .end = F};
    segment EG = (segment){.start = E, .end = G};
    segment FG = (segment){.start = F, .end = G};

    segment segments[] = {AC, AE, BC, BD, BE, CD, DF, EF, EG, FG};

    for (size_t i = 0; i < 9; i++) {
        for (size_t j = i + 1; j < 10; j++) {
            point p1 = segments[i].start;
            point p2 = segments[i].end;
            point p3 = segments[j].start;
            point p4 = segments[j].end;

            if (point_equals(p1, p3)) { continue; }
            if (point_equals(p1, p4)) { continue; }
            if (point_equals(p2, p3)) { continue; }
            if (point_equals(p2, p4)) { continue; }

            if (segments_intersect(segments[i], segments[j])) {
                return BROKEN_SKELETON;
            }
        }
    }

    skeleton skel = (skeleton){.broken = false};

    skel.joints[0] = (point){Ax, Ay};
    skel.joints[1] = (point){Bx, By};
    skel.joints[2] = (point){Cx, Cy};
    skel.joints[3] = (point){Dx, Dy};
    skel.joints[4] = (point){Ex, Ey};
    skel.joints[5] = (point){Fx, Fy};
    skel.joints[6] = (point){Gx, Gy};

    return skel;
}

path *compute_stride(linkage link, size_t resolution) {
    path *p = path_init(resolution);

    for (size_t step = 0; step < resolution; step++) {
        decimal crank_angle = 2 * M_PI * step / resolution;
        skeleton skel = fkin(link, crank_angle);

        if (skel.broken) {
            free(p);
            return NULL;
        }

        p->points[step] = skeleton_get_foot(skel);
    }

    return p;
}