var file = require("file");
var fs = require("fs");
var async = require("async");

function async(fileName){
var total = 0
var open = 0
async.parallel([function(callback) {file.walk(fileName, function(err, dirPath, dirs, files){
	files.forEach(function(file){
		var count = 0
		open += 1
		fs.createReadStream(file).on('data', function(chunk) {
                                        count += chunk.toString('utf8')
                                        .split(/\r\n|[\n\r\u0085\u2028\u2029]/g)
                                        .length-1;
                                }).on('end', function() {
                                        console.log(file, count);
					total += count
					open -= 1
					if (open == 0)
						callback(0)
                                       
                                }).on('error', function(err) {
                                       console.error(err);
               	 		});
	})
})
}], function(err, result){
	console.log(total)
})}

function sync(fileName){
	var total = 0
	var open = 0
	file.walkSync(fileName, function(dirPath, dirs, files){
		files.forEach(function(f){
			var count = 0
			open += 1
			fs.createReadStream(file.path.join(dirPath, f)).on('data', function(chunk) {
                                        count += chunk.toString('utf8')
                                        .split(/\r\n|[\n\r\u0085\u2028\u2029]/g)
                                        .length-1;
                                }).on('end', function() {
                                        console.log(f, count);
                                        total += count
					open -= 1
					if (open == 0){
						console.log(total)
					}

                                }).on('error', function(err) {
                                       console.error(err);
                                });
		})
	})
}

sync("./PAM08")
