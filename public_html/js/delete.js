function delClicked() {
	var thediv = document.getElementById("add-del");
	thediv.innerHTML = "";
	if (document.getElementById("switch-button").innerHTML === "Switch to Movies")
		actorDel(thediv);
	else
		movieDel(thediv);
}

function actorDel(thediv) {
	thediv.innerHTML = '<form id="actor-del" action="php/delActor.php" method="get"><label for="actid-del">Actor ID:</label><input type="number" name="actid" id="actid-del"><br /><input type="submit" name="submit" value="Delete Actor"></form>';
}
function movieDel(thediv) {
	thediv.innerHTML = '<form id="movie-add" action="php/delMovie.php" method="get"><label for="movname-input">Movie ID:</label><input type="number" name="movid" id="movname-input"><br /><input type="submit" name="submit" value="Delete Movie"></form>';
}
