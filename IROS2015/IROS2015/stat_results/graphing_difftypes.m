% For graphing the different types of filtering.
clear all
close all
clc

n_runs = 5;

blind = [];
weighted = [];
crossweighted = [];
multimind = [];

for i=1:n_runs
   blind = [blind; load(strcat(strcat('blind_conflict-',int2str(i)),'.txt'))];
   weighted = [blind; load(strcat(strcat('weighted_conflict-',int2str(i)),'.txt'))];
   crossweighted = [blind; load(strcat(strcat('crossweighted_conflict-',int2str(i)),'.txt'))];
   multimind = [blind; load(strcat(strcat('multimind_conflict-',int2str(i)),'.txt'))];
end

%%

plot(-moving_average(mean(blind_conflict),5),'c*-')
plot(-moving_average(mean(weighted_conflict),5),'b')
plot(-moving_average(mean(crossweighted_conflict),5),'k')
plot(-moving_average(mean(multimind_conflict),5),'g')

