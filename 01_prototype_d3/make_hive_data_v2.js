//D3 DOES NOT SUPPORT NODE


// var http = require('http');
// var net = require('net');
// var url = require('url');

// var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
// var xhr = new XMLHttpRequest();

// var d3 = require("d3"),
//     jsdom = require("jsdom");

// var document = jsdom.jsdom(),
//     svg = d3.select(document.body).append("svg");

var sample = "Bee";

var d3Data = {};
var gData = null;
d3.csv("ALL_HIVE_DATA_preliminary_hive_analysis.csv", function(data) {
      console.log(data)
      gData = data;
      
	d3Data.name = sample + " " + data[0].Location;
	d3Data.children = [];

	data.forEach(function(row) {
		if (row.Location == data[0].Location) {
			var hierarchy = row.ID.split("|");
			if (hierarchy.length == 1) { //if greater than 1 there is a pipe |
				var child = {
					name: hierarchy[0],
					children: [],
					size: parseFloat(row[sample])
				};
				d3Data.children.push(child);
			// } else if (hierarchy.length == 2) {
			// 	for (var i = 0; i < d3Data.children.length; i++) {
			// 		var parent = d3Data.children[i];
			// 		if (parent.name == hierarchy[0]) {
			// 			var child = {
			// 				name: hierarchy[1],
			// 				children: [],
			// 				size: parseFloat(row.Honey),
			// 			};
			// 			parent.children.push(child);
			// 		}
			// 	}
			// }
			} else {
				add_child(d3Data, hierarchy, row);
			}
		}
	});
	document.getElementById('json').innerHTML = JSON.stringify(d3Data);
});

// works for hierarchy.length >= 2
// "k__Bacteria|p__Actinobacteria|c__Actinobacteria"

//root is recursive part: ex. first root is Honey- Astoria
//second root would be k__Bacteria

function add_child(root, hierarchy, row) {

	for (var i = 0; i < root.children.length; i++) {
		var parent = root.children[i];
		if (parent.name == hierarchy[0]) {
			var size = parseFloat(row[sample]);
			// if (size > 0) { //keeping 0's to have color uniformity in viz
				if (hierarchy.length == 2) {
					var child = {
						name: hierarchy[1],
						children: [],
						size: size,
					};
					parent.children.push(child);
				} else {
					add_child(parent, hierarchy.slice(1), row);
				}
			// }
			break;
		}
	}
}






