%% Spur Gear Torque Calculations
% Based on "Elements of Metric Gear Technology" from www.SDP-SI.com
%
% Compiled by Thomas Jareczek and Tim Olson
close all; clear all; clc;

global show;
show = 1;  % show plots of interpolated values

%--------------------Inputs--------------------------------

% Young's Modulus, Poisson's Ratio, surface roughness, hardness
E1 = 193; % GPa, 316  SS
pr1 = 0.25;
E2 = 193; % GPa, 316 SS
pr2 = 0.25;
rough1 = 3.2e-6; % m - assuming ISO 8
rough2 = 3.2e-6; % m - assuming ISO 8
hardness1 = 194; % Brinell Hardness, 316 SS
hardness2 = 187; 

% Module, Pressure Angle
m = 1.5; % mm
alpha = 20; % °

% Beta (helix angle)
beta = 0;

% Number of teeth
z1 = 12; % Pinion (driven, smaller, less teeth)
z2 = 40; % Gear (driver, larger, more teeth)

% % Output torque (actually smaller gear)
% T = 1.28; % Nm
% T = T*z2/z1; % convert to input torque for calculations

% Input torque (actually bigger gear)
T = 6.; % Nm

% Tooth width / Face Width
b1 = 10; % mm , small gear
b2 = 10; % mm , large gear

% Modified tooth profile (true or false)
% Tooth profile number
mod_tooth_prof = false;
tooth_prof = 4; % See SDP SI sheet, page 154 - http://www.sdp-si.com/PDFS/Elements-of-Metric-Gear-Technology.pdf

% Coefficient of profile shift
X = 0;

% % Rotating speed of smaller gear
% n1 = 30; % rpm (actual smaller gear)
% n2 = n1*z1/z2; % (actual larger gear)

% Rotating speed of larger gear
degSec = 17; % degrees/second rotation
n2 = degSec*60/360; % (actual larger gear)
n1 = n2*z2/z1; % rpm (actual smaller gear)

% Allowable bending stress
sigmaf_lim = 19; % kgf/mm^2 (estimated from chart and Brinell Hardness, 316 SS)

% Factor of safety for calculating limits
% Displayed FOS is calculated after this is accounted for.
Sf = 1.0;

% Allowable Hertz stress (USED FOR SURFACE STRENGTH CALCULATIONS)
deltah_lim = 50; % kgf/mm^2

% ---------------End Inputs----------------------------


% Conversions
alpha = deg2rad(alpha);
beta = deg2rad(beta);
kgfTON = 9.807;
NmTOozin = 141.611932278;
NTOlbs = 0.224808942443;
GPaTOkgfmm2 = 101.9716212978;
T = T / kgfTON;
E1 = E1 * GPaTOkgfmm2;
E2 = E2 * GPaTOkgfmm2;

% Teeth ratio
u = z2/z1; % driven / driving

% Center distance
a = ((z1+z2)*m)/2; % mm

% Center distance increment factor
gamma = (a/m) - (z1+z2)/2;

% Working pressure angle
alphaw = acos(((z1+z2)*cos(alpha))/(2*gamma+z1+z2));

% Pitch diameter
d1 = z1*m;
d2 = z2*m;

% Base diameter
db1 = d1*cos(alpha);
db2 = d2*cos(alpha);

% Working pitch diameter
dw1 = db1/cos(alphaw); % mm
dw2 = db2/cos(alphaw); % mm

% Addendum & Dedendum
ha = 1*m;
hf = 1.25*m;

% Outside diameter
da1 = d1+2*m;
da2 = d2+2*m;

% Root diameter
df1 = d1-2.5*m;
df2 = d2-2.5*m;

% Tangential speed
v1 = (dw1*n1)/19100; % m/sec
v2 = (dw2*n2)/19100; % m/sec


% ----------------Bending Factors-----------------------

% Tooth profile factor
Yf1 = Get_Tooth_Profile_Factor(X,beta,z1);
Yf2 = Get_Tooth_Profile_Factor(X,beta,z2);

% Load distribution factor
% Y_ep = 1 / ep_alpha = 1 / contact ratio
Y_ep = Get_Load_Distribution_Factor(z2,z1,m,a,alphaw,da1,da2,db1,db2);

% Helix angle factor
if (beta >= deg2rad(0)) && (beta <= deg2rad(30))
    Y_beta = 1 - beta/deg2rad(120);
