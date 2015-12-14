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


    
    var style_file = 'styles/pointCloudBuildings.yaml';

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


var camera, 
    tick = 0,
    scene, 
    renderer, 
    clock = new THREE.Clock(true),
    controls, 
    container, //gui = new dat.GUI(),
    options, 
    spawnerOptions, 
    particleSystem;

    init();
    animate();

    function init() {


      container = document.createElement('div');
      document.body.appendChild(container);

      camera = new THREE.PerspectiveCamera(28, window.innerWidth / window.innerHeight, 1, 10000);
      camera.position.z = 100;

      scene = new THREE.Scene();

      // The GPU Particle system extends THREE.Object3D, and so you can use it
      // as you would any other scene graph component.  Particle positions will be
      // relative to the position of the particle system, but you will probably only need one
      // system for your whole scene
      particleSystem = new THREE.GPUParticleSystem({
        maxParticles: 250000
      });
      scene.add( particleSystem);


      // options passed during each spawned
      options = {
        position: new THREE.Vector3(),
        positionRandomness: .3,
        velocity: new THREE.Vector3(),
        velocityRandomness: .5,
        color: 0xaa88ff,
        colorRandomness: .2,
        turbulence: .5,
        lifetime: 2,
        size: 5,
        sizeRandomness: 1
      };

      spawnerOptions = {
        spawnRate: 15000,
        horizontalSpeed: 1.5,
        verticalSpeed: 1.33,
        timeScale: 1
      }

      renderer = new THREE.WebGLRenderer();
      renderer.setPixelRatio(window.devicePixelRatio);
      renderer.setSize(window.innerWidth, window.innerHeight);
      container.appendChild(renderer.domElement);

      // setup controls
      controls = new THREE.TrackballControls(camera, renderer.domElement);
      controls.rotateSpeed = 5.0;
      controls.zoomSpeed = 2.2;
      controls.panSpeed = 1;
      controls.dynamicDampingFactor = 0.3;


    }

    function animate() {

      requestAnimationFrame(animate);

      controls.update();

      var delta = clock.getDelta() * spawnerOptions.timeScale;
      tick += delta;

      if (tick < 0) tick = 0;

      if (delta > 0) {
        options.position.x = Math.sin(tick * spawnerOptions.horizontalSpeed) * 20;
        options.position.y = Math.sin(tick * spawnerOptions.verticalSpeed) * 10;
        options.position.z = Math.sin(tick * spawnerOptions.horizontalSpeed + spawnerOptions.verticalSpeed) * 5;

        for (var x = 0; x < spawnerOptions.spawnRate * delta; x++) {
          // Yep, that's really it.  Spawning particles is super cheap, and once you spawn them, the rest of
          // their lifecycle is handled entirely on the GPU, driven by a time uniform updated below
          particleSystem.spawnParticle(options);
        }
      }

      particleSystem.update(tick);

      render();

    }

    function render() {

      renderer.render(scene, camera);

    }


}());
