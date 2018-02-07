function [Kv] = Get_Dynamic_Load_Factor(mod_tooth_prof, tooth_prof, v)

% Read in table
table = xlsread('SpurTables','Dynamic Load Factor','C4:I10');

% Determine row and column indices based upon tooth profile and tangential
% speed of the gear
if mod_tooth_prof == true
    switch tooth_prof
        case 1
            r_index = 1;
        case 2
            r_index = 2;
        case 3
            r_index = 3;
        case 4
            r_index = 4;
        otherwise
            fprintf('Invalid modified tooth profile number: %d.',tooth_prof);
    end
else
    switch tooth_prof
        case 1
            r_index = 2;
        case 2
            r_index = 3;
        case 3
            r_index = 4;
        case 4
            r_index = 5;
        case 5
            r_index = 6;
        case 6
            r_index = 7;
        otherwise
            fprintf('Invalid unmodified tooth profile number: %d.',tooth_prof);
    end
end

if (v < 1)
    c_index = 1;
elseif (v >=1) && (v < 3)
    c_index = 2;
elseif (v >= 3) && (v < 5)
    c_index = 3;
elseif (v >= 5) && (v < 8)
    c_index = 4;
elseif (v >= 8) && (v < 12)
    c_index = 5;
elseif (v >= 12) && (v < 18)
    c_index = 6;
elseif (v >= 18) && (v < 25)
    c_index = 7;
else
    fprintf('Invalid tangential speed of: %1.2f.',v);
end

% Retrieve value from table
Kv = table(r_index,c_index);

end