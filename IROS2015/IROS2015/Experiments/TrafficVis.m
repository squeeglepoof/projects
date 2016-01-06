
clear all
close all
clc

locations = csvread('C:\Users\Carrie\Documents\Visual Studio 2012\Projects\projects\IROS2015\IROS2015\locations0.csv');
agent_locations = csvread('C:\Users\Carrie\Documents\Visual Studio 2012\Projects\projects\IROS2015\IROS2015\agent_locations0.csv');


% FOR VISUALIZATION
% Need to know location of UAVs
% Need to know territories of agents
% Need to show this in map (video)

% For abstraction with state congestion, it will be either in or out of the sector

agent_locations(end)=[];
agent_locations = reshape(agent_locations,2,length(agent_locations)/2); % reshape to xy
agent_traffic = zeros(length(locations),length(agent_locations));

for step = 1:length(locations)
   steploc = locations(step,:);
   steploc = steploc(steploc~=0); % Remove zero padding
   steploc = reshape(steploc,2,length(steploc)/2); % Reshape to xy
   
   
   % Count the number at each location
   for a = 1:length(agent_locations)
       agent_traffic(step,a) = nnz(steploc(2,:)==agent_locations(2,a) & steploc(1,:)==agent_locations(1,a));
   end
   agent_traffic;
end

%% colored voronoi regions


x = agent_locations';
[v,c] = voronoin(x);
for i = 1:length(c)
if all(c{i}~=1)   % If at least one of the indices is 1,
                  % then it is an open region and we can't
                  % patch that.
patch(v(c{i},1),v(c{i},2),i); % use color i.
end
end

%% find enclosing line segments for each agent

%% fill with corresponding congestion color



%%


for i=1:length(agent_traffic)
    i
    hmap = zeros(200);
    hmap(sub2ind(size(hmap),agent_locations(1,:),agent_locations(2,:))) = agent_traffic(i,:);

    pause(0.1)
    image(imfilter(hmap*10,fspecial('gaussian',100),'replicate'))
end
