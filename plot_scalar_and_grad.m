function [ ] = plot_scalar_and_grad( A, gradA )

  figure;
  xdim = size(A, 2);
  ydim = size(A, 1);

  Ap = A - min(A(:));
  Ap = Ap / max(Ap(:));
  d = ceil(xdim / 15);  % decimation ratio
  imshow(repmat(Ap,1,1,3)); hold on;
  [U, V] = ndgrid(1:xdim, 1:ydim); colormap default;
  contour(V, U, Ap, 'LineWidth', 1.5);
  [U, V] = ndgrid(1:d:xdim, 1:d:ydim);
  quiver(V, U, squeeze(gradA(1,1:d:end,1:d:end)), squeeze(gradA(2,1:d:end,1:d:end)));

end

