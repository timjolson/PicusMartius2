function p = DirectionGradient( goal, endpoint, bodyVector )
degrees(1) = 0.5; % 1/2 degree in x as important as 1 length unit
degrees(2) = 0.5; % 1/2 degree in x as important as 1 length unit
degrees(3) = 0.5; % 1/2 degree in x as important as 1 length unit

c_d = 180/(pi*degrees);

p(1) = -2*(c_d^2) *( (goal - endpoint)/vLength(goal - endpoint) - bodyVector );
p(2) = 2*(c_d^2) * (vLength(goal - endpoint)^2 * bodyVector - (goal - endpoint)*bodyVector*(goal - endpoint) ;

end