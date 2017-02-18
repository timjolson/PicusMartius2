function p = LineGradient( goal, endpoint, unitV )
p = 2*(unitV*(goal - endpoint)*unitV - (goal - endpoint));


end