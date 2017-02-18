function p = LinePotential( goal, endpoint, unitV )
p = ((goal - endpoint) - (goal - endpoint)*unitV*unitV)^2;


end