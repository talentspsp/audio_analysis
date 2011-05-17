function allcomponents=readcomponents_ascii(Genredir,filename)
% input:
% Genredir: the path that contain the data for all the genres
% filename: the file contain the relative path for songs. each line of the
% file is supposed to be in the format that genre/song
% output:
% allcomponents: 1*ngenre cell, in each cell, it is a 1*nsong cell, in each
% cell, it is the matrix W. (in future maybe change to genre album song)



fid=fopen(filename,'r');
allsong=textscan(fid,'%s'); % each line of the file is supposed to be in the format that genre/album/song
pos=strfind(allsong{1}{1},'/');
head=allsong{1}{1}(1:pos(1)-1);% genrename
tail=[];
for i=2:length(allsong{1})
    pos=strfind(allsong{1}{i},'/');
    if (strcmp(head,allsong{1}{i}(1:pos(1)-1))==0)
        head=allsong{1}{i}(1:pos(1)-1);
        tail(end+1)=i-1;
    end
end
tail(end+1)=length(allsong{1});
ngenre=length(tail);

allcomponents=cell(1,ngenre);
head=1;
for i=1:ngenre
    sc=1; %song count
    songcell=[];
    for j=head:tail(i)
        song=[Genredir filesep allsong{1}{j}]; % the full path for the song
%         cmd=['unzip ' song filesep 'WHZ.zip -d ' song];
%         st=unix(cmd);
%         tc=0;
%         while(st~=0 && tc<3)
%             st=unix(cmd);
%             tc=tc+1;
%         end
%         if(st ~=0)
%             continue;
%         end
        W=[];
        d=dir([song filesep '*_W.ascii']);
        dz=dir([song filesep '*_Z.ascii']);
        for k=1:length(d)
            wt=load([song filesep d(k).name]);
            zt=load([song filesep dz(k).name]);
            %get rid of columns with all zeros
            ncomp=length(zt);
            wt=wt(:,zt>0.5/ncomp);   %only keep the components with higher probability
            wt=wt(:,wt(1,:)>0);
            W=[W wt];
        end
        if(isempty(W))
            continue;
        end
        W(W==0)=1e-10;
        songcell{sc}=W;
        sc=sc+1;
%         cmd=['rm ' song filesep '*.ascii'];
%         unix(cmd);
    end
    head=tail(i)+1;
    allcomponents{i}=songcell;
end
