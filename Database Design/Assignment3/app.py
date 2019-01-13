# ----- CONFIGURE YOUR EDITOR TO USE 4 SPACES PER TAB ----- #
import pymysql as db
import settings
import sys

def connection():
    ''' User this function to create your connections '''
    con = db.connect(
        settings.mysql_host, 
        settings.mysql_user, 
        settings.mysql_passwd, 
        settings.mysql_schema,
        charset="utf8")
    
    return con

def updateRank(rank1, rank2, movieTitle):

    # Create a new connection
    con=connection()
    
    # Create a cursor on the connection
    cur=con.cursor()
    
    try:
        r1 = float(rank1)

        if r1 < 0 or r1 > 10:                   # check if r1 is out of bounds
            return [("status",),("error",),]
    except ValueError:
        return [("status",),("error",),]
    
        
    try:
        r2 = float(rank2)

        if r2 < 0  or r2 > 10:                  #check if r2 is out of bounds
            return [("status",),("error",),]

    except ValueError:
        return [("status",),("error",),]

    sql = """   SELECT m.movie_id,m.rank
                FROM movie m
                WHERE m.title = %s"""

    parameters = (movieTitle) 
    
    cur.execute(sql,parameters)            #find the movies

    result = cur.fetchall()

    if cur.rowcount != 1 :         #if there is no movie or 2 movies with same name
        return [("status",),("error",),]

    for row in result:  # since we will have only one row each time, (it passed the above if )

        movie_id = row[0]
        r3 = row[1]
        
        if r3 is None:
            try:
                
                avg = float((r1+r2)/2)

                sql_1 = "UPDATE movie m SET m.rank = %s WHERE m.movie_id = %s "
                parameters_1 = ( str(avg) , str(movie_id) )
                

                cur.execute( sql_1, parameters_1 ) 

                con.commit()

                
                
            except:                                                         #if something goes wrong rollback
                print("Error1")

                con.rollback()
                return [("status",),("Error1",),]
        else:
            try:

                
                avg1 = float((r1+r2+r3)/3)

                sql_1 = "UPDATE movie m SET m.rank = %s WHERE m.movie_id = %s"
                
                parameters_1 = ( str(avg1), str(movie_id) )

                cur.execute(sql_1 , parameters_1 )

                con.commit()


            except:                                                         #if something goes wrong rollback

                print("Error2")
                      
                con.rollback()
                      
                return [("status",),("Error2",),]
        

    print (rank1, rank2, movieTitle)
    
    return [("status",),("ok",),]






def colleaguesOfColleagues(actorId1, actorId2):

    # Create a new connection
    con=connection()
    
    # Create a cursor on the connection
    cur=con.cursor()

    sql = """   SELECT m.title,a1.actor_id as actor_c, a2.actor_id as actor_d , %s as actor_a, %s as actor_b
    
                FROM    actor a1, actor a2,
                        role r1, role r2,
                        movie m
		
        
                WHERE a1.actor_id IN(			# έχει παίξει σε ταινία με τον actor_a

                    SELECT a1.actor_id

                    FROM    actor a1 , actor a2,
                            role r1, role r2,
                            movie m

                    WHERE   a1.actor_id = r1.actor_id and r1.movie_id = m.movie_id
                            and a2.actor_id = %s and r2.actor_id = a2.actor_id   and r2.movie_id = m.movie_id 
        
                            and a1.actor_id != a2.actor_id		# actor_c != actor_a
        
                    GROUP BY a1.actor_id)

                and a2.actor_id IN(

                    SELECT a1.actor_id

                    FROM    actor a1, actor a2,
                            role r1, role r2,
                            movie m
        
                    WHERE   a1.actor_id = r1.actor_id and r1.movie_id = m.movie_id
                            and a2.actor_id = %s and a2.actor_id = r2.actor_id and r2.movie_id = m.movie_id
        
                            and a1.actor_id != a2.actor_id		# actor_d != actor_b

                    GROUP BY a1.actor_id)

                and a1.actor_id = r1.actor_id and r1.movie_id = m.movie_id
		and a2.actor_id = r2.actor_id and r2.movie_id = m.movie_id 
        
                and a1.actor_id != a2.actor_id			# actor_c != actor_d
        
                GROUP BY m.title, a1.actor_id, a2.actor_id;    """

    parameters = ( str(actorId1), str(actorId2), str(actorId1), str(actorId2) )

    cur.execute( sql , parameters)

    results = cur.fetchall()

    list = [("movieTitle", "colleagueOfActor1", "colleagueOfActor2", "actor1","actor2",),]

    for i in results:
        list.append(i)
        
  
    print (actorId1, actorId2)
    print (list)
    return list

