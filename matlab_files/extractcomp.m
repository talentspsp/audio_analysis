function fst=extractcomp(startind,endind)
toolpath='/Users/Talent/Documents/courses/sound/sourceseparation';  % path to plca
addpath(toolpath);
datapath='/Users/Talent/Dropbox/cs134/project/dataset/wav';  % path to the data
allwav=dir([datapath filesep '*.wav']);
savepath='/Users/Talent/Dropbox/audio_analysis/matlab_files/testdir';  % path to save the result data

% parameters
fftN=1024;
fftW=1024;
fftH=512;
ncomp=10;
sz=0.2;

% TODO: separate into different tasks


for i=startind:endind
    wavfile=allwav(i).name;
    [s f]= wavread([datapath filesep wavfile]);
    if(size(s,2)==2)
        s=s(:,1)+s(:,2);
    end
    st=stft1(s',fftN,fftW,fftH);
    st=abs(st);
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