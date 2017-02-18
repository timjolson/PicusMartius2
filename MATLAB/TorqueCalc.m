clear all;

%TODO: verify sub-assembly moments and torques for their joints

len(:) = [ 530 410 160 80 ]; %mm length between pivot points
len = len./1000; %-> m

g = [ 9.81 0 0 ];

cgOffset(1,:) = [ 134.3 0 0 ]; %mm length, mm to center of mass from pivot
cgOffset(2,:) = [ 173.4 0 0 ]; %mm length, mm to center of mass from pivot
cgOffset(3,:) = [ 86.11 0 0 ];
cgOffset(4,:) = [ 40.0  0 0 ];

% Member Moments of Inertia
moment(1,:) = [ .0612 .0072 .0612 ];
moment(2,:) = [ .0099 .0003 .0100 ];
moment(3,:) = [ .0009 .0008 .0009 ];
moment(4,:) = [ 0.0037 .0022 .0037 ]; % lifted object

% Mass
m(1) = 2.6987; % kg
m(2) = .5916;
m(3) = .8113;
m(4) = 5;

totalMass = sum(m);

maxLength = sum(len);
cgOffset = cgOffset./1000;	% -> meters

g = 9.81; % m/s^2

% Gear pitch diam's
% joint 1
gear(1,1) = 35; % worm gears
gear(1,2) = 45; % tilt bevel gears
gear(1,3) = 45; % rotate bevel gear

%joint 2
gear(2,1) = 16; % belt pulley1
gear(2,2) = 24; % belt pulley2
gear(2,3) = 35; % worm gear

%joint 3
gear(3,1) = 20; % worm gears
gear(3,2) = 24; % tilt bevel gears
gear(3,3) = 24; % rotate bevel gear

gear = gear./1000; % diameters -> m

% Desired time to tilt 90 degress from stop
t90(1) = 5; % seconds
t90(2) = 3;
t90(3) = 2;
angle = 90 * pi/180;

% desired avg. angular acceleration
alpha(1) = 2*(angle)/(t90(1)^2);	% rad/s^2
alpha(2) = 2*(angle)/(t90(2)^2);
alpha(3) = 2*(angle)/(t90(3)^2);

alphaDeg = alpha *180/pi;

angleCheck(1) = (1/2)*alpha(1)*t90(1)^2;
angleCheck(2) = (1/2)*alpha(2)*t90(2)^2;
angleCheck(3) = (1/2)*alpha(3)*t90(3)^2;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% STATIC CALCULATIONS
%%%%%%%%%%%%%%%%%%%%
% Torques at each pivot point
% Newtons * meters
for i =1:3
staticT(4) = m(4)*cgOffset(4,i);
staticT(3) = m(4)*(cgOffset(4,i)+len(3))+ ...
	m(3)*cgOffset(3,i);
staticT(2) = m(4)*(cgOffset(4,i)+len(3)+len(2)) + ...
	m(3)*(cgOffset(3,i)+len(2)) + ...
	m(2)*cgOffset(2,i);
staticT(1) = m(4)*(cgOffset(4,i)+len(3)+len(2)+len(1))...
	+ m(3)*(cgOffset(3,i)+len(2)+len(1))...
	+ m(2)*(cgOffset(2,i)+len(1)) + ...
	m(1)*cgOffset(1,i);
staticT = staticT*g;
end

for i=1:2:3
subI(4,i) = moment(4,i) + m(4)*cgOffset(4,i)^2;
subI(3,i) = moment(3,i) + m(3)*cgOffset(3,i)^2 + ...
	m(4)*(len(3)+cgOffset(4,i))^2 ...
	+ moment(4,i);
subI(2,i) = moment(2,i) + m(2)*cgOffset(2,i)^2 + ...
	m(3)*(cgOffset(3,i)+len(2))^2 + ...
	m(4)*(len(3)+cgOffset(4,i)+len(2))^2 ...
	+ moment(4,i) + moment(3,i);
subI(1,i) = moment(1,i) + m(1)*cgOffset(1,i)^2 + ...
	m(2)*(len(1)+cgOffset(2,i))^2 + ...
	m(3)*(cgOffset(3,i)+len(2)+len(1))^2 + ...
	m(4)*(len(3)+cgOffset(4,i)+len(2)+len(1))^2 ... 
	+ moment(4,i) + moment(3,i) + moment(2,i);
end
i =2;
subI(4,i) = moment(4,i) + m(4)*cgOffset(4,i)^2;
subI(3,i) = moment(3,i) + moment(4,i);
subI(2,i) = moment(2,i) + moment(4,i) + moment(3,i);
subI(1,i) = moment(1,i) + moment(4,i) + moment(3,i) + moment(2,i);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% DYNAMIC CALCULATIONS
%%%%%%%%%%%%%%%%%%%%
% Dynamic torque without gravity at each pivot point
% Newtons * meters
% Using parallel axis theorem
for i=1:3
T(4,i) = (moment(4,i)+m(4)*cgOffset(4,i)^2)*alpha(3);
T(3,i) = (moment(3,i)+m(3)*cgOffset(3,i)^2 + ...
	moment(4,i)+m(4)*(cgOffset(4,i)+len(3))^2)*alpha(3);
T(2,i) = (moment(2,i)+m(2)*cgOffset(2,i)^2 + ...
	moment(3,i)+m(3)*(len(2)+cgOffset(3,i))^2 + ...
	moment(4,i)+m(4)*(len(2)+len(3)+cgOffset(4,i))^2)*alpha(2);
T(1,i) = (moment(1,i)+m(1)*cgOffset(1,1)^2 + ...
	moment(2,i)+m(2)*(cgOffset(2,i)+len(1))^2 + ...
	moment(3,i)+m(3)*(cgOffset(3,i)+len(2)+len(1))^2 + ...
	moment(4,i)+m(4)*(cgOffset(4,i)+len(3)+len(2)+len(1))^2 ...
	)*alpha(1);
end


%%%%%%%%%%%%%%%%%%%
% Dynamic torque with gravity at each pivot point
% Newtons * meters
% 
DT(4,1) = T(4,1) + staticT(4);
DT(3,1) = T(3,1) + staticT(3);
DT(2,1) = T(2,1) + staticT(2);
DT(1,1) = T(1,1) + staticT(1);
for i=2:3
DT(4,i) = T(4,i);
DT(3,i) = T(3,i);
DT(2,i) = T(2,i);
DT(1,i) = T(1,i);
end


%%%%%%%%%%%%%%%%%%%%
% Gear tangential force (dynamic torque) for each joint
% Newtons
Ft(3) = DT(3,1) / (gear(3,2)/2);
Ft(2) = DT(2,1) / (gear(2,3)/2);
Ft(1) = DT(1,1) / (gear(1,2)/2);




