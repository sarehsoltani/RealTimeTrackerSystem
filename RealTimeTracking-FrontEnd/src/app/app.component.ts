import { Component, OnInit } from '@angular/core';
import { DataApiService } from './services/data-api/data-api.service'
import { ToastrService } from 'ngx-toastr';

import * as moment from 'moment-timezone';
declare const google: any;

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
})
export class AppComponent {
  title: string = 'سامانه ردیابی اشیاء';
  mapCenterLat: number;
  mapCenterLng: number;
  mapData:any[];
  showLocation:boolean;
  heatMapData:any[];
  enableSwitch:boolean;
  googleMapsLoaded:boolean;
  private map: google.maps.Map = null;
  private heatmap: google.maps.visualization.HeatmapLayer = null;
  constructor(private dataApi:DataApiService,private toastr: ToastrService){
    this.toastr.warning("Loading Google Maps ...")
  }
  ngOnInit(){
    this.enableSwitch=false;
    this.googleMapsLoaded=false;
    this.showLocation= true;
    this.heatMapData=[];
    this.mapData=[];
    setInterval(()=>{this.updateData()}, 60000);
    this.dataApi.getMapCenter().subscribe(
      (res)=>{
        this.mapCenterLat = res.lat;
        this.mapCenterLng = res.lon
      },
      (err)=>{
        console.log("err")
      }
    );
    this.getMapData(null);

  }
  updateData(){
    if(!this.mapData.length)
      return;
    var lastDate = this.mapData[this.mapData.length-1].date;
    this.getMapData(lastDate);
  }
  getMapData(date){
    this.dataApi.getMapData(date).subscribe(
      (res)=>{
        for(let i=0;i<res.length;i++){
            res[i].date = moment(res[i].date).tz('Asia/Tehran').format("YYYY/MM/DD-hh:mm:ss");
            this.mapData.push(res[i])
            if(this.googleMapsLoaded){
              this.heatMapData.push(new google.maps.LatLng(res[i].lat,res[i].lon));
              this.heatmap.setData(this.heatMapData)
            }
        }
      },
      (err)=>{
        console.log("err")
      }
    );
  }
  onMapLoad(mapInstance: google.maps.Map) {
      this.map = mapInstance;
      this.toastr.success("Maps Loaded.");
      this.googleMapsLoaded=true;
      for(let i=0;i<this.mapData.length;i++){
            this.heatMapData.push(new google.maps.LatLng(this.mapData[i].lat,this.mapData[i].lon));
            // this.heatmap.setData(this.heatMapData)
      }
    this.heatmap = new google.maps.visualization.HeatmapLayer({
        data: this.heatMapData
    });
  }
  toggleLocation(){
    if(this.showLocation)
      this.heatmap.setMap(this.map)
    else
      this.heatmap.setMap(null)
  }
}
