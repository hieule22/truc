program semt3;

# Semantic test 4
# Shouldn't compile

  	i1, i2: int;
  	b1, b2: bool;

begin
  
  	# Lots of semantic errors.  Hopefully your
  	# program will find at least one.  Ideally,
  	# of course, you should find the first one.

  	# compare boolean and integer
	while (7 = b1) loop begin
		i1 := 1;
	end;

	# assign boolean to integer
	i1 := b1;

  	# assign integer to boolean
	# division by 0 is not checked for by the compiler. Should it be?
  	b1 := 0 / 0;
	
  	# combine boolean and integer
  	if b1 or i2 then begin
		i1 := 0;
	end else begin
		i2 := 0;
	end;
	
  	# perform boolean operation on integer
  	b1 := b1 or b2 and not b3 or (b1 and not (b2 or b3) or 0);
	
  	# perform integer operation on bool
  	i1 := i1 / i2 * (0 - -7) / -0 * (+i1 + (b1 or b2));

end;
