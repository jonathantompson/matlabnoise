clearvars; clc; close all;
% This script will just plot the scalar fields and check the derivatives
% using FEM (central differences).

ydim = 512;
xdim = 511;
frequency = 1;

% 2D Grid
[V, U] = ndgrid(1:ydim, 1:xdim);
X = 2 * (U - 1) ./ (xdim - 1) - 1;  % [-1, 1]
Y = 2 * (V - 1) ./ (ydim - 1) - 1;  % [-1, 1]
X = X * frequency;
Y = Y * frequency;

figure;
noise = Perlin2D(X, Y);
imshow(noise, [min(noise(:)) max(noise(:))]);
title('Perlin2D');

% 2D Gradient
[noise_deriv, gradNoise] = Perlin2DDeriv(X, Y);
figure;
plot_2D_scalar_and_grad(noise_deriv, gradNoise);
title('Perlin2DDeriv');

err = abs(noise_deriv - noise);
assert(max(err(:)) < 1e-6, ...
  'scalar field from Perlin2DDeriv does not match Perlin2D!');

% addpath('C:\Users\IggyMenou\Documents\NYU\detection_nets\matlab\export_fig');
% export_fig('grad_field.jpg', gcf, '-jpg', '-a4');

% Check the gradient using finite differences (central)
epsilon = 1e-6;
[pos, posGrad] = Perlin2DDeriv(X + epsilon, Y);
[neg, negGrad] = Perlin2DDeriv(X - epsilon, Y);
dx = (pos - neg) / (2 * epsilon);
[pos, posGrad] = Perlin2DDeriv(X, Y + epsilon);
[neg, negGrad] = Perlin2DDeriv(X, Y - epsilon);
dy = (pos - neg) / (2 * epsilon);
gradNoiseFEM = permute(cat(3, dx, dy), [3 1 2]);
err = abs(gradNoiseFEM - gradNoise);
assert(max(err(:)) < 1e-8, 'FEM derivative does not match!');

% 3D Grid
xdim = 63;
ydim = 64;
zdim = 65;
[D, V, U] = ndgrid(1:zdim, 1:ydim, 1:xdim);
X = 2 * (U - 1) ./ (xdim - 1) - 1;  % [-1, 1]
Y = 2 * (V - 1) ./ (ydim - 1) - 1;  % [-1, 1]
Z = 2 * (D - 1) ./ (zdim - 1) - 1;  % [-1, 1]
X = X * frequency;
Y = Y * frequency;
Z = Z * frequency;
figure;
noise = Perlin3D(X, Y, Z);
plot_3D_scalar(noise);
title('Perlin3D');

% 3D Gradient
[noise_deriv, gradNoise] = Perlin3DDeriv(X, Y, Z);
err = abs(noise_deriv - noise);
assert(max(err(:)) < 1e-6, ...
  'scalar field from Perlin3DDeriv does not match Perlin3D!');

figure;
plot_3D_scalar_and_grad(noise_deriv, gradNoise);
title('Perlin3DDeriv');
set(gcf,'Renderer','OpenGL')
title('Perlin3DDeriv');

% Check the gradient using finite differences (central)
epsilon = 1e-6;
[pos, posGrad] = Perlin3DDeriv(X + epsilon, Y, Z);
[neg, negGrad] = Perlin3DDeriv(X - epsilon, Y, Z);
dx = (pos - neg) / (2 * epsilon);
[pos, posGrad] = Perlin3DDeriv(X, Y + epsilon, Z);
[neg, negGrad] = Perlin3DDeriv(X, Y - epsilon, Z);
dy = (pos - neg) / (2 * epsilon);
[pos, posGrad] = Perlin3DDeriv(X, Y, Z + epsilon);
[neg, negGrad] = Perlin3DDeriv(X, Y, Z - epsilon);
dz = (pos - neg) / (2 * epsilon);
gradNoiseFEM = permute(cat(4, dx, dy, dz), [4 1 2 3]);
err = abs(gradNoiseFEM - gradNoise);
assert(max(err(:)) < 1e-8, 'FEM derivative does not match!');

% Now just do a time series of a 3D grid (because I can't visualize 4D
% noise :-) )
figure;
for i = 1:4
  subplot(2,2,i);
  W = ones(size(X,1), size(X,2), size(X,3)) * i;
  noise = Perlin4D(X, Y, Z, W);
  plot_3D_scalar(noise);
  title(['Perlin4D w=', num2str(i)]);
end
