//API access tocken
mapboxgl.accessToken = 'pk.eyJ1Ijoiamt4enZiIiwiYSI6ImNrd2M5NXplOTA3d2EydXF2N3Nwd3Y0M2oifQ.Ne_ZJObv7Vpi2Wh2c1kFpQ';

setupMap([25.483300 ,42.725000]) //Bulgaria's default coordinates

function setupMap(center){ //setting up default map
    const map = new mapboxgl.Map({
        container: 'map',
        style: 'mapbox://styles/mapbox/streets-v11',
        center: center,
        zoom: 7
    });

    const nav = new mapboxgl.NavigationControl()
    map.addControl(nav); // adds navigation controlls
}

