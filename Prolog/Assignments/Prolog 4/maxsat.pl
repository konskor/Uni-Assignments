:-lib(ic).
:-lib(branch_and_bound).


maxsat(NV,NC,D,F,S,M):-

	create_formula(NV,NC,D,F),	% creating the formula F = formula,


	% constrains 
	length(S,NV),		% creating S (solution list which contains 0 and 1 ==> False and True values, for each variable)
	S #:: 0..1,

	length(Sc,NC),		% a list which will contain 0 and 1, if a clause is true or false
	Sc #:: 0..1,		% the sum M is the value which we try to MAXIMIZE ( 0 + 1 + 1 + 0 + ... + 0 = M)

	M #> 0,
	M #=< NC,		% 0 <= M <= NC 
	M #= sum(Sc),	

	constrains(F,S,Sc,0,Counter),		% Counter will contain the number of empty clauses ==> always false

%	Max is NC - Counter,
	Max is Counter - NC,

	write("Empty Clauses are "),
	write(Counter),
	write("\n"),

	Profit #= M,		% for bb_min in ic libra
	Cost #= -Profit,

%	bb_min( search([S,M,Sc],0,first_fail,indomain_middle,complete,[]), Cost, bb_options{ strategy:continue, from:Max, to:0} ).

	% correct
%	bb_min( search([S,M],0,first_fail,indomain_middle,complete,[]), Cost, bb_options{ strategy:continue, from:Max, to:0} ).


	% testing
%	bb_min( search([S,M],0,first_fail,indomain_middle,complete,[]), Cost, bb_options{ strategy:restart, from:Max, to:0} ).  		=> Good

	bb_min( search([S,M],0,first_fail,indomain_middle,complete,[]), Cost, bb_options{ strategy:dichotomic, from:Max, to:0} ).		=> even better



constrains([],_,[],Counter,Counter).
constrains([Clause|L],S,[CValue|L1], Counter_temp, Counter):-		% Counter will contain the number of empty clauses ==> always false

	( Clause = [] -> (Counter_temp1 is Counter_temp + 1) ;
	Counter_temp1 is Counter_temp), 
	
	literal_constrain(Clause,S,CValue),	% CValue contains : Clause #= ( (X1 #= 1) or (X2 #= 1) .... )

	constrains(L,S,L1,Counter_temp1,Counter).



literal_constrain([],_,CValue):- CValue #= 0.	% if clause is empty
literal_constrain([X|L],S,CValue):-

	I is abs(X),
	find(S,I,Sx),

	( X > 0 -> ( Temp #= ( Sx #= 1) );
	Temp #= ( Sx #= 0) ),

	literal_constrain1(L,S,Temp,CValue).




literal_constrain1([],_,Temp,CValue):- CValue #= Temp.
literal_constrain1([X|L],S,Temp,CValue):-

	I is abs(X),
	find(S,I,Sx),

	( X > 0 -> Temp1 #= ( Temp or ( Sx #= 1) );
	Temp1 #= ( Temp or ( Sx #= 0) ) ),

	literal_constrain1(L,S,Temp1,CValue).


find([X|_],I,X):- I is 1, !.
find([_|L],I,Y):- 
	I > 1,
	I1 is I - 1,
	find(L,I1,Y).

 







create_formula(NVars, NClauses, Density, Formula) :-
   formula(NVars, 1, NClauses, Density, Formula).

formula(_, C, NClauses, _, []) :-
   C > NClauses.
formula(NVars, C, NClauses, Density, [Clause|Formula]) :-
   C =< NClauses,
   one_clause(1, NVars, Density, Clause),
   C1 is C + 1,
   formula(NVars, C1, NClauses, Density, Formula).

one_clause(V, NVars, _, []) :-
   V > NVars.
one_clause(V, NVars, Density, Clause) :-
   V =< NVars,
   rand(1, 100, Rand1),
   (Rand1 < Density ->
      (rand(1, 100, Rand2),
       (Rand2 < 50 ->
        Literal is V ;
        Literal is -V),
       Clause = [Literal|NewClause]) ;
      Clause = NewClause),
   V1 is V + 1,
   one_clause(V1, NVars, Density, NewClause).

rand(N1, N2, R) :-
   random(R1),
   R is R1 mod (N2 - N1 + 1) + N1.

