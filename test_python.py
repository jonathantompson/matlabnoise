import math
import matlabnoise
import matplotlib.pyplot as plt
import numpy as np

# This script will just plot the results from test_mex.m.
# We will not replicate any of the FEM tests here.

ydim = 512
xdim = 511
u, v = np.meshgrid(np.linspace(1, xdim, xdim), np.linspace(1, ydim, ydim))
xstep = 1 / (0.5 * (xdim - 1));
ystep = 1 / (0.5 * (ydim - 1));
X = (u - 1) * xstep - 1;
Y = (v - 1) * ystep - 1;

# TODO(tompson): Call vectorize to fill this.
noise = np.ndarray(shape=(ydim, xdim), dtype='float64')
for y in range(ydim):
  for u in range(xdim):
    noise[y, u] = matlabnoise.Hermite2D(X[y, u], Y[y, u])

plt.figure()
plt.imshow(noise)
plt.title('Hermite2D')
plt.show()

derivx = np.ndarray(shape=(ydim, xdim), dtype='float64')
derivy = np.ndarray(shape=(ydim, xdim), dtype='float64')
# TODO(tompson): Call vectorize to fill this.
for y in range(ydim):
  for u in range(xdim):
    val, dx, dy = matlabnoise.Hermite2DDeriv(X[y, u], Y[y, u])
    noise[y, u] = val
    derivx[y, u] = dx
    derivy[y, u] = dy

def plot2DScalarAndGrad(noise, derivx, derivy):
  xdim = noise.shape[1]
  ydim = noise.shape[0]
  plt.figure()
  cax = plt.imshow(noise)
  plt.colorbar(cax)
  Ap = noise - np.amin(noise)
  Ap = Ap / np.amax(Ap)
  d = int(math.ceil(xdim / 15))  # Decimation ratio.
  u, v = np.meshgrid(np.linspace(1, xdim, xdim), np.linspace(1, ydim, ydim))
  plt.contour(u, v, Ap)
  u = u[0::d,0::d]  # recall start:stop:step
  v = v[0::d,0::d]
  derivx = derivx[0::d,0::d]
  derivy = derivy[0::d,0::d]
  # Matlab plots have the y axis pointing the other way.
  plt.quiver(u, v, derivx, -derivy)

plot2DScalarAndGrad(noise, derivx, derivy)
plt.title('Hermite2DDeriv')
plt.show()

for y in range(ydim):
  for u in range(xdim):
    noise[y, u] = matlabnoise.Perlin2D(X[y, u], Y[y, u])

plt.figure()
plt.imshow(noise)
plt.title('Perlin2D')
plt.show()

# TODO(tompson): Call vectorize to fill this.
for y in range(ydim):
  for u in range(xdim):
    val, dx, dy = matlabnoise.Perlin2DDeriv(X[y, u], Y[y, u])
    noise[y, u] = val
    derivx[y, u] = dx
    derivy[y, u] = dy

plot2DScalarAndGrad(noise, derivx, derivy)
plt.title('Perlin2DDeriv')
plt.show()
