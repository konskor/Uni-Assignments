
/* Create a list of lists, ( with the list inside ASP) */

create_listoflists(ASP,LL):- create_listoflists1(ASP, [], LL).


create_listoflists1([],SoFar,SoFar).
create_listoflists1([_-L-_|T],SoFar,LL):-

	append(SoFar,[L],NewSoFar),
	create_listoflists1(T,NewSoFar,LL).




/* Complementary predicate for delete_dup1*/

check_exist(LL,[ASP1-_|_]):-  check_exist1(LL,ASP1),!.
check_exist(LL,[_-_|T1]):-
	check_exist(LL,T1).



/* Complementary of check_exist ( if this ASP has all of our lists (in our lists of lists) ) */

check_exist1([],_).
check_exist1([L|T],ASP1):- 

	check_exist2(L,ASP1),

	check_exist1(T,ASP1).
	



check_exist2(L,[_-L-_|_]):- !.
check_exist2(L,[_|T]):- check_exist2(L,T).








/* Deleting "duplicate" solutions */

delete_dup(L1,NewL1):- delete_dup1(L1,[],NewL1).

% if any of the lists in ASP exist in other ASP(BruteForce) => don't add this "node" to the new list 
delete_dup1([],SoFar,SoFar).
delete_dup1([ASP1-_|T],SoFar,NewL1):-

	create_listoflists(ASP1,LL),

	check_exist(LL,T),		% is there another ASP, with the same lists
	
	!,

	delete_dup1(T,SoFar,NewL1).


%if it doesn't exist in any of the other lists

delete_dup1([ASP1-ASA1|T],SoFar,NewL1):-

	append(SoFar,[ASP1-ASA1],NewSoFar),

	delete_dup1(T,NewSoFar,NewL1).








between(X1,X2,X1):- X1 =< X2.
between(X1,X2,X):- X1 < X2, NewX1 is X1+1, between(NewX1,X2,X).


/* Updating ASP */ 
replace([_-_-_|T], 1, X-Y-Z, [X-Y-Z|T]).
replace( [X-Y-Z|T], I, X1-Y1-Z1, [X-Y-Z|T1]):- 

	I > 1, 
	NewI is I-1,
	replace(T, NewI,X1-Y1-Z1,T1).


/* Instantiating ASP */
instantiate(_,0,[]).
instantiate(Counter, NP, [Counter-[]-0|T]):-

	NP > 0,
	NP1 is NP-1,
	Counter1 is Counter + 1,
	instantiate(Counter1, NP1,T).


/* Instantiating ASA */
instantiate1( [], []).
instantiate1( [X|L], [X-_|L1]):- instantiate1(L,L1). 



	
% List,SearchValue,ReplaceValue, newList ==> update ASA

search_and_Replace([X1-_|L1], X1, Y1, [X1-Y1|L1] ).
search_and_Replace([X-Y|L],  X1, Y1, [X-Y|L2] ):- search_and_Replace(L,X1,Y1,L2). 




% Check if activity is after the last one assigned(+1) 
afterLast([],_).
afterLast([X|L],S):- 

	activity(X, act(_,E)),

	S >= E+1, 

	afterLast(L,S).








assignment(NP,ST,ASP,ASA):-

	findall(X,activity(X,_),L),

	instantiate(1,NP,ASP_temp),

	instantiate1(L,ASA_temp),

	!,


	%assignment1(L,NP,ST, ASP_temp, ASA_temp, ASP, ASA).


	%findall(ASP1-ASA1, ( assignment1(L,NP,ST, ASP_temp, ASA_temp, ASP1, ASA1, L1),    ), L1),

	findall(ASP1-ASA1,assignment1(L,NP,ST, ASP_temp, ASA_temp, ASP1, ASA1), L1),

	%setof(ASP1-ASA1,assignment1(L,NP,ST, ASP_temp, ASA_temp, ASP1, ASA1), L1),

	/* Check if we have duplicates list in ASP */ 

	delete_dup(L1,NewL1),

	member(ASP-ASA,NewL1).

	




assignment1( [], _, _,ASP_temp, ASA_temp, ASP_temp, ASA_temp).
assignment1( [X|L], NP, ST, ASP_temp , ASA_temp, ASP, ASA):-

	activity(X,act(S,E)),

	between(1,NP,Num),

	member(Num-Sofar-T, ASP_temp),

	afterLast(Sofar,S),

	K is E-S,
	T1 is T + K,
	T1 =< ST,

	% update ASP_temp

	replace(ASP_temp,Num, Num-[X|Sofar]-T1, New_ASP_temp),

	% update ASA_temp	

	search_and_Replace(ASA_temp, X, Num, New_ASA_temp),	

	assignment1(L, NP, ST, New_ASP_temp, New_ASA_temp, ASP, ASA).	

		