% Plots the blind conflict with error bars
function [vals, avg, err] = load5files(fname)

num_vals = 5;

vals = [];
for i=0:(num_vals-1)
    vals = [vals; load([fname '-' int2str(i) '.txt'])];
end

avg = mean(vals);

err = std(vals)/num_vals;

end