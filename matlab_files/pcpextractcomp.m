sched = findResource('scheduler', 'Configuration', 'anthill2010b');

j = createJob(sched);
set(j, 'PathDependencies', {pwd});

directory = pwd;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

datapath=  % path to the data
allwav=dir([datapath filesep '*.wav']);
numdata=50;

for i=1:numdata:length(allwav)
    startind=i;
    endind=min(i+numdata-1,length(allwav));
    createTask(j, @extractcomp, 1, {startind endind});  %parameter can use structure
end
alltasks = get(j, 'Tasks');
submit(j);
waitForState(j);
results = getAllOutputArguments(j);