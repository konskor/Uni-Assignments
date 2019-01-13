# --------------------------------------
# --------------------------------------
DROP PROCEDURE IF EXISTS ValidateQuery;
DELIMITER //
CREATE PROCEDURE ValidateQuery(IN qNum INT, IN queryTableName VARCHAR(255))
BEGIN
	DECLARE cname VARCHAR(64);
	DECLARE done INT DEFAULT FALSE;
	DECLARE cur CURSOR FOR SELECT c.column_name FROM information_schema.columns c WHERE 
c.table_schema='movies' AND c.table_name=queryTableName;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	# Add the column fingerprints into a tmp table
	DROP TABLE IF EXISTS cFps;
	CREATE TABLE cFps (
  	  `val` VARCHAR(50) NOT NULL
	) 
	ENGINE = InnoDB;

	OPEN cur;
	read_loop: LOOP
		FETCH cur INTO cname;
		IF done THEN
      			LEAVE read_loop;
    		END IF;
		
		DROP TABLE IF EXISTS ordered_column;
		SET @order_by_c = CONCAT('CREATE TABLE ordered_column as SELECT ', cname, ' FROM ', queryTableName, ' ORDER BY ', cname);
		PREPARE order_by_c_stmt FROM @order_by_c;
		EXECUTE order_by_c_stmt;
		
		SET @query = CONCAT('SELECT md5(group_concat(', cname, ', "")) FROM ordered_column INTO @cfp');
		PREPARE stmt FROM @query;
		EXECUTE stmt;

		INSERT INTO cFps values(@cfp);
		DROP TABLE IF EXISTS ordered_column;
	END LOOP;
	CLOSE cur;

	# Order fingerprints
	DROP TABLE IF EXISTS oCFps;
	SET @order_by = 'CREATE TABLE oCFps as SELECT val FROM cFps ORDER BY val'; 
	PREPARE order_by_stmt FROM @order_by;
	EXECUTE order_by_stmt;

	# Read the values of the result
	SET @q_yours = 'SELECT md5(group_concat(val, "")) FROM oCFps INTO @yours';
	PREPARE q_yours_stmt FROM @q_yours;
	EXECUTE q_yours_stmt;

	SET @q_fp = CONCAT('SELECT fp FROM fingerprints WHERE qnum=', qNum,' INTO @rfp');
	PREPARE q_fp_stmt FROM @q_fp;
	EXECUTE q_fp_stmt;

	SET @q_diagnosis = CONCAT('select IF(@rfp = @yours, "OK", "ERROR") into @diagnosis');
	PREPARE q_diagnosis_stmt FROM @q_diagnosis;
	EXECUTE q_diagnosis_stmt;

	INSERT INTO results values(qNum, @rfp, @yours, @diagnosis);

	DROP TABLE IF EXISTS cFps;
	DROP TABLE IF EXISTS oCFps;
END//
DELIMITER ;

# --------------------------------------

# Execute queries (Insert here your queries).

# Validate the queries
drop table if exists results;
CREATE TABLE results (
  `qnum` INTEGER  NOT NULL,
  `rfp` VARCHAR(50)  NOT NULL,
  `yours` VARCHAR(50)  NOT NULL,
  `diagnosis` VARCHAR(10)  NOT NULL
)
ENGINE = InnoDB;


# -------------
# Q1
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select m.title  

from actor a, role r, movie m, movie_has_genre mhg, genre g  

where 	a.actor_id = r.actor_id and r.movie_id = m.movie_id and m.movie_id = mhg.movie_id and mhg.genre_id = g.genre_id 
		and a.last_name = "Allen" and g.genre_name = "Comedy";

CALL ValidateQuery(1, 'q');
drop table if exists q;
# -------------


# -------------
# Q2
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select d.last_name, m.title

from director d, movie m, actor a, role r, movie_has_director mhd

