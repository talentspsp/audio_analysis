function [accuracy confusion_mat_ratio assign_prob true_label]=sepclassify(Genredir,filename,p,model_opt)
% [accuracy confusion_mat_ratio assign_prob true_label]=sepclassify(Genredir,listfile,[p model_opt])
% model opt is the model to be used, 1 max weight, 2 SVM
% Input:
% DIRpath is the path for the directory containing all the genres
% listfile: the name of the file which contains all the songs to be used
% p is the partition of hold-out for test
% Output:
% accuracy: 1*testtimes vector containinng the accuracy of each test
% confusion_mat_ratio: ngenre*ngenre*testtimes array containing the
% confusion matrix of each test
% assign_prob: 1*testtimes cell, each cell is nsong*ngenre matrix
% containing the probability of the song belonging to that genre
% true_label: 1*testtimes cell, each cell is nsong*1 vector containing the
% true label

%addpath('/home/qkong/musicprj/codes/sep/separate'); %path to components_separation
%addpath('/home/qkong/musicprj/sourceseparation'); % path to sourceseparation
addpath('/Users/Talent/Dropbox/audio_analysis/libsvm-mat-2.91-1');
if nargin<3
    p=0.3; %partition hold-out for test
end

if nargin<4
    model_opt=1;
end

allcomponents=readcomponents_ascii(Genredir,filename);
ngenre=length(allcomponents);

testtimes=ceil(1/p); %number of times for test

accuracy=zeros(1,testtimes);
confusion_mat_ratio=zeros(ngenre,ngenre,testtimes);
assign_prob=cell(1,testtimes);
true_label=cell(1,testtimes);

traindata=cell(1,ngenre);
maxiter=100;

for i=1%:testtimes   DEGUG: save data
    tlabel=[];
    testdata=[];
    train_ind=cell(1,ngenre);
    test_ind=cell(1,ngenre);
    for j=1:ngenre
        nsong=length(allcomponents{j});
        [t_train_ind t_test_ind]=crossvalind('Holdout',nsong,p);
        train_ind{j}=t_train_ind;
        test_ind{j}=t_test_ind;
        traindata{j}=cell2mat(allcomponents{j}(t_train_ind));
        tlabel=[tlabel;ones(sum(t_test_ind),1)*j];
        testdata=[testdata allcomponents{j}(t_test_ind)];
    end
    true_label{i}=tlabel;
    disp('getting genre components...');
    gcomps=extract_genre_components(traindata);
    assign_prob{i}=zeros(length(tlabel),ngenre);
    
    %DEBUG
    %save('testdata','allcomponents','train_ind','test_ind','traindata','testdata','tlabel','gcomps');
    
    if model_opt==1
        for k=1:length(testdata)
            disp('analyzing input data...');
            pts=components_separation(gcomps,testdata{k},maxiter);
            ws=sum(pts,2);
            ws=ws/sum(ws);
            assign_prob{i}(k,:)=ws';
        end
        [mp pred]=max(assign_prob{i},[],2);
        accuracy(i)=sum(pred==tlabel)/length(tlabel);
        confusion_mat_ratio(:,:,i)=create_confusion_mat(tlabel,pred);
    end
    
    if model_opt==2
        svm_train_label=[];
        svm_test_label=[];
        svm_train_data=[];
        svm_test_data=[];
        
        
        %DEBUG:
        alltrainpts=cell(1,ngenre);
        alltrainptzs=cell(1,ngenre);
        alltestpts=cell(1,ngenre);
        alltestptzs=cell(1,ngenre);
        for j=1:ngenre
            svm_train_label=[svm_train_label;ones(sum(train_ind{j}),1)*j];
            svm_test_label=[svm_test_label;ones(sum(test_ind{j}),1)*j];
            alltrainpts{j}=cell(1,sum(train_ind{j}));
            alltrainptzs{j}=cell(1,sum(train_ind{j}));
            alltestpts{j}=cell(1,sum(test_ind{j}));
            alltestptzs{j}=cell(1,sum(test_ind{j}));
            
            t_ind=find(train_ind{j});
            for k=1:length(t_ind)
                t_data=allcomponents{j}{t_ind(k)};
                [pts ptzs]=components_separation(gcomps,t_data,100);
                alltrainpts{j}{k}=pts;
                alltrainptzs{j}{k}=ptzs;
            end
            
            t_ind=find(test_ind{j});
            for k=1:length(t_ind)
                t_data=allcomponents{j}{t_ind(k)};
                [pts ptzs]=components_separation(gcomps,t_data,100);
                alltestpts{j}{k}=pts;
                alltestptzs{j}{k}=ptzs;
            end
        end
       save('testdata_mfcc.mat','allcomponents','train_ind','test_ind','traindata','testdata','tlabel','gcomps','alltrainpts','alltrainptzs','alltestpts','alltestptzs','svm_train_label','svm_test_label');
        %end DEBUG
        
        
        
        
        
%         for j=1:ngenre
%             svm_train_label=[svm_train_label;ones(sum(train_ind{j}),1)*j];
%             svm_test_label=[svm_test_label;ones(sum(test_ind{j}),1)*j];
%             t_ind=find(train_ind{j});
%             for k=1:length(t_ind)
%                 t_data=allcomponents{j}{t_ind(k)};
%                 [pts ptzs]=components_separation(gcomps,t_data,100);
%                 t_ft=[]; % temp feature
%                 for m=1:size(pts,2) % m for time
%                     t_ft_sub=[];
%                     for n=1:size(pts,1) % n for sources
%                         tst=ptzs{n}(:,m)*pts(n,m);
%                         t_ft_sub=[t_ft_sub;tst];
%                     end
%                     t_ft=[t_ft t_ft_sub];
%                 end
%                 t_ft=sum(t_ft,2);
%                 svm_train_data=[svm_train_data;t_ft'];
%             end
%             
%             t_ind=find(test_ind{j});
%             for k=1:length(t_ind)
%                 t_data=allcomponents{j}{t_ind(k)};
%                 [pts ptzs]=components_separation(gcomps,t_data,100);
%                 t_ft=[]; % temp feature
%                 for m=1:size(pts,2) % m for time
%                     t_ft_sub=[];
%                     for n=1:size(pts,1) % n for sources
%                         tst=ptzs{n}(:,m)*pts(n,m);
%                         t_ft_sub=[t_ft_sub;tst];
%                     end
%                     t_ft=[t_ft t_ft_sub];
%                 end
%                 t_ft=sum(t_ft,2);
%                 svm_test_data=[svm_test_data;t_ft'];
%             end
%         end
%         option='-c 0.5 -t 2 -b 1';
%         disp('train svm...');
%         model = svmtrain(svm_train_label, svm_train_data, option);
%         [predict_label, a, dec_values] = svmpredict(svm_test_label, svm_test_data, model,'-b 1');
%         accuracy(i)=a(1);
%         confusion_mat_ratio(:,:,i)=create_confusion_mat(svm_test_label,predict_label);
        
        
    end
end






