clear all;
clc
feat_opt=2;
cov_opt=0;
LDA_opt=1;
svmscale=[1 10 20 30 40 50 60 70 80 90 100];

kernel_type=0;

disp('linear kernel');
for i=1:length(svmscale)
    disp(['scale ' num2str(svmscale(i))]);
    [accuracy confusion_mat_ratio]=test_feature(feat_opt,cov_opt,LDA_opt,svmscale(i),kernel_type);
end

kernel_type=2;

disp('gaussian kernel');
for i=1:length(svmscale)
    disp(['scale ' num2str(svmscale(i))]);
    [accuracy confusion_mat_ratio]=test_feature(feat_opt,cov_opt,LDA_opt,svmscale(i),kernel_type);
end