where 	a.actor_id = r.actor_id and r.movie_id = m.movie_id and a.last_name = "Allen" # τις ταινίες που έχει παίξει ο Allen
		and mhd.movie_id = m.movie_id and mhd.director_id = d.director_id and         #και έχουν σκηνοθετηθεί απο κάποιους σκηνοθέτες
		
		d.director_id not in (	select d.director_id
								from  director d, movie_has_director mhd, movie m, movie_has_genre mhg, genre g  
								where 	d.director_id = mhd.director_id and mhd.movie_id = m.movie_id and
										m.movie_id = mhg.movie_id and mhg.genre_id = g.genre_id  
								group by director_id
								having count( mhg.genre_id) = 1);

CALL ValidateQuery(2, 'q');
drop table if exists q;
# -------------


# -------------
# Q3
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select a.last_name

from	actor a
		

where a.actor_id in(

select a.actor_id

from	actor a,
		role r,
		movie m,
		movie_has_director mhd,
		director d

where 	a.actor_id = r.actor_id and r.movie_id = m.movie_id and				#Ηθοποιοί που παίζουν SAME
		m.movie_id = mhd.movie_id and mhd.director_id = d.director_id

		and a.last_name = d.last_name

group by a.actor_id
)
and a.actor_id in( 

select a.actor_id

from 		actor a,
			role r1, role r2,
			movie m1, movie m2,
			movie_has_director mhd1, movie_has_director mhd2,
			director d1, director d2,
			movie_has_genre mhg1, movie_has_genre mhg2,
			genre g1, genre g2

where			d1.director_id =  mhd1.director_id and mhd1.movie_id = m1.movie_id  	#Έχει σκηνοθετήσει ο SAME
				and m1.movie_id = mhg1.movie_id and mhg1.genre_id = g1.genre_id

				and d2.director_id = mhd2.director_id and mhd2.movie_id = m2.movie_id	#Έχει σκηνοθετήσει ο DIFF
				and m2.movie_id = mhg2.movie_id and mhg2.genre_id = g2.genre_id

				and d1.last_name = a.last_name and d2.last_name <> a.last_name and
				
				a.actor_id = r1.actor_id and  a.actor_id = r2.actor_id 			# Έχει παίξει σε τανία DIFF και 
				and r1.movie_id <> m1.movie_id and r2.movie_id = m2. movie_id		# ΟΧΙ  SAME

				and  mhg1.genre_id = mhg2.genre_id									# Ίδιο είδος τανιών

group by a.actor_id);

CALL ValidateQuery(3, 'q');
drop table if exists q;
# -------------


# -------------
# Q4
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select 'yes' as answer

from movie
where exists(
			select m.movie_id
			from movie m, movie_has_genre mhg, genre g 
			where m.movie_id = mhg.movie_id and mhg.genre_id = g.genre_id
				and m.year = 1995 and g.genre_name = 'Drama')
						
union

select 'no' as answer
from movie
where not exists(
			select m.movie_id
			from movie m, movie_has_genre mhg, genre g 
			where m.movie_id = mhg.movie_id and mhg.genre_id = g.genre_id
				  and m.year = 1995 and g.genre_name = 'Drama');


CALL ValidateQuery(4, 'q');
drop table if exists q;
# -------------


# -------------
# Q5
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select d1.last_name as name1, d2.last_name as name2

from	director d1, director d2, 
		movie_has_director mhd1, movie_has_director mhd2, 
		movie_has_genre mhg, 
		genre g , 
		movie m

where	d1.director_id = mhd1.director_id and d2.director_id = mhd2.director_id 
		and mhd1.movie_id = mhd2.movie_id	# έχουν συνσκηνοθετήσει

		and mhd1.movie_id = m.movie_id and m.movie_id = mhg.movie_id and g.genre_id =mhg.genre_id

		and m.year <= 2006 and m.year >= 2000 and mhd1.director_id < mhd2.director_id

group by d1.last_name,d2.last_name

having count(distinct g.genre_id) >=6;


CALL ValidateQuery(5, 'q');
drop table if exists q;
# -------------


# -------------
# Q6
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.



select a.first_name, a.last_name, count(distinct mhd.director_id) as number_of_directors 

from actor a, role r, movie m, movie_has_director mhd, director d

where a.actor_id = r.actor_id and r.movie_id = m.movie_id and m.movie_id = mhd.movie_id and mhd.director_id = d.director_id
	  
