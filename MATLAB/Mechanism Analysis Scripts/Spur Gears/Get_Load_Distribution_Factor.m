function [Y_epsilon] = Get_Load_Distribution_Factor(z1, z2,m,a,alphaw,da1,da2,db1,db2)

ep_alpha = (sqrt((da1/2)^2-(db1/2)^2)+sqrt((da2/2)^2-(db2/2)^2)...
    -a*sin(alphaw))/...
    (pi*m*cos(alphaw));

% Calculating the load distribution factor
Y_epsilon = 1 / ep_alpha;

end