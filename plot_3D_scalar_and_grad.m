function [ ] = plot_3D_scalar_and_grad( noise, gradNoise )

  xdim = size(noise, 3);
  ydim = size(noise, 2);
  zdim = size(noise, 1);

  nslices = 4;
  d = ceil(zdim / nslices);
  slices = 1:d:zdim;
  
  noise = noise - min(noise(:));
  noise = noise / max(noise(:));
  h = contourslice(noise, slices, [], [], 16);
  set(h, 'LineWidth', 1.5);
  view(3);
  axis tight
  grid on;
  hold on;
  
  d = ceil(xdim / 5);  % decimation ratio
  gradNoise = gradNoise(:, slices, 1:d:end, 1:d:end);
  dX = squeeze(gradNoise(1,:,:,:));
  dY = squeeze(gradNoise(2,:,:,:));
  dZ = squeeze(gradNoise(3,:,:,:));
  [D, V, U] = ndgrid(slices, 1:d:ydim, 1:d:xdim);
  quiver3(D, V, U, dX, dY, dZ);

end

