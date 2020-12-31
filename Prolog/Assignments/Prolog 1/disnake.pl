
/* IMPORTANT: when pattern < width ==> the first char of pattern becomes 'x'  for some uknown reason */












/* pattern > width */
 
% L3 is result
createList( L1, [], [], L1).
createList([X|L1],[_|L2], [X|L3],Remain):- createList( L1,L2,L3,Remain).

/* pattern < width */

%if the remaining_space = pattern
createList1( [X|L1] , [Y|L2], Previous, Result, []):- 
	
	equal( [X|L1], [Y|L2]),

	createEqual( [×|L1], Result1),

	append( Previous, Result1, Result). %done for result list

createList1( [X|L1] , [Y|L2], Previous, Result, Remain):- 

	
	%if the pattern < remaining_space 

	less( [X|L1], [Y|L2]),
	
	createLess( [×|L1],[Y|L2], Result1, RemainSpace),

	append( Previous, Result1, R),

	createList1( [X|L1], RemainSpace, R, Result, Remain).

createList1( [X|L1] , [Y|L2], Previous, Result, Remain):- 
	
	%if the pattern > remaining_space 
 
	more( [X|L1], [Y|L2]),

	createMore( [×|L1],[Y|L2], Result1, Remain),

	append( Previous, Result1, Result).


%3 helpful functions for above
createEqual( [], []).
createEqual( [X|L1], [X|L2]):- createEqual( L1,L2).



createLess( [], L2, [], L2).
createLess( [X|L1], [_|L2], [X|L3], Remain):-

 	createLess( L1, L2, L3, Remain).


createMore( L1, [], [], L1). 
createMore( [X|L1], [_|L2], [X|L3], Remain):-

	createMore( L1, L2, L3, Remain).


/*///////////////////////////////////// */
/*///////////////////////////////////// */



restOfLine( [], L3, L3).
restOfLine( [_|L1], [_|L2], RestOfLine):- restOfLine(L1,L2,RestOfLine).


printList( []).
printList( [X|L] ):- write(X), printList(L).





/*///////////////////////////////////// */
/*///////////////////////////////////// */




% reverse a list
reverse1( [], R, R).
reverse1( [H|L], Sofar, R):- reverse1(L,[H|Sofar], R).

/*///////////////////////////////////// */
/*///////////////////////////////////// */


%length_pattern = length_width
equal([],[]).
equal([_|L1],[_|L2]):- equal(L1,L2).

%length_pattern < length_width
less([],[_|_]).
less([_|L1],[_|L2]):- less(L1,L2).


%length_pattern > length_width
more([_|_],[]).
more([_|L1],[_|L2]):- more(L1,L2).

/*///////////////////////////////////// */
/*///////////////////////////////////// */


% 3 different "decide"
decide(Pattern, Width, Height):-  
	equal(Pattern,Width),
 	disnake1(Pattern,Height).

decide(Pattern, Width, Height):- 
	less(Pattern,Width),
 	disnake3(Pattern,Width, Height,[]).

decide(Pattern, Width, Height):- 
	more(Pattern,Width),
 	disnake2(Pattern,Width, Height,[]).

/*///////////////////////////////////// */
/*///////////////////////////////////// */

%3 different "disnake"

% pattern = width
disnake1( [_|_], []).

disnake1( [X|L1], [_|L3] ):-
   %We just print the pattern and then reverse it (recursively). 

	printList( [X|L1] ),

	nl,

	reverse1( [X|L1], [], R),

	disnake1(R, L3).


/*///////////////////////////////////// */
/*///////////////////////////////////// */

	
% pattern > width

disnake2( [_|_],  [_|_], [], [_|_]).
disnake2( [_|_],  [_|_], [], []).


disnake2( [X|L1], [Y|L2], [_|L3], Previous ):-

   % Check how many chars we have left, to fill our list
	restOfLine( Previous,[Y|L2], RestOfLine),

   %We first create the list 
	createList( [X|L1], RestOfLine, R, Remain),

   % Connect previous with R
	append( Previous, R, Result),

   % Print the Line
	printList( Result),

	% print new line
	nl,

% Now for the reverse 
%(L4 is the new line(_,_,_)list after we printed the reverse line

	disnake21( [X|L1], [Y|L2], L3, Remain).

/*///////////////////////////////////// */

%complementary of disnake2 (print reverse)
disnake21( [_|_], [_|_], [], [_|_]).
disnake21( [_|_],  [_|_], [], []).

disnake21( [X|L1], [Y|L2], [_|L3], Previous):-

   % Check how many chars we have left, to fill our list
	restOfLine( Previous,[Y|L2], RestOfLine),

   % We first create the list 
	createList( [X|L1], RestOfLine, R, Remain),

   % Connect previous with R
	append( Previous, R, Result),

   % Reverse our List
	reverse1( Result, [], R1),

   % Print the Line
	printList( R1),

	%print new line
	nl,
	
	disnake2( [X|L1], [Y|L2], L3, Remain).




/*///////////////////////////////////// */
/*///////////////////////////////////// */

	
% pattern < width

% may or may not have previous
disnake3( [_|_],  [_|_], [], [_|_]).	
disnake3( [_|_],  [_|_], [], []).

disnake3( [X|L1], [Y|L2], [_|L3], Previous ):-

	%write(X),

	% Check how many chars we have left, to fill our list
	restOfLine( Previous,[Y|L2], RestOfLine),

   	% First create the list
   	createList1( [X|L1], RestOfLine, [], R, Remain), 	% [] is previous1

  	 % Append previous with current list
	append( Previous, R, Result),

	% Print the Line
	printList( Result),
	
	%print new line
	nl,
	
	disnake31( [X|L1], [Y|L2], L3, Remain).




%complementary of disnake3 (print reverse)
disnake31( [_|_], [_|_], [], [_|_]).
disnake31( [_|_],  [_|_], [], []).

disnake31( [X|L1], [Y|L2], [_|L3], Previous):-

   % Check how many chars we have left, to fill our list
	restOfLine( Previous,[Y|L2], RestOfLine),

   % We first create the list 
	createList1( [X|L1], RestOfLine, [], R, Remain),

   % Connect previous with R
	append( Previous, R, Result),

   % Reverse our List
	reverse1( Result, [], R1),

   % Print the Line
	printList(R1),

	%print new line
	nl,
	
	disnake3( [X|L1], [Y|L2], L3, Remain).

   









/*///////////////////////////////////// */
/*///////////////////////////////////// */

disnake(Pattern, Width, Height):- decide(Pattern,Width,Height).
