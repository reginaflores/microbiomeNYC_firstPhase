var links = document.querySelectorAll( 'a[rel=external]' );
for( var j = 0; j < links.length; j++ ) {
	var a = links[ j ];
	a.addEventListener( 'click', function( e ) {
		window.open( this.href, '_blank' );
		e.preventDefault();
	}, false );
}

var loader,
	map,
	camera, 
	scene, 
	renderer,
	sphere,
	uniforms,
	attributes,
	container,
	mesh,
	marker,
	torus;
	
var container = document.getElementById( 'container' );
var preloader = document.getElementById( 'preloader' );
var bar = document.getElementById( 'bar' );
var options = document.getElementById( 'options' );
var title = document.getElementById( 'title' );

var fov = 90,
texture_placeholder,
isUserInteracting = false,
onMouseDownMouseX = 0, onMouseDownMouseY = 0,
lon = 0, onMouseDownLon = 0,
lat = 0, onMouseDownLat = 0,
phi = 0, theta = 0;
lat = 15;

init();
animate();

function setProgress( progress ) {
	bar.style.width = ( preloader.clientWidth - 6 ) * progress / 100 + 'px';
}

function showProgress( show ) {
	preloader.style.opacity = ( show == true )?1:0;
	preloader.style.display = ( show == true )?'block':'none';
}

function geoSuccess( position ) {

var currentLocation = new google.maps.LatLng( position.coords.latitude, position.coords.longitude );
	map.panTo( currentLocation );
	addMarker( currentLocation ); // move to position (thanks @theCole!)

}

function geoError( message ) {
	showError( message );
}

function showError( message ) {
	errorDiv.innerHTML = message;
}

function showMessage( message ) {
	showError('');
	messageDiv.innerHTML = message;
}

