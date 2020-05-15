Alexander Vilesov
Email: asvilesov@gmail.com

Project: IMDB Rest Server

Description: This program sets up a REST server which loads an IMDB database of movies. It will return JSON messages of information about movies. 
One can test with postman...

Instructions:
0) Download 'title.basics.tsv.gz' from 'https://www.imdb.com/interfaces/' and place it into the directory outside of this folder and name it 'data.tsv'.
1) cd bin
2) cmake ..
3) make
4) cd ../bin
5) ./main 

Example:
One can receive a response from the server if asking for a movie ID at:
1) http://localhost:12345/movie/id/tt0076759
{
 "ID": "tt0076759",
 "Runtime": 121,
 "Title": "Star Wars: A New Hope",
 "URL": "https://www.imdb.com/title/tt0076759/",
 "Year": 1977
}

or 
2) http://localhost:12345/movie/name/Top%20Gun
{
 "ID": "tt0048734",
 "Runtime": 73,
 "Title": "Top Gun",
 "URL": "https://www.imdb.com/title/tt0048734/",
 "Year": 1955
 },
 {
 "ID": "tt0092099",
 "Runtime": 110,
 "Title": "Top Gun",
 "URL": "https://www.imdb.com/title/tt0092099/",
 "Year": 1986
 }
