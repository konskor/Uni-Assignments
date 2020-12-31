:-lib(ic).
:-lib(ic_global).




numpart(N,L1,L2):-

	N mod 4 =:= 0,	% constraints for N
	N >= 8,

	% Creating the orginal set L 
	
	length(L,N),
	L #:: 1..N,
	ic_global:alldifferent(L),	% all L members are different
	constrain1(0,L),	% all L members are different( increasing sequence)


	% now for the other 2 sub-sets

	Size is integer(N/2),	% size of set

	length(L1,Size),	 % creating the lists/sets
	length(L2,Size),

	ic_global:alldifferent(L1),	 % all L1 members are different
	ic_global:alldifferent(L2),	 % all L2 members are different	

	constrain1(0,L1),	% all L1 members are different( increasing sequence)
	constrain1(0,L2),	% all L2 members are different( increasing sequence)

	constrain2(L1,L2),	% first element of L1 < first element of L2

	constrain(L1,L2),	% each element of L1 #\= each element of L2
	constrain(L2,L1),	% each element of L2 #\= each element of L1

	sum(L1) #= sum(L2),
	sums(L1,L2,Sum2X,Sum2Y),

	final_constrain(L,L1,L2),	% S1 union/or S2 = S, S1 intersection/and S2 = 0

	generate( [L1,L2] ).





/*  each element of L will be put either in X or Y (mutual exclusive)

% 3 base cases 
final_constrain1([],[],[]).
final_constrain([X|L],[],[Z|L2]:-
	Z #= X,
	final_constrain1(L,[],L2).

final


final_constrain1([X|L],[Y|L1],[Z|L2]):-

	Y #= X or Z #= X,	
	final_constrain1(L,L1,L2).

*/



% each element orginates from Set L
final_constrain(_,[],[]).
final_constrain(L,[Y|L1],[Z|L2]):-

	Y #= ic:element(_,L),
	Z #= ic:element(_,L),

	Y #\= Z,

	final_constrain(L,L1,L2).









%the first element of L1 must be < first element of L2
constrain2([X|_],[Y|_]):- X #< Y.

% Ensure we have increasing order of values (and of course no duplicates)
constrain1(_,[]).
constrain1(Previous,[X|L1]):-

	X #> Previous,
	constrain1(X,L1).






constrain([],_).
constrain([X|L1],L2):-

	% X will not exist in L2 <=> every memeber
	no_contain(X,L2),

	constrain(L1,L2).	


no_contain(_,[]).
no_contain(X,[Y|L1]):-

	X #\= Y,
	no_contain(X,L1).








% The two Sum constraints can be made into one ==> Ó i(i-1) = Ó j(j-1)
% sums(L1,L2,SumX,Sum2X,SumY,Sum2Y):- sums1(L1,L2,0,0,0,0,SumX,Sum2X,SumY,Sum2Y).

sums(L1,L2,Sum2X,Sum2Y):- sums1(L1,L2,0,0,Sum2X,Sum2Y).


% base case
sums1([],[],Temp2X,Temp2Y,Sum2X,Sum2Y):-

	Sum2X #= Temp2X,

	Sum2Y #= Temp2Y,
	
	Sum2X #= Sum2Y.

sums1([X|L1],[Y|L2],Temp2X,Temp2Y,Sum2X,Sum2Y):-

	% (#=) instead of (is)
	
	Temp2X1 #= Temp2X + (X * X),

	Temp2Y1 #= Temp2Y + (Y * Y),	

	sums1(L1,L2,Temp2X1,Temp2Y1,Sum2X,Sum2Y).

	
















generate(Solution):-
	search(Solution, 0, most_constrained, indomain_middle, complete, []).



/*

generate(_,L,L).
generate(temp, L) :-

	search([X,Y], 0, first_fail, indomain_split, complete, []),

	ic_global:sorted(X,L1),
	ic_global:sorted(Y,L2),

	\+ member([L1,L2],temp),	% [L1,L2] doesn't exist in our List of solutions

	generate( [[L1,L2] |temp], L). 

	

*/



/*
constrain([],[],sumX,sumY,sum2X,sum2Y):- write("Hi2\n"), sumX = sumY, sum2X = sum2Y.
constrain([X|L1],[Y|L2],sumX,sumY,sum2X,sum2Y):-

	write('Hi3\n'),

	L1 #\= X,	% removing value of X from domain of L1, and [Y|L2]			
	[Y|L2] #\= X,

	indomain(Y,split),

	L1 #\= Y,	% removing value of Y from domain of L1, and L2
	L2 #\= Y,

	sumX1 is sumX + X,
	sum2X1 is sumX + (X * X),

	sumY1 is sumY + Y,
	sum2Y1 is sumY + (Y * Y),


	constraint1(L1,L2,sumX1,sumY1,sum2X1,sum2Y1).	
*/