function init() {

	container = document.getElementById( 'container' );

	var locations = [
		{ lat: 51.50700703827454, lng: -0.12791916931155356 },
		{ lat: 32.6144404, lng: -108.9852017 },
		{ lat: 39.36382677360614, lng: 8.431220278759724 },
		{ lat: 59.30571937680209, lng: 4.879402148657164 },
		{ lat: 28.240385123352873, lng: -16.629988706884774 },
		{ lat: 50.09072314148827, lng: 14.393133454556278 },
		{ lat: 41.413416092316275, lng: 2.1531126527786455 },
		{ lat: 35.69143938066447, lng: 139.695139627539 },
		{ lat: 35.67120372775569, lng: 139.77167914398797 },
		{ lat: 54.552083679428065, lng: -3.297380963134742 }
	];
	
	var pos;
	if( window.location.hash ) {
		parts = window.location.hash.substr( 1 ).split( ',' );
		pos = { lat: parts[ 0 ], lng: parts[ 1 ] };
	} else {
		pos = locations[ Math.floor( Math.random() * locations.length ) ];
	}
	var myLatlng = new google.maps.LatLng( pos.lat, pos.lng );
	
	var myOptions = {
		zoom: 14,
		center: myLatlng,
		mapTypeId: google.maps.MapTypeId.ROADMAP,
		streetViewControl: false
	}
	map = new google.maps.Map( document.getElementById( 'map' ), myOptions );
	google.maps.event.addListener(map, 'click', function(event) {
		addMarker(event.latLng);
	});
	
	geocoder = new google.maps.Geocoder();
	
	scene = new THREE.Scene();

	camera = new THREE.PerspectiveCamera( fov, window.innerWidth / window.innerHeight, 1, 1100 );
	camera.target = new THREE.Vector3( 0, 0, 0 );
	scene.add( camera );

	var shadowMaterial = new THREE.MeshBasicMaterial( { emissive: 0xffffff, color: 0xffffff, map: THREE.ImageUtils.loadTexture( 'shadow.png' ), transparent: true } );
	var shadowGeo = new THREE.PlaneGeometry( 300, 300, 1, 1 );
	plane = new THREE.Mesh( shadowGeo, shadowMaterial );
	plane.position.y = - 150;
	plane.rotation.x = - 90 * Math.PI / 180;
	scene.add( plane );

	mesh = new THREE.Mesh( new THREE.SphereGeometry( 500, 60, 40 ), new THREE.MeshBasicMaterial( { map: THREE.ImageUtils.loadTexture( 'placeholder.jpg' ) } ) );
	mesh.scale.x = -1;
	scene.add( mesh );				
	
	renderer = new THREE.WebGLRenderer( { antialias: true } );
	renderer.setSize( window.innerWidth, window.innerHeight );
	renderer.sortObjects = false

	container.appendChild( renderer.domElement );
	
	var shader = new THREE.ShaderMaterial( {
		uniforms: {
			texture: { type: 't', value: null },
			scaledTexture: { type: 't', value: null },
			rAmount: { type: 'f', value: 0.0 }
		},
		vertexShader: document.getElementById( 'vertexShader' ).textContent,
		fragmentShader: document.getElementById( 'fragmentShader' ).textContent
	} );
	
	uniforms = THREE.UniformsUtils.clone( shader.uniforms );
	var material = new THREE.ShaderMaterial( {
		fragmentShader: shader.fragmentShader,
		vertexShader: shader.vertexShader,
		uniforms: uniforms
	} );
	
	// torus = new THREE.Mesh( new THREE.TorusKnotGeometry( 30, 10, 200, 50, 1, 3 ), material );
	//using torus variable for sphere
	// torus = new THREE.Mesh( new THREE.SphereGeometry( 500, 60, 60 ), material );
	// torus.scale.x = -1;
	// torus.doubleSided = true;
	
	// scene.add( torus );

	// var sphere = new THREE.Mesh( new THREE.SphereGeometry( 500, 60, 60 ), material );
	// sphere.scale.x = -1;
	// sphere.doubleSided = true;
	// scene.add( sphere );
	torus = new THREE.Mesh( new THREE.IcosahedronGeometry( 30, 5 ), material );
	scene.add( torus );
	// scene.add( torus );	
	// scene.add(shader);

	container.addEventListener( 'mousedown', onMouseDown, false );
	container.addEventListener( 'mousemove', onMouseMove, false );
	container.addEventListener( 'mouseup', onMouseUp, false );
	container.addEventListener( 'mousewheel', onMouseWheel, false );
	container.addEventListener( 'DOMMouseScroll', onMouseWheel, false);
	window.addEventListener( 'resize', onWindowResized, false );

	var reflectButton = document.getElementById( 'reflectButton' );
	if( reflectButton ) {
		reflectButton.addEventListener( 'click', function( e ) {
			uniforms[ 'rAmount' ].value = 0.0;
			reflectButton.className = 'button active';
			refractButton.className = 'button';
			e.preventDefault();
		} );
	}
	
	var refractButton = document.getElementById( 'refractButton' );
	if( refractButton ) {
		refractButton.addEventListener( 'click', function( e ) {
			uniforms[ 'rAmount' ].value = 0.8;
			reflectButton.className = 'button';
			refractButton.className = 'button active';
			e.preventDefault();
		} );
	}
	
	onWindowResized( null );

	loadPanorama( myLatlng );
		
	var el = document.getElementById( 'myLocationButton' );
	el.addEventListener( 'click', function( event ) {
		event.preventDefault();
		navigator.geolocation.getCurrentPosition( geoSuccess, geoError );
	}, false );
	
	navigator.pointer = navigator.pointer || navigator.webkitPointer;  
	
	function lockPointer () {
		if( navigator.pointer ) {
			navigator.pointer.lock( container, function() {  
				console.log( 'Pointer locked' ); 
			}, function() { 
				console.log( 'No pointer lock' ); 
			} );  
		}
	}
	
	var el = document.getElementById( 'fullscreenButton' );
	if( el ) {
		el.addEventListener( 'click', function( e ) {
			container.onwebkitfullscreenchange = function(e) {
				lockPointer();
				container.onwebkitfullscreenchange = function() {
				};
			};
			container.onmozfullscreenchange = function(e) {
				lockPointer();
				container.onmozfullscreenchange = function() {
				};
			};
			if( container.webkitRequestFullScreen ) container.webkitRequestFullScreen();
			if( container.mozRequestFullScreen ) container.mozRequestFullScreen();
			e.preventDefault();
		}, false );
	}
	
	el = document.getElementById( 'searchButton' );
	el.addEventListener( 'click', function( event ) {
		event.preventDefault();
		findAddress( document.getElementById("address").value );
	}, false );
	
	errorDiv = document.getElementById( 'error' );
	messageDiv = document.getElementById( 'message' );
}

