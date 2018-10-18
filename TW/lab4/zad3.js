var async = require('async');

function printAsync(s, cb) {
   var delay = Math.floor((Math.random()*1000)+500);
   setTimeout(function() {
       console.log(s);
       if (cb) cb();
   }, delay);
}

function task1(cb) {
    printAsync("1", function() {
        task2(cb);
    });
}

function task2(cb) {
    printAsync("2", function() {
        task3(cb);
    });
}

function task3(cb) {
    printAsync("3", cb);
}

// wywolanie sekwencji zadan
//task1(function() {
//    console.log('done!');
//});

function loop(n){
	for (let i=0; i<n; i++){
	async.waterfall([
		function(){
			task1(function(){
				console.log('done!');
			})
		},
		task1,
		task2,
		task3
	], function(err, foo){ foo() });
	}
}

loop(4)

