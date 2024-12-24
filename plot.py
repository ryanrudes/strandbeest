from typing import Optional
from numpy import ndarray, sin, cos, atan2, sqrt, acos, inf

import matplotlib.animation as animation
import matplotlib.pyplot as plt
import matplotlib
import numpy as np

PATH_RESOLUTION = 100
FRAMERATE = 200

cmap = matplotlib.colormaps.get_cmap("viridis") 

def fkin(links: ndarray, theta: float, return_all=False) -> Optional[ndarray]:
    """Compute the forward kinematics
    
    Args:
        links (np.ndarray): The lengths of the links
        theta (float): The crank angle

    Returns:
        np.ndarray: The position of the foot (or None if the mechanism breaks)
    """
    a, b, c, d, e, f, g, h, i, j, k, l, m = links

    Ax = m * cos(theta)
    Ay = m * sin(theta)

    Bx = -a
    By = -l

    dABx = Ax - Bx
    dABy = Ay - By

    AB2 = dABx * dABx + dABy * dABy
    AB = sqrt(AB2)
    
    alpha = atan2(dABy, dABx)

    cos_beta = (AB2 + b * b - j * j) / (2 * AB * b)

    if abs(cos_beta) > 1:
        return

    beta = acos(cos_beta)

    Cx = Bx + b * cos(alpha + beta)
    Cy = By + b * sin(alpha + beta)

    cos_gamma = (b * b + d * d - e * e) / (2 * b * d)

    if abs(cos_gamma) > 1:
        return

    gamma = acos(cos_gamma)

    cos_delta = (AB2 + c * c - k * k) / (2 * AB * c)

    if abs(cos_delta) > 1:
        return

    delta = acos(cos_delta)

    Dx = Bx + d * cos(alpha + beta + gamma)
    Dy = By + d * sin(alpha + beta + gamma)

    Ex = Bx + c * cos(alpha - delta)
    Ey = By + c * sin(alpha - delta)

    dDEx = Dx - Ex
    dDEy = Dy - Ey

    DE2 = dDEx * dDEx + dDEy * dDEy
    DE = sqrt(DE2)

    epsilon = atan2(dDEy, dDEx)

    cos_zeta = (DE2 + g * g - f * f) / (2 * DE * g)

    if abs(cos_zeta) > 1:
        return

    zeta = acos(cos_zeta)

    cos_eta = (g * g + i * i - h * h) / (2 * g * i)

    if abs(cos_eta) > 1:
        return

    eta = acos(cos_eta)

    Fx = Ex + g * cos(epsilon + zeta)
    Fy = Ey + g * sin(epsilon + zeta)

    Gx = Ex + i * cos(epsilon + zeta + eta)
    Gy = Ey + i * sin(epsilon + zeta + eta)

    if Ay < Gy or By < Gy or Cy < Gy or Dy < Gy or Ey < Gy or Fy < Gy:
        raise Exception("Linkage broke")

    if return_all:
        A = np.array([Ax, Ay])
        B = np.array([Bx, By])
        C = np.array([Cx, Cy])
        D = np.array([Dx, Dy])
        E = np.array([Ex, Ey])
        F = np.array([Fx, Fy])
        G = np.array([Gx, Gy])

        return A, B, C, D, E, F, G
    else:
        return np.array([Gx, Gy])
        
def plot(crank_angle, linkage, color, ground, path):
    A, B, C, D, E, F, G = fkin(linkage, crank_angle, return_all = True)
    
    A[1] -= ground
    B[1] -= ground
    C[1] -= ground
    D[1] -= ground
    E[1] -= ground
    F[1] -= ground
    G[1] -= ground

    O = np.array([0, -ground])

    OA = np.array([O, A])
    AC = np.array([A, C])
    AE = np.array([A, E])
    BC = np.array([B, C])
    BD = np.array([B, D])
    BE = np.array([B, E])
    CD = np.array([C, D])
    DF = np.array([D, F])
    EF = np.array([E, F])
    EG = np.array([E, G])
    FG = np.array([F, G])

    segments = [OA, AC, AE, BC, BD, BE, CD, DF, EF, EG, FG]

    for segment in segments:
        plt.plot(segment[:, 0], segment[:, 1], '-', c = color)

    plt.plot(path[:, -1, 0], path[:, -1, 1], c = color, linestyle = "--")

def get_path(links: ndarray, resolution: int = PATH_RESOLUTION, return_all = False) -> Optional[ndarray]:
    """
    Get the path traced by the foot of the mechanism.

    If the mechanism breaks, return None.
    """
    path = []

    for crank_angle in np.linspace(0, 2 * np.pi, resolution):
        foot = fkin(links, crank_angle, return_all = return_all)

        if foot is None:
            return

        path.append(foot)
    
    return np.array(path)

def animate(frame):
    plt.cla()

    crank_angle = frame * 2 * np.pi / 200

    xmin = inf
    xmax = -inf
    ymin = inf
    ymax = -inf

    for i, linkage in enumerate(linkages):
        path = get_path(linkage, return_all = True)
        ground = path[..., 1].min()
        path[..., 1] -= ground

        color = cmap(i / len(linkages))
        plot(crank_angle, linkage, color, ground, path)

        xmin = min(xmin, path[..., 0].min())
        xmax = max(xmax, path[..., 0].max())
        ymin = min(ymin, path[..., 1].min())
        ymax = max(ymax, path[..., 1].max())

    waypoints_x = np.array([-0.43160111, -0.07689066, -0.3372973, -0.70670563])
    waypoints_y = np.array([0.00076335, 0.01443916, 0.1831617, 0.02190431])

    plt.scatter(waypoints_x, waypoints_y, c = "red")

    xrange = xmax - xmin
    yrange = ymax - ymin

    axrange = max(xrange, yrange)

    xmid = (xmin + xmax) / 2
    ymid = (ymin + ymax) / 2

    xmin = xmid - axrange / 2 - axrange / 10
    xmax = xmid + axrange / 2 + axrange / 10
    ymin = ymid - axrange / 2 - axrange / 10
    ymax = ymid + axrange / 2 + axrange / 10

    plt.xlim(xmin, xmax)
    plt.ylim(ymin, ymax)

    plt.axis("off")

# Edit me!
linkages_data = [
    "0.380 0.415 0.393 0.401 0.558 0.394 0.367 0.657 0.490 0.500 0.619 0.078 0.150", # Jansen's linkage
    # You can add more linkages here
    #"0.391070 0.473696 0.664591 0.591427 1.000000 0.532940 0.742106 0.601984 0.706593 0.232262 0.486343 0.058045 0.148783",
]

linkages = [np.array(list(map(float, linkage_data.split(" ")))) for linkage_data in linkages_data]

fig, ax = plt.subplots()
ani = animation.FuncAnimation(fig, animate, frames=200, interval = 1000 / FRAMERATE)
plt.show()