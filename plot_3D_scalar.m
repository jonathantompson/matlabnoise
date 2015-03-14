function [ ] = plot_3D_scalar( noise )

  noise = noise - min(noise(:));
  noise = noise / max(noise(:));
  h = contourslice(noise, [], [], [1,10,20,30], 16);
  set(h, 'LineWidth', 1.5);
  view(3);
  axis tight
  
  grid on;

end

