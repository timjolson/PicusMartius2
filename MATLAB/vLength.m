function l = vLength ( input )
sum =0;
for i=1:length(input)
	sum = sum + input(i) * input(i);
end
l = sqrt(sum);

end