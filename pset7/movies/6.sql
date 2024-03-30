SELECT AVG(rating) FROM ratings 
INNER JOIN movies ON movie_id = movies.id
WHERE movies.year = 2012; 