var mongoose =require('mongoose');
var locationSchema = new  mongoose.Schema({
	lat:String,
	lon:String,
    alt:String,
    speed:String,
    date:Date
});
mongoose.model('Location',locationSchema);

module.exports =mongoose.model('Location');