else
    Y_beta = 0.75;
end

% Life Factor - Using 10^7 cycles
Kl = 1.0;

% Dimension factor of root stress
Kfx = 1.0;

% Dynamic load factor
Kv1 = Get_Dynamic_Load_Factor(mod_tooth_prof, tooth_prof, v1);
Kv2 = Get_Dynamic_Load_Factor(mod_tooth_prof, tooth_prof, v2);

% Overload factor
% Uniform Load - 1.0
% Medium Impact Load - 1.25
% Heavy Impact Load - 1.75
Ko = 1;

%------------End Bending Factors--------------------------


%-----------Bending Forces / Strengths--------------------------

% Nominal Tangential force
Ft1 = (2000*T)/dw1;
Ft2 = Ft1;

% Nominal Radial force
Fr1 = Ft1*tand(alpha);
Fr2 = Fr1;

% Nominal Normal force
Fn1 = Ft1/cosd(alpha);
Fn2 = Fn1;

% Nominal Bending Stress
sigmaf1 = Ft1*((Yf1*Y_ep*Y_beta)/(m*b1))*((Kv1*Ko)/(Kl*Kfx))*Sf; % kgf/mm^2
sigmaf2 = Ft2*((Yf2*Y_ep*Y_beta)/(m*b2))*((Kv2*Ko)/(Kl*Kfx))*Sf; % kgf/mm^2

% Tangential Force Limits
Ft_lim1 = sigmaf_lim*((m*b1)/(Yf1*Y_ep*Y_beta))*((Kl*Kfx)/(Kv1*Ko))*(1/Sf); % kgf
Ft_lim2 = sigmaf_lim*((m*b2)/(Yf2*Y_ep*Y_beta))*((Kl*Kfx)/(Kv2*Ko))*(1/Sf); % kgf

fos = min(abs(Ft_lim1 / Ft1),abs(Ft_lim2 / Ft2));

fprintf('Pinion (smaller):\n');
fprintf('Teeth: \t\t\t\t\t%8d\n',z1);
fprintf('Pitch Diameter: \t\t%8.2f mm\n',d1);

fprintf('Tangential Force: \t\t%8.2f N\n',Ft1*kgfTON);
fprintf('Radial Force: \t\t\t%8.2f N\n',Fr1*kgfTON);
fprintf('Normal Force: \t\t\t%8.2f N\n',Fn1*kgfTON);
fprintf('Bending Stress: \t\t%8.2f N/mm^2\n',sigmaf1*kgfTON);

fprintf('Limit Tangential Force: %8.2f N\n\n',Ft_lim1*kgfTON);

fprintf('Gear (larger):\n');
fprintf('Teeth: \t\t\t\t\t%8d\n',z2);
fprintf('Pitch Diameter: \t\t%8.2f mm\n',d2);

fprintf('Tangential Force: \t\t%8.2f N\n',Ft2*kgfTON);
fprintf('Radial Force: \t\t\t%8.2f N\n',Fr2*kgfTON);
fprintf('Normal Force: \t\t\t%8.2f N\n',Fn2*kgfTON);
fprintf('Bending Stress: \t\t%8.2f N/mm^2\n',sigmaf2*kgfTON);

fprintf('Limit Tangential Force: %8.2f N\n\n',Ft_lim2*kgfTON);

fprintf('Bending Stress Limit (based on material): %8.2f N/mm^2\n\n',sigmaf_lim*kgfTON);

fprintf('Designed Factor of Safety:\t\t\t%8.2f\n',Sf);
fprintf('Actual Bending Factor of Safety:\t%8.2f\n----------------\n',fos);
if fos < 1
    fprintf('\t\tFAIL\n----------------\n\n');
else
    fprintf('\t\tPASS\n----------------\n\n');
end

%-------------End Bending Forces / Strengths--------------------

fprintf('Big Gear Torque:\t%8.2fNm\t\t%8.2fozin\n',T*kgfTON,T*kgfTON*NmTOozin);
fprintf('Small Gear Torque:\t%8.2fNm\t\t%8.2fozin\n',T*kgfTON/u,T*kgfTON*NmTOozin/u);

fprintf('Big Gear Rev:\t\t%8.2frpm\t\t%8.2fdeg/s\n',n2,n2*360/60);
fprintf('Small Gear Rev:\t\t%8.2frpm\t\t%8.2fdeg/s\n',n1,n1*360/60);