def actorPairs(actorId):

    # Create a new connection
    con=connection()
    
    # Create a cursor on the connection
    cur=con.cursor()

    # αποκλείουμε τους ηθοποιούς που μπορεί να παίζουν ταινίες ίδιου είδους με τον άλλο ηθοποιό ActorId

    sql ="""    SELECT a1.actor_id

                FROM actor a1, role r1, role r2, movie m1, movie m2, movie_has_genre mhg1, movie_has_genre mhg2, genre g1, genre g2

                WHERE a1.actor_id = r1.actor_id and r1.movie_id = m1.movie_id  and m1.movie_id = mhg1.movie_id and mhg1.genre_id = g1.genre_id


                and  m1.movie_id NOT IN(				    # αποκλείουμε τους ηθοποιούς που μπορεί να παίζουν ταινίες ίδιου είδους με τον άλλο ηθοποιό ActorId

                    SELECT m.movie_id
                        
                    FROM movie m, movie_has_genre mhg, genre g
                        
                    WHERE m.movie_id = mhg.movie_id and mhg.genre_id = g.genre_id
        
                            and g.genre_id IN(
			
					SELECT g1.genre_id
                                                
					FROM genre g1, role r, movie m, movie_has_genre mhg
                                                
					WHERE r.actor_id = %s and r.movie_id = m.movie_id and  m.movie_id = mhg.movie_id and  mhg.genre_id = g1.genre_id
					GROUP BY g1.genre_id
                            
                                        )
                    GROUP BY m.movie_id
                )

                and r2.actor_id = %s and r2.movie_id = m2.movie_id and m2.movie_id = mhg2.movie_id and mhg2.genre_id = g2.genre_id

                GROUP BY a1.actor_id

                HAVING count(distinct g1.genre_id) + count(distinct g2.genre_id) >= 7;   """

    parameters = ( str(actorId) , str(actorId) )

    cur.execute( sql, parameters)

    results = cur.fetchall()

    list = [("actor2Id",),]

    for i in results:
        list.append(i)

    


    	
    print (actorId)
    print (list)

    return (list)
    
	
def selectTopNactors(n):

    # Create a new connection
    con=connection()
    
    # Create a cursor on the connection
    cur=con.cursor()

    
    sql_1 = " SELECT distinct g.genre_id, g.genre_name FROM genre g GROUP BY g.genre_id; "

    cur.execute(sql_1)

    genres = cur.fetchall()

    list = [("genreName", "actorId", "numberOfMovies"),]

    for row in genres :

        genre_id = row[0]
        genre_name = row[1]

        sql_2 = """SELECT %s as genre_name , a.actor_id , count(m.movie_id) as Number_of_Movies

                    FROM actor a, role r, movie m , movie_has_genre mhg

                    WHERE a.actor_id = r.actor_id and r.movie_id = m.movie_id and m.movie_id = mhg.movie_id and mhg.genre_id = %s

                    GROUP BY a.actor_id

                    ORDER BY count(m.movie_id) DESC

                    LIMIT %s ; """

        parameters = ( str(genre_name) , str(genre_id), int(n) )

        cur.execute(sql_2, parameters)

        results = cur.fetchall()

        for i in results :

            list.append(i)

        

    print (n)
    print (list)

    return list

#res = updateRank(7,4,'Batman Begins')

#res = colleaguesOfColleagues(26409, 18534)

#res = actorPairs(3226)

#res = selectTopNactors(10)



