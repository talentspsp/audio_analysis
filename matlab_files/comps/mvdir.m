d=dir('*');
for i=3:length(d)
    if(d(i).isdir)
         unix(['mkdir -p ' d(i).name(6)]);
         unix(['mv ' d(i).name ' ' d(i).name(6) filesep]);
%          d(i).name(6) 
    end
end
