function addClicked() {
	var thediv = document.getElementById("add-del");
	thediv.innerHTML = "";
	if (document.getElementById("switch-button").innerHTML === "Switch to Movies")
		actorAdd(thediv);
	else
		movieAdd(thediv);
}

function actorAdd(thediv) {
	thediv.innerHTML = '<form id="actor-add" action="php/addActor.php" onSubmit="return validateActor(this);" method="get"><label for="actname-input">Actors full name:</label><input type="text" id="actname-input" name="aname"><br/><input type="submit" name="submit" value="Add Actor"></form>';
}

function movieAdd(thediv) {
	thediv.innerHTML = '<center><form id="movie-add" action="php/addMovie.php" method="get"><label for="movname-input">Movie title:</label><br /><input type="text" name="movname" id="movname-input"><br /><label for="movactname-input">ID for Movies Leading Actor:</label><br /><input type="number" name="actid" id="movactname-input"><br /><label for="movgenre-input">Genre:</label><br /><input type="text" name="movgenre" id="movgenre-input"><br /><label for="movyear-input">Year of Release:</label><br /><input type="number" name="movyear" id="movyear-input"><br /><label for="movprice-input">Retail Rrice:</label><br /><input type="number" name="movprice" id="movprice-input"><br /><input type="submit" name="submit" value="Add Movie"></form></center>';
}
