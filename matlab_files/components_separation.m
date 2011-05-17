%   [pts ptzs]=components_separation(comps,Vft,maxiter,maxdf)
%   Input:
%   comps: a 1*s cell array. In each cell, it is the components of a source, which is a 2-D matrix, each column is a distribution Ps(f|z),   
%   Vft: a 2D array, each column is a distribution in frequency domain.
%   maxiter: number of max iteration
%   maxdf:threshold to stop iteration
%   Output:
%   pts: a 2D array, echo column is a distritution over different sources
%   at the same time
%   ptzs: a 1*s cell array. In each cell, it is a 2D array, and each column
%   is the distribution over different components at the same time.