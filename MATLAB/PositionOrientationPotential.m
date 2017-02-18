function p = OrientationPotential( goal, endpoint, goalOrthos, endOrthos )
w(1) = 1;
w(2) = 2;

w(1) = w(1) / sum(w);
w(2) = w(2) / sum(w);

p = w(1) * PositionPotential(goal, endpoint) + w(2) * OrientationPotential(goalOrthos,endOrthos);

end %end function