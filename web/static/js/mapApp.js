//Original source: https://github.com/twtrubiks/leaflet-tutorials-interesting



var tiles = L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
    maxZoom: 18,
    minZoom: 2,
    attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
});
var latlng = L.latLng(51.5072, 0.1276);
var map = L.map('map', {
    center: latlng,
    zoom: 8,
    zoomControl: false,
    fullscreenControl: true,
    fullscreenControlOptions: { // optional
        title: "Show me the fullscreen !",
        titleCancel: "Exit fullscreen mode"
    },
    layers: [tiles]
});
var markersClusters = L.markerClusterGroup();
var markerList = [];
var controlSearch = new L.Control.Search({
    position: 'topright',
    layer: markersClusters,
    initial: false,
    zoom: 18,
    marker: false
});

DoMapUpdate(); //Initial call so we don't have to wait 5 seconds, then timer:
var DoMapUpdateTimer=self.setInterval(DoMapUpdate, 5000);
//window.clearInterval(DoMapUpdateTimer) //To clear if needed later ..

function DoMapUpdate() {

    $.ajax({
        url: '/mapinjectapi',
        type: 'POST',
        dataType: "json",
        success: function (map_data) {

            if (markerList.length > 0) {
                markerList = []; //Reset makerList
                markersClusters.clearLayers();
            }

            //console.log(`DoMapUpdate called: ${JSON.stringify(map_data)}`)


            for (var i = 0; i < map_data.length; i++) {
                //console.log(`map_data[${i}]: { Name: ${map_data[i].Name} Lat: ${map_data[i].Latitude} Lon: ${map_data[i].Longitude} Time: ${map_data[i].Time}}`);


                var marker = L.marker(new L.LatLng(map_data[i].Latitude, map_data[i].Longitude), {
                    title: map_data[i].Name,
                    // icon: selfIcon
                }).setBouncingOptions({
                    bounceHeight: 20,
                    exclusive: true
                }).on('click', function () {
                    this.bounce(3);
                }).addTo(markersClusters);

                var content = map_data[i].Name + "</br>" + "Latitude:" + map_data[i].Latitude + "</br>" + "Longitude:" + map_data[i].Longitude + "<br>" + "Time:" + map_data[i].DataTime + "<br>";
                marker.bindPopup(content, {
                    maxWidth: 600
                });
                
                markersClusters.removeLayer(marker);
                markersClusters.addLayer(marker);
                markerList.push(marker);

            }
            controlSearch.on('search:locationfound', function (e) {
                if (e.layer._popup) {
                    var index = markerList.map(function (e) {
                        return e.options.title;
                    }).indexOf(e.text);
                    var m = markerList[index];
                    markersClusters.zoomToShowLayer(m, function () {
                        m.openPopup();
                        m.bounce(3);
                    });
                }
            });
            map.addControl(controlSearch);
            map.addLayer(markersClusters);
            //mini map
            // lc = L.control.locate({
            //     position: 'topright',
            //     strings: {
            //         title: "Show me where I am, yo!",
            //         popup: "i am here"
            //     },
            //     drawCircle: true,
            //     showPopup: true
            // }).addTo(map);

            // zoom position
            L.control.zoom({
                position: 'topright'
            }).addTo(map);
        }
    });


}