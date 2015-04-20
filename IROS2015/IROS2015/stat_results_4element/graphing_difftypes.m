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

%%  

%[x,y] = meshgrid(1,size(obstacle_map,1),1:size(obstacle_map,2));
%combo = [x(:),y(:),obstacle_map(:)];
Path0 = csvread('path-0.csv');

% Plot the paths over the image
figure(1)
imshow(obstacle_map)
for i=1:2:size(Path0,1)
   hold on
   scatter(Path0(i+1,:)+1,Path0(i,:)+1);
end

figure(2)
Path50 = csvread('path-50.csv');
imshow(obstacle_map)
for i=1:2:size(Path50,1)
   hold on
   scatter(Path50(i+1,:)+1,Path50(i,:)+1);
end
%%

figure(3)
trace50 = csvread('trace.csv');
imshow(obstacle_map)
for i=1:2:size(trace50,1)
   hold on
   scatter(trace50(i+1,:)+1,trace50(i,:)+1);
end

%% high level path looking at

high_path = [4,8,9];


imshow(obstacle_map)
hold on
for i=1:length(high_path)
   imshow(membership_map==high_path(i));
   alpha(.5)
end
