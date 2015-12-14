window.requestAnimFrame = (function() {
    return  window.requestAnimationFrame ||
            window.webkitRequestAnimationFrame ||
            window.mozRequestAnimationFrame ||
            window.oRequestAnimationFrame ||
            window.msRequestAnimationFrame ||
            function(/* function FrameRequestCallback */ callback, /* DOMElement Element */ element) {
                return window.setTimeout(callback, 1000/60);
         };
})();

var map = (function () {
    'use strict';

    //Defaut to Crown Heights Location:
    //654 Park Place, Brooklyn, NY
    //40.6738101, -73.9584195
    // 40.70531887544228, -74.00976419448853
    var map_start_location = [40.70531887544228,-74.00976419448853 , 16];


    
    var style_file = 'styles/pattern.yaml';

    /*** Map ***/
    var map = L.map('map', {
        maxZoom: 20,
        trackResize: true,
        keyboard: false
});

var layer = Tangram.leafletLayer({
    scene: style_file,

});

window.layer = layer;
var scene = layer.scene;
window.scene = scene;

map.setView(map_start_location.slice(0, 2), map_start_location[2]);
var hash = new L.Hash(map);

// Resize map to window
function resizeMap() {
    document.getElementById('map').style.width = window.innerWidth + 'px';
    document.getElementById('map').style.height = window.innerHeight + 'px';
    map.invalidateSize(false);
}

window.addEventListener('resize', resizeMap);
resizeMap();

window.addEventListener('load', function () {
    // Scene initialized
    layer.addTo(map);
    renderCanvas();
});

//disable mousewheel zoom if iframed
if (window.self !== window.top) {
  map.scrollWheelZoom.disable();
}
return map;
}());