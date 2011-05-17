function w = lambert_arg_outof_range(x)
% Computes value of the lambert function W(z)
% for values of z = -exp(-x) that are outside the range of values
% of digital floating point representations.
%
% Algorithm:
% Eq (38) and Eq (39), page 23 from
% Brand, M. "Structure learning in Conditional Probability
% Models via an entropic prior and parameter extinction", 1998.
% Available at: http://www.merl.com/reports/docs/TR98-18.pdf
%
% Madhu Shashanka, <shashanka@cns.bu.edu>

w = x;
if ~isempty( x)
	while 1
		wo = w;
		w = x - log( abs( w));
		if max( abs( (w-wo)./wo)) < eps
			break
		end
	end
end