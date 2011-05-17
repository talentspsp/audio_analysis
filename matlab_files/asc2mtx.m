function mtx=asc2mtx(filename)
% read the matrix in filename to mtx
% the format of the file:
%    row
%    col
%    (empty line)
%    data in 2D


fin=fopen(filename,'r');
line=fgetl(fin);
temp=textscan(line,'%u');
row=temp{1};
line=fgetl(fin);
temp=textscan(line,'%u');
col=temp{1};

mtx=zeros(row, col);
line=fgetl(fin); %empty line
for i=1:row
    line=fgetl(fin);
    temp=textscan(line,'%f');
    mtx(i,:)=temp{1}';
end
fclose(fin)