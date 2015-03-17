function [ X, Y, xstep, ystep ] = ndgrid_normalized( xdim, ydim )
  [v, u] = ndgrid(1:ydim, 1:xdim);
  xstep = 1 / (0.5 * (xdim - 1));
  ystep = 1 / (0.5 * (ydim - 1));
  X = (u - 1) .* xstep - 1;  % [-1, 1]
  Y = (v - 1) .* ystep - 1;  % [-1, 1]
end

