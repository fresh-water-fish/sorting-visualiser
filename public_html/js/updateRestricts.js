function upRestrict(btn, classname) {
	// go through all the buttons in the div and redo select...
	alert('you pressed me!');
	var elems = document.getElementsByClassName(classname);
	console.log(elems);
	if (btn.value === 'on') {
		/* hide */
		console.log('attempting to hide elements');
		function applyChange(item){
			item.style.display = 'none';
			item.style.visibility = 'hidden';
		}
	} else {
		/* make visible */
		function applyChange(item) {
			item.style.display = 'block';
			item.style.visibility = 'visible';
		}
	}
	for (var i = 0; i < elems.length; i++)
		applyChange(elems.item(1));
}


