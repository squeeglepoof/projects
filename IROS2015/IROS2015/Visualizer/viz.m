% Pygame and numpy were used to make the visualization. 
% Download pygame here: http://www.pygame.org/download.shtml
% Make sure you download the installer that matches your python version
if exist('viz.py', 'file') ~= 2
    disp 'Error: This script must be in same location as the visualizer python script'
    return
end
% This calls the python executable and runs the visualizer script.
% Script takes one argument, which is the location of the data.
% The data required:
%   edges.csv
%   nodes.csv
%   link_UAVs_trained.csv and link_UAVs_untrained.csv
%   sector_UAVs_trained.csv and sector_UAVs_untrained.csv
system('python viz.py .\')

% Ask to save movie files
x = input('Do you want to save the previous visualizations to movie files? [Y/n]: ', 's');

% If yes...
if strcmp(x, 'y') || strcmp(x, 'Y')
    disp 'Converting to movies...'
    % The python script creates a directory called "frames"
    % It contains the frames required to make the movies
    cd('frames');
    
    % Number of frames per movie is how many steps each epoch is
    num_frames = length(dir(fullfile('Untrained*.png')));
    
    v = VideoWriter('..\untrained.avi');
    open(v);
    for i = 1:num_frames
        img = imread(['Untrained', int2str(i-1), '.png']);
        fr = im2frame(img);
        % This slows down the video so it's more pleasing to the eye :)
        for n = 1:4
            writeVideo(v, fr);
        end
    end
    close(v);
    
    v = VideoWriter('..\trained.avi');
    open(v);
    for i = 1:num_frames
        img = imread(['Trained', int2str(i-1), '.png']);
        fr = im2frame(img);
        % This slows down the video so it's more pleasing to the eye :)
        for n = 1:4
            writeVideo(v, fr);
        end
    end
    close(v);
    
    cd('..')
else
    disp 'OK! All done. Be sure to drink your Ovaltine!'
end