var myMap;
ymaps.ready(init);
function init ()
{
	myMap = new ymaps.Map("map",
		{
			center:[53.6729, 55.957],
			zoom:17,
			behaviors: ["scrollZoom"],
			type: "yandex#map",
			controls: ["zoomControl","typeSelector", "fullscreenControl"]
		});
}