function findAddress( address ) {

	showMessage( 'Getting coordinates...' );
	geocoder.geocode( { 'address': address}, function(results, status) {
		if (status == google.maps.GeocoderStatus.OK) {
			map.setCenter(results[0].geometry.location);
			showMessage( 'Address found.' );
			addMarker( results[0].geometry.location );
		} else {
			showError("Geocode was not successful for the following reason: " + status);
			showProgress( false );
		}
	});
}

function addMarker(location) {
	if( marker ) marker.setMap( null );
	marker = new google.maps.Marker({
		position: location,
		map: map
	});
	marker.setMap( map ); 
	loadPanorama( location );
}

function loadPanorama( location ) {

	setProgress( 0 );
	showProgress( true );
	
	loader = new GSVPANO.PanoLoader( {
		useWebGL: false,
		zoom: 3
	} );
	loader.onSizeChange = function() { 
		
	};
	loader.onProgress = function( p ) {
		setProgress( p );
	};
	loader.onError = function( message ) {
		showError( message );
		showProgress( false );
	};
	loader.onPanoramaLoad = function() {
		
		window.location.hash = location.lat() + ',' + location.lng();
	
		var source = this.canvas[ 0 ];
		mesh.material.map = new THREE.Texture( source ); 
		mesh.material.map.needsUpdate = true;
		
		var canvas = document.createElement( 'canvas' );
		var s = 2;
		canvas.width = source.width / s;
		canvas.height = source.height / s;
		var ctx = canvas.getContext( '2d' );
		ctx.drawImage( source, 0, 0, source.width, source.height, 0, 0, canvas.width, canvas.height );
		
		uniforms[ 'texture' ].value = new THREE.Texture( source );
		uniforms[ 'texture' ].value.needsUpdate = true;
		uniforms[ 'scaledTexture' ].value = new THREE.Texture( canvas );
		uniforms[ 'scaledTexture' ].value.needsUpdate = true;
		
		showMessage( 'Street view data ' + this.copyright + '.' );
		
		showProgress( false );
	};

	loader.load( location );
	
}

function onWindowResized( event ) {
	renderer.setSize( container.clientWidth, container.clientHeight );
	camera.projectionMatrix.makePerspective( fov, container.clientWidth / container.clientHeight, camera.near, camera.far );
}

function onMouseDown( event ) {

	title.style.opacity = 0;
	title.style.pointerEvents = 'none';
	options.style.opacity = 0;
	options.style.pointerEvents = 'none';
					
	event.preventDefault();

	isUserInteracting = true;

	onPointerDownPointerX = event.clientX;
	onPointerDownPointerY = event.clientY;

	onPointerDownLon = lon;
	onPointerDownLat = lat;

}

function onMouseMove( event ) {

	if ( isUserInteracting ) {

		lon = ( event.clientX - onPointerDownPointerX ) * 0.1 + onPointerDownLon;
		lat = ( event.clientY - onPointerDownPointerY ) * 0.1 + onPointerDownLat;

	}
	
}

function onMouseUp( event ) {

	title.style.opacity = 1;
	title.style.pointerEvents = 'auto';
	options.style.opacity = 1;
	options.style.pointerEvents = 'auto';
	
	isUserInteracting = false;
	
}

function onMouseWheel( event ) {

	// WebKit

	if ( event.wheelDeltaY ) {

		fov -= event.wheelDeltaY * 0.05;

	// Opera / Explorer 9

	} else if ( event.wheelDelta ) {

		fov -= event.wheelDelta * 0.05;

	// Firefox

	} else if ( event.detail ) {

		fov += event.detail * 1.0;

	}

	camera.projectionMatrix.makePerspective( fov, container.clientWidth / container.clientHeight, camera.near, camera.far );
	
}

function animate() {

	requestAnimationFrame( animate );
	render();

}

function render() {
	
	if( !isUserInteracting ) {
		torus.rotation.x += .01;
		lon += .15;
	}
	lat = Math.max( - 85, Math.min( 85, lat ) );
	phi = ( 90 - lat ) * Math.PI / 180;
	theta = lon * Math.PI / 180;

	camera.position.x = 100 * Math.sin( phi ) * Math.cos( theta );
	camera.position.y = 100 * Math.cos( phi );
	camera.position.z = 100 * Math.sin( phi ) * Math.sin( theta );
	
	camera.lookAt( camera.target );
	
	renderer.render( scene, camera );

}