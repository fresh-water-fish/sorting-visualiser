// Initialise arrays of actors / movies 
// - async so that when it fetches the data tl can still be loaded
// - function should actually get the actors/moviesthe database
function Actor(id, name) {
	this.id = id;
	this.name = name;
}
function Movie(actId, movId, title, price, genre, year) {
	this.actId = actId;
	this.movId = movId;
	this.title = title;
	this.price = price;
	this.genre = genre;
	this.year = year;
}
var actors = ["Benjamin the III", "Henry Gordan","Jamie Maister", "Petru Bad"];
var movies = [];
