clear all;
%read input data
datain=fopen('../plcadata.txt','r');
line=fgetl(datain);
lent=0;
frame=[];
while(ischar(line))
    lent=lent+1;
    temp=textscan(line,'%f');
    frame=[frame temp{1}];
    line=fgetl(datain);
end
fclose(datain);
lenf=size(frame,1);

ptzin=fopen('../plcaptz.txt','r');
line=fgetl(ptzin);
c_ptz=[];
while(ischar(line))
     temp=textscan(line,'%f');
     c_ptz=[c_ptz temp{1}];
     line=fgetl(ptzin);
end
fclose(ptzin);

pfzin=fopen('../plcapfz.txt','r');
line=fgetl(pfzin);
c_pfz=[];
while(ischar(line))
     temp=textscan(line,'%f');
     c_pfz=[c_pfz temp{1}];
     line=fgetl(pfzin);
end
fclose(pfzin);

pzin=fopen('../plcapz.txt');
line=fgetl(pzin);
temp=textscan(line,'%f');
c_pz_read=temp{1};
fclose(pzin);


K=5;
tic
[w,h,z] = plca2D( frame, K, 100, 0,0,0,[],[],[],0);
toc
mat_rct=w*diag(z)*h;

figure;
imagesc(frame);
colorbar;
title('original');
figure;
imagesc(mat_rct);
colorbar;
title('reconstruction from matlab');
figure;
pf_t=c_pfz*c_ptz;
pt=sum(frame);
pt_mat1=ones(lenf,1)*pt;
c_rct=pf_t.*pt_mat1;
imagesc(c_rct);
colorbar;
title('reconstrunction from C');

pt_mat2=ones(K,1)*pt;
pz=c_ptz.*pt_mat2;
pz=sum(pz,2);
figure;
stem(pz);
title('c_pz');

figure;
stem(c_pz_read);
title('c pz read')

err_mat=sum(sum(abs(mat_rct-frame)))
err_c=sum(sum(abs(c_rct-frame)))
