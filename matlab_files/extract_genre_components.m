function gcomps=extract_genre_components(data,initial_nump,sz)
% gcomps=extract_genre_components(data,[initial_nump,sz])
% This function is to extract components for each genre
% Input:
% data: 1*ngenre cell, each cell is a matrix, each columm is a component
% from songs in that genre
% initial_nump: initial number of components for plca [100] (try 100 first, maybe need to be changed)
% sz: sparseness on z. if sz=0, then we keep all the components, otherwise
% ,we will drop components with lower z. [0.2] (try 0.2 first)
% Output:
% gcomps: genre components. a 1*ngenre cell, each genre contains the
% components for that genre

if(nargin<2)
    initial_nump=100;
end

if(nargin<3)
    sz=0.2;
end

ngenre=length(data);
gcomps=cell(1,ngenre);
thd=2/initial_nump; %threshold to accept the w
for i=1:ngenre
    [w,h,z]=plca2d( data{i}, initial_nump, 100, sz, 0, 0, [], [], [], 0);
    if(sz==0)
        gcomps{i}=w;
    else
        gcomps{i}=w(:,z>thd);
    end
end
        
        
