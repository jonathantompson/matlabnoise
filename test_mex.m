clearvars; clc; close all;

ydim = 1024;
xdim = 1024;

% 2D Grid
[V, U] = ndgrid(1:ydim, 1:xdim);
X = 2 * (U - 1) ./ (xdim - 1) - 1;  % [-1, 1]
Y = 2 * (V - 1) ./ (ydim - 1) - 1;  % [-1, 1]
Z = zeros(size(X,1), size(X,2));
W = zeros(size(X,1), size(X,2));

figure;
noise = Perlin2D(X, Y);
imshow(noise, [min(noise(:)) max(noise(:))]);

figure;
noise = Perlin3D(X, Y, Z);
imshow(noise, [min(noise(:)) max(noise(:))]);

figure;
noise = Perlin4D(X, Y, Z, W);
imshow(noise, [min(noise(:)) max(noise(:))]);