and a.actor_id in( 

select a.actor_id

from actor a, role r, movie m

where a.actor_id = r.actor_id and r.movie_id = m.movie_id

group by a.actor_id

having count(r.movie_id ) = 3
)
 
group by a.actor_id;

CALL ValidateQuery( 6, 'q');
drop table if exists q;
# -------------


# -------------
# Q7
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.



select distinct  mhg.genre_id, count(d.director_id) as number_of_directors

from director d, movie m, movie_has_genre mhg, genre g

where m.movie_id = mhg.movie_id and mhg.genre_id = g.genre_id and

m.movie_id in(

select m.movie_id

from movie m, movie_has_genre mhg, genre g

where m.movie_id = mhg.movie_id and mhg.genre_id = g.genre_id 

group by m.movie_id

having count(mhg.genre_id) = 1
)
and d.director_id in(

select d.director_id

from director d, movie_has_director mhd, movie m, movie_has_genre mhg       

where	d.director_id = mhd.director_id and mhd.movie_id = m.movie_id		#οι σκηνοθέτες μας ασχολούνται με  ταινίες, που έχουν 1 απο τα genre που έχουμε βρει
		and mhg.movie_id =m.movie_id and mhg.genre_id = g.genre_id

group by director_id
)

group by m.movie_id;


CALL ValidateQuery(7, 'q');
drop table if exists q;
# -------------


# -------------
# Q8
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.



select a.actor_id

from actor a

where not exists(

select g.genre_id

from genre g

where not exists(

select m.movie_id

from movie m, movie_has_genre mhg, role r

where 	a.actor_id = r.actor_id and r.movie_id = m.movie_id
		and m.movie_id = mhg.movie_id and mhg.genre_id = g.genre_id
)
); 


CALL ValidateQuery(8, 'q');
drop table if exists q;
# -------------


# -------------
# Q9
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.



select  g1.genre_id as genre1_id, g2.genre_id as genre2_id, count(distinct d.director_id) as number_of_directors

from	genre g1, genre g2,
		movie_has_genre mhg1, movie_has_genre mhg2,
		movie m1, movie m2,
		movie_has_director mhd1, movie_has_director mhd2,
		director d 

where	d.director_id = mhd1.director_id and mhd1.director_id = mhd2.director_id and
		
		mhd1.movie_id = m1.movie_id and m1.movie_id = mhg1.movie_id and mhg1.genre_id = g1.genre_id and

		mhd2.movie_id = m2.movie_id and m2.movie_id = mhg2.movie_id and mhg2.genre_id = g2.genre_id and

		g1.genre_id < g2.genre_id

group by g1.genre_id, g2.genre_id;


CALL ValidateQuery(9, 'q');
drop table if exists q;
# -------------


# -------------
# Q10
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.


select  g.genre_id, a.actor_id, count(m.movie_id) as number_of_movies

from actor a , genre g, movie m, role r 

where a.actor_id = r.actor_id and r.movie_id = m.movie_id

and m.movie_id in(				#ταινίες που έχει παίξει ο ηθοποιός

select m.movie_id

from director d, movie_has_director mhd, movie m , movie_has_genre mhg

where d.director_id = mhd.director_id and mhd.movie_id = m.movie_id and m.movie_id = mhg.movie_id and mhg.genre_id = g.genre_id

	and d.director_id in(		# σκηνοθέτες που έχουν σκηνοθετήσει μόνο ένα είδος

		select d.director_id

			from director d, movie_has_director mhd, movie m, movie_has_genre mhg1, genre g 

				where d.director_id = mhd.director_id and mhd.movie_id = m.movie_id and m.movie_id = mhg1.movie_id and mhg1.genre_id = g.genre_id

					group by d.director_id
					having count( mhg1.genre_id) = 1
	)

	group by m.movie_id

)

group by g.genre_id, a.actor_id;


CALL ValidateQuery(10, 'q');
drop table if exists q;
# -------------

DROP PROCEDURE IF EXISTS RealValue;
DROP PROCEDURE IF EXISTS ValidateQuery;
DROP PROCEDURE IF EXISTS RunRealQueries;

select * from results;