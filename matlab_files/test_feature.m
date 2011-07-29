function [accuracy confusion_mat_ratio]=test_feature(feat_opt,cov_opt,LDA_opt,svmscale,kernel_type,PCAthre)
% Input:
% feat_opt:
% 1: mean pts
% 2: mean pts*ptzs
% 3: mean ptzs
% cov_opt:
% 0: no cov
% 1: cov
% 2: diag cov
% LDA_opt:
% 0: no LDA
% 1: PCA+LDA
% svmscale: data*svmscale [10]
% kernel_type: 0 linear, 2 gaussian [0]
% 
% PCAthre: threshold for PCA and LDA [0.01]

% TODO: observe the data and choose a good PCA threshold


% load the data
load testdata_2high_thd_gcomp.mat

if(nargin<4)
    svmscale=10;
end

if(nargin<5)
    kernel_type=0;
end

if(nargin<6)
    PCAthre=0.01;
end


ngenre=length(allcomponents);

svm_train_data=[];
svm_test_data=[];
for j=1:ngenre
    for k=1:length(alltrainpts{j})
        switch feat_opt
            case 1
                pts=alltrainpts{j}{k};
                featset=pts';
                mean_feat=mean(featset);
                
            case 2
                pts=alltrainpts{j}{k};
                ptzs=alltrainptzs{j}{k};
                featset=[];
                for m=1:size(pts,2) % m for time
                    t_ft_sub=[];
                    for n=1:size(pts,1) % n for sources
                        tst=ptzs{n}(:,m)*pts(n,m);
                        t_ft_sub=[t_ft_sub;tst];
                    end
                    featset=[featset t_ft_sub];
                end
                featset=featset';
                mean_feat=mean(featset);
            case 3
                pts=alltrainpts{j}{k};
                ptzs=alltrainptzs{j}{k};
                featset=[];
                for m=1:size(pts,2) % m for time
                    t_ft_sub=[];
                    for n=1:size(pts,1) % n for sources
                        tst=ptzs{n}(:,m);
                        t_ft_sub=[t_ft_sub;tst];
                    end
                    featset=[featset t_ft_sub];
                end
                featset=featset';
                mean_feat=mean(featset);
        end
        
        switch cov_opt
            case 0
                svm_train_data=[svm_train_data;mean_feat];
            case 1
                cc=cov(featset);
                cc=reshape(cc,1,[]);
                temp_feat=[mean_feat cc];
                svm_train_data=[svm_train_data;temp_feat];
            case 2
                cc=cov(featset);
                cc=reshape(diag(cc),1,[]);
                temp_feat=[mean_feat cc];
                svm_train_data=[svm_train_data;temp_feat];
        end
    end
    %process testdata
    
    for k=1:length(alltestpts{j})
        switch feat_opt
            case 1
                pts=alltestpts{j}{k};
                featset=pts';
                mean_feat=mean(featset);
                
            case 2
                pts=alltestpts{j}{k};
                ptzs=alltestptzs{j}{k};
                featset=[];
                for m=1:size(pts,2) % m for time
                    t_ft_sub=[];
                    for n=1:size(pts,1) % n for sources
                        tst=ptzs{n}(:,m)*pts(n,m);
                        t_ft_sub=[t_ft_sub;tst];
                    end
                    featset=[featset t_ft_sub];
                end
                featset=featset';
                mean_feat=mean(featset);
            case 3
                pts=alltestpts{j}{k};
                ptzs=alltestptzs{j}{k};
                featset=[];
                for m=1:size(pts,2) % m for time
                    t_ft_sub=[];
                    for n=1:size(pts,1) % n for sources
                        tst=ptzs{n}(:,m);
                        t_ft_sub=[t_ft_sub;tst];
                    end
                    featset=[featset t_ft_sub];
                end
                featset=featset';
                mean_feat=mean(featset);
        end
        
        switch cov_opt
            case 0
                svm_test_data=[svm_test_data;mean_feat];
            case 1
                cc=cov(featset);
                cc=reshape(cc,1,[]);
                temp_feat=[mean_feat cc];
                svm_test_data=[svm_test_data;temp_feat];
            case 2
                cc=cov(featset);
                cc=reshape(diag(cc),1,[]);
                temp_feat=[mean_feat cc];
                svm_test_data=[svm_test_data;temp_feat];
        end
    end
end


% pca+lda
if LDA_opt==1
    %PCA
    me=mean(svm_train_data);
    ntrain=size(svm_train_data,1);
    svm_train_data=svm_train_data-repmat(me,ntrain,1);
    [U,S,V]=svd(svm_train_data'./sqrt(ntrain));
    ndim=sum((diag(S)>PCAthre));
    svm_train_data=svm_train_data*U(:,1:ndim);
    
    ntest=size(svm_test_data,1);
    svm_test_data=svm_test_data-repmat(me,ntest,1);
    svm_test_data=svm_test_data*U(:,1:ndim);
    
    %LDA
    dim=size(svm_train_data,2);
    allmean=zeros(ngenre,dim);
    allcov=zeros(dim,dim,ngenre);
    for k=1:ngenre
        cind=(svm_train_label==k);
        allmean(k,:)=mean(svm_train_data(cind,:),1);
        allcov(:,:,k)=cov(svm_train_data(cind,:));
    end
    Sw=sum(allcov,3);
    Sb=cov(allmean);
    S=Sw\Sb;
    [V D]=eig(S);
    sidx=(diag(D)>PCAthre);
    W=V(:,sidx);
    svm_train_data=svm_train_data*W;
    svm_test_data=svm_test_data*W;
end

svm_train_data=svm_train_data*svmscale;
svm_test_data=svm_test_data*svmscale;

%option='-c 0.5 -t 2 -b 1';
option=['-c 0.5 -t ' num2str(kernel_type) ' -b 1'];
disp('train svm...');
model = svmtrain(svm_train_label, svm_train_data, option);
[predict_label, a, dec_values] = svmpredict(svm_test_label, svm_test_data, model,'-b 1');
accuracy=a(1);
confusion_mat_ratio=create_confusion_mat(svm_test_label,predict_label);
