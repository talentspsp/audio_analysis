clear all;

maxiter=100;

filevft='../Vft.txt';
Vft=asc2mtx(filevft);
[t f]=size(Vft);
Vft=Vft';

pt=sum(Vft);
pt_mat=ones(f,1)*pt;
filepsfz=dir('../psfz*.txt');
s=length(filepsfz);
comps=cell(1,s);
for i=1:s
    psfz=asc2mtx(['..' filesep filepsfz(i).name]);
    comps{i}=psfz';
end

filepts='../pts.txt';
c_pts=asc2mtx(filepts);
c_pts=c_pts';

fileptzs=dir('../ptzs*.txt');
c_ptzs=cell(1,s);
for i=1:s
    c_ptzs{i}=asc2mtx(['..' filesep fileptzs(i).name]);
    c_ptzs{i}=c_ptzs{i}';
end

[pts ptzs]=components_separation(comps,Vft,maxiter);


%reconstruction
rec_sr_mat=cell(1,s); %reconstruction for each source
for i=1:s
    rec_sr_mat{i}=comps{i}*ptzs{i};
end

rec_mat=zeros(size(Vft));
for i=1:s
    rec_mat=rec_mat+ones(f,1)*pts(i,:).*rec_sr_mat{i};
end
rec_mat=rec_mat.*pt_mat;

%reconstruction
rec_sr_c=cell(1,s); %reconstruction for each source
for i=1:s
    rec_sr_c{i}=comps{i}*c_ptzs{i};
end

rec_c=zeros(size(Vft));
for i=1:s
    rec_c=rec_c+ones(f,1)*c_pts(i,:).*rec_sr_c{i};
end
rec_c=rec_c.*pt_mat;
%show result
imagesc(Vft);
colorbar;
title('original')
figure;
imagesc(rec_mat);
colorbar;
title('reconstruction matlab');

figure;
imagesc(rec_mat);
colorbar;
title('reconstruction C');