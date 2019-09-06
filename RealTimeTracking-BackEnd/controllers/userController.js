const express = require('express');
const fs = require('fs');
const path = require('path');
var locationModel = require('../models/Location');
var router = express.Router();
router.post("/mapCenter",(req,res)=>{
  var center = {
    lat:35.710377,
    lon:51.430542
  };
  return res.status(200).json(center);
});
router.post("/mapData",(req,res)=>{
  var lastDate =new Date('January 1, 1970 00:00:00');;
  if(req.body.lastDate)
    {
      var userlastDate = Date.parse(req.body.lastDate);
      if(!isNaN(userlastDate))
        lastDate = userlastDate;
    }
    locationModel.find({date:{$gt: lastDate}}).sort({date:1}).exec(function(error,locations){
      if(error)
        return res.sendStatus(500);
      return res.status(200).json(locations);
    }
    );
});
module.exports = router;
