% clear all
% close all
% clc

load('G_log.txt');

hold on

plot(moving_average(G_log(1,:),5),'b')
plot(moving_average(G_log(2,:),5),'r')
plot(moving_average(G_log(3,:),5),'g')