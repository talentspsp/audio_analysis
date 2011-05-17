function confusion_mat=create_confusion_mat(truth,predict,option)
% truth: a vector containing the true label
% predict: a vector containing the predicted labels
% option: 0 ratio, 1 number, default as 0
% confution_mat: x axis is the truth label, y axis is the predict label
if length(truth)~= length(predict)
    error('truth and predict have different length');
end

if nargin<3
    option=0;
end

m=max(truth);
confusion_mat=zeros(m,m);

for i=1:m
    idx_true=(truth==i);
    ntrue=sum(idx_true);
    pred=predict(idx_true);
    for j=1:m
        npred=sum(pred==j);
        if option==0
	  confusion_mat(j,i)=npred/ntrue;
        else
	  confusion_mat(j,i)=npred;
        end
    end
end




