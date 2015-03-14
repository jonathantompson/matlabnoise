clearvars; clc; close all;

ydim = 512;
xdim = 512;
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

% 3D Grid
ydim = 64;
xdim = 64;
zdim = 64;
[V, U, D] = ndgrid(1:ydim, 1:xdim, 1:zdim);
X = 2 * (U - 1) ./ (xdim - 1) - 1;  % [-1, 1]
Y = 2 * (V - 1) ./ (ydim - 1) - 1;  % [-1, 1]
Z = 2 * (D - 1) ./ (zdim - 1) - 1;  % [-1, 1]
X = X * frequency;
Y = Y * frequency;
Z = Z * frequency;
figure;
noise = Perlin3D(X, Y, Z);
noise = noise - min(noise(:));
noise = noise / max(noise(:));
contourslice(noise, [], [], [1,10,20,30], 16);
view(3);
axis tight
title('Perlin3D');
grid on;

% Now just do a time series of a 3D grid (because I can't visualize 4D
% noise :-) )
figure;
for i = 1:4
  subplot(2,2,i);
  W = ones(size(X,1), size(X,2), size(X,3)) * i;
  noise = Perlin4D(X, Y, Z, W);
  noise = noise - min(noise(:));
  noise = noise / max(noise(:));
  contourslice(noise, [], [], [1,10,20,30], 16);
  view(3);
  axis tight
  title(['Perlin4D w=', num2str(i)]);
end
grid on;