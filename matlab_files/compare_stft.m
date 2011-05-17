clear all;
%read input data
fin=fopen('../build/release/data','r');
%supose each line of the data is a complex number,a+ib
indata=textscan(fin,'%f');
fclose(fin);

frame=[];
lent=0;
fout=fopen('../build/release/output','r');
line=fgetl(fout);
while(ischar(line))
    lent=lent+1;
    temp=textscan(line,'%f');
    frame=[frame temp];
    line=fgetl(fout);
end
fclose(fout);
lenf=length(frame{1});

stft_matlab=stft1(indata{1}',16,16,5);