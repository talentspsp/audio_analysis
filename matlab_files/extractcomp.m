function fst=extractcomp(filename)
%toolpath='/net/grad/kong/machine_learning/sourceseparation';  % path to plca
%addpath(toolpath);
datapath='/Users/Talent/Dropbox/cs134/project/dataset/wav';  % path to the data
allwav=dir([datapath filesep '*.wav']);
savepath='/Users/Talent/Dropbox/audio_analysis/matlab_files/comps';  % path to save the result data

% parameters
fftN=1024;
fftW=1024;
fftH=512;
ncomp=10;
sz=0.2;

fid=fopen(filename,'r');
allsong=textscan(fid,'%s');

for i=1:length(allsong{1})
    wavfile=allsong{1}{i};
    [s f]= wavread([datapath filesep wavfile]);
    if(size(s,2)==2)
        s=s(:,1)+s(:,2);
    end
    st=stft1(s',fftN,fftW,fftH);
    st=abs(st);
    st(st==0)=0.000001;
    [w h z]=plca2d( st, ncomp, 100, sz, 0, 0, [], [], [], 0);
    
    % save the result
    rt=unix(['mkdir -p ' savepath filesep wavfile(1:end-4)]);
    if (rt)
        warning('cannot create directory');
        continue;
    end
    save( [savepath filesep wavfile(1:end-4) filesep wavfile(1:end-4) '_W.ascii'], 'w', '-ascii');
    save( [savepath filesep wavfile(1:end-4) filesep wavfile(1:end-4) '_Z.ascii'], 'z', '-ascii');
end
fst=1;
