function [ ] = plot_2D_scalar_and_grad( A, gradA )

  xdim = size(A, 2);
  ydim = size(A, 1);

  Ap = A - min(A(:));
  Ap = Ap / max(Ap(:));
  d = ceil(xdim / 15);  % decimation ratio
  imshow(repmat(Ap,1,1,3)); hold on;
  [V, U] = ndgrid(1:ydim, 1:xdim); colormap default;
  contour(U, V, Ap, 'LineWidth', 1.5);
  [V, U] = ndgrid(1:d:ydim, 1:d:xdim);
  quiver(U, V, squeeze(gradA(1,1:d:end,1:d:end)), squeeze(gradA(2,1:d:end,1:d:end)));

end

