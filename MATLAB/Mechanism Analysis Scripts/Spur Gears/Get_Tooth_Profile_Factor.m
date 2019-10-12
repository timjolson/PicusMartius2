function [Yf] = Get_Tooth_Profile_Factor(X, beta, z)
global show;
if X == 0
    x_val = [10,11,12,14,16,20,26,27,35,40,42.5,45,50,55,60,80,100,200]';
    y_val = [3.87,3.65,3.475,3.225,3.05,2.81,2.64,2.6,2.45,2.4,2.38,...
        2.36,2.33,2.31,2.28,2.23,2.19,2.15]';

    % Calculating the regression line coefficients
    % REPLACED WITH INTERPOLATION
    %fit = polyfit(x_val,y_val,3);

    % Equivalent spur gear number of teeth is given by:
    zv = z / (cos(beta))^3;
    %Yf = polyval(fit,zv); % REPLACED WITH INTERPOLATION
    
    Yf = interp1(x_val,y_val,zv);
    if show
        figure;
        plot(11:55,interp1(x_val,y_val,11:55),'b',zv,Yf,'ro');
        title(sprintf('Yf(z_v) = %1.2f',Yf));
    end

else
    fprintf('Invalid profile shift: X = %d. Yf set to 1.', X);
    Yf = 1.0;
end
end