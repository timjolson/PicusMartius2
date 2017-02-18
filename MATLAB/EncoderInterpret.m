clear all;
close all;

res = 4096;

offset = [ 4 8 ];
ratio = [ 5 4 ];

x = zeros(res);
e_1 = 0;
e_2 = 0;
for theta = 0:res
    x(e_1+2,e_2+1) = theta*360/res;
    x(e_1+1,e_2+2) = theta*360/res;
    x(e_1+2,e_2+2) = theta*360/res;
    x(e_1+1,e_2+1) = theta*360/res;
    e_1 = floor(mod(theta*ratio(1)*360/res+offset(1),res));
    e_2 = floor(mod(theta*ratio(2)*360/res+offset(2),res));
end

p = 1:res;
plot3(p,p,x(p,:));
%plot (theta,theta,theta,e_1,theta,e_2);
