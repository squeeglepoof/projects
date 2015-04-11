% For graphing the different types of filtering.
clear all
close all
clc

n_runs = 5;

blind = [];
weighted = [];
crossweighted = [];
multimind = [];

for i=0:(n_runs-1)
   blind = [blind; load(strcat(strcat('blind_conflict-',int2str(i)),'.txt'))];
   weighted = [weighted; load(strcat(strcat('weighted_conflict-',int2str(i)),'.txt'))];
   crossweighted = [crossweighted; load(strcat(strcat('crossweighted_conflict-',int2str(i)),'.txt'))];
   multimind = [multimind; load(strcat(strcat('multimind_conflict-',int2str(i)),'.txt'))];
end

%%

hold on

plot(-moving_average(mean(blind),5),'c*-')
plot(-moving_average(mean(weighted),5),'b')
plot(-moving_average(mean(crossweighted),5),'k')
plot(-moving_average(mean(multimind),5),'g')

legend('blind','weighted','crossweighted','multimind');
xlabel('epoch')
ylabel('conflict count')

title('Comparison of different methods for agent type conflict radius=1 and conflict radius = 5')