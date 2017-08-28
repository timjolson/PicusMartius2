%MEE 623 Project 3 MATLAB PseudoCode

clc;
clear all;
close all;

show = 1; %Plot toggle

%Read in images from folder (color, channeled, and bw)
s_seq=10;
e_seq=28;
    tlength = e_seq - s_seq+1;
for seq=1:tlength
    floc=['..', filesep, 'data', filesep'];
    frmid='frame';
    files=dir([floc, frmid, '*.jpg']);
    sample_frm{seq}=imread([floc, files(seq).name]);
        r_channel{seq} = sample_frm{seq}(:,:,1);
        g_channel{seq} = sample_frm{seq}(:,:,2);
        b_channel{seq} = sample_frm{seq}(:,:,3);
    bw_img{seq}=rgb2gray(imread([floc, files(seq).name]));
    hsv_img{seq}=rgb2hsv(imread([floc, files(seq).name]));
end
bat = imread(['..', filesep, 'data', filesep, 'test images', filesep, 'image000000.jpg']);
    rgb_bat = [112 149 80];
    
sample_pic = imread([floc, files(1).name]);
    x_length = length(sample_pic(1,:,1));
    y_length = length(sample_pic(:,1,1));

%Convert images hsv and process
low_hue = 0.24;
high_hue = 0.28;
low_sat = 0.33;
high_sat = 0.5;
threshold = [0.1 0.15];
sigma = 4;

for i=1:tlength
    hue_filtered{i} = hsv_img{i}(:,:,1) > low_hue & hsv_img{i}(:,:,1) < high_hue;
    sat_filtered{i} = hsv_img{i}(:,:,2) > low_sat & hsv_img{i}(:,:,2) < high_sat;
    full_image{i} = (hue_filtered{i} + sat_filtered{i}) == 2;
    %canny_img{i} = edge(full_image{i},'canny',threshold, sigma);

%Create Bounding Box and Area Parameters
%Variables to examine
feat1 = 'Centroid';
feat2 = 'BoundingBox';
feat3 = 'Area';

stats = regionprops(full_image{i},feat1,feat2,feat3);
area = [stats.Area].';
[~,ind] = max(area);
box = stats(ind).BoundingBox; %Create box around moving area

if show
    %stats(ind)
end

%Plotting of Image, Bounding Box, etc.
if show
    figure(1);
    hold on
    imshow(full_image{i})
    rectangle('Position', box, 'EdgeColor', 'red');
    pause;
end

%Create Robot Control Conditions
error = 30; %allowable pixel error

if stats(ind).Centroid(1) == 0 %Robot sees nothing, spin
    disp('Spinning to find object');
elseif stats(ind).Centroid(1) < (x_length/2 - error) %Robot is facing right of object, turn left
    disp('Too far left. Turning left to center object.');
elseif stats(ind).Centroid(1) > (x_length/2 + error) %Robot is facing left of object, turn right
    disp('Too far right. Turning right to center object.');
else %Robot is facing object (mostly), move forward
    disp('Facing object.');
    if stats(ind).BoundingBox(3) < (250 - error) %pixel width of bat when ~1 ft away
        disp('Object too far away. Moving Forward.');
    elseif stats(ind).BoundingBox(3) > (250 + error)
        disp('Object too close. Moving Backwards.');
    else %Object is within optimal range
        disp('Object within optimal distance. Stopping.');
    end
end

end