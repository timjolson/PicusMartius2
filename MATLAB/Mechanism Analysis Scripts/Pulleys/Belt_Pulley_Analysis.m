%% Pulley Torque Calculations
% Based on "The World of Timing Belts",
% "Timing Belts, Pulleys, Chains, and Sprockets" from www.SDP-SI.com, and 
% "Synchronous Belt Data Summary" from Gates(R)
%
% Written by Tim Olson

% NOTES
% Uses pulley info to find and check belt tension and find motor requirements.
% Does not check torque ratings of pulleys.
clear all; close all; clc;

%--------- inputs
outputTorque = 6.6; % torque req'd (Nm)
motorTorqueFOS = 1; % used for final output of motor requirements
beltType = '3mm GT2'; % 2mm GT2 / 3mm GT2, others can be added to BeltTables.xlsx
motorPulleyTeeth = 16;
drivenPulleyTeeth = 60;
beltWidth = 6;  % mm
beltFOScheck = 1.2; % pass/fail check on belt

% set pass/fail based on recommended tension or yielding tension
FOSmethod = 'yield';
%FOSmethod = 'recommended';

% rotation of driven pulley (used for rpm and acceleration
% in motor section of output)
deg = 45; % degrees traveled
time = 6; % time traveled

%--------- start calculations
%get pulley tables
BeltTables = xlsread('BeltTables.xlsx', beltType);

% conversions
NmTOozin = 141.611932278;
NTOlbs = 0.224808942443;

%--------- ratios and pulley sizes
disp('---Pulleys---');
ratio = motorPulleyTeeth/drivenPulleyTeeth;
[n,d]=rat(ratio);

% look up pitch diameters
motorPitchDiam = BeltTables((motorPulleyTeeth-uint8(BeltTables(1,1)))+1,2);
wristPitchDiam = BeltTables((drivenPulleyTeeth-uint8(BeltTables(1,1)))+1,2);

% print pulley info
fprintf('Motor Pulley:: Teeth: %d  PitchDiam(mm): %1.2f',motorPulleyTeeth,motorPitchDiam);
fprintf('\nWrist Pulley:: Teeth: %d  PitchDiam(mm): %1.2f',drivenPulleyTeeth,wristPitchDiam);
fprintf('\nRatio::  %d : %d',n,d);

%--------- motor info
fprintf('\n\n---Motor---');
motorTorque = ratio*outputTorque*motorTorqueFOS;
fprintf('\nMin Stall Torque (oz-in): %1.f', motorTorque*NmTOozin);
fprintf('\nMin Stall Torque (N*m): %1.2f', motorTorque);
fprintf('\nAvg Angular Accel. (deg/s/s): %1.2f', (2*deg/time^2)/ratio);
fprintf('\nAvg RPM: %1.1f', (deg/time)*(60/360));
fprintf('\nPeak RPM: %1.1f', ((2*deg/time^2)/ratio)*time*60/360);

%--------- belt tensile info
% TODO: add to spreadsheet to automate
if strcmp(beltType,'2mm GT2')
    beltTensYield = 5338;
    beltTensRec = 111;
elseif strcmp(beltType,'3mm GT2')
    beltTensYield = 9786;
    beltTensRec = 507;
end
beltTensYield = ((beltTensYield/2)/25.4)*beltWidth;
beltTensRec = ((beltTensRec/2)/25.4)*beltWidth;

% correct for small belts
if beltWidth < (.25*25.4)
    beltTensYield = 0.82*beltTensYield;
elseif (.25*25.4) < beltWidth < (.5*25.4)
    beltTensYield = 0.9*beltTensYield;
end

%--------- belt tension
tension = motorTorque / (motorPitchDiam/2000);
fprintf('\n\n---Belt---');
fprintf('\nTension (N): %1.1f',tension);
fprintf('\nTension (lbs): %1.1f\n',tension*NTOlbs);

if strcmp(FOSmethod,'yield')
    beltTens = beltTensYield;
elseif strcmp(FOSmethod,'recommended')
    beltTens = beltTensRec;
end

if beltFOScheck*tension < beltTens
    fprintf('\nPass, %s Belt\nFOS: %1.2f\n',beltType, beltTens/tension);
else
    fprintf('\nFail, %s Belt\nFOS: %1.2f\n',beltType, beltTens/tension);
end


