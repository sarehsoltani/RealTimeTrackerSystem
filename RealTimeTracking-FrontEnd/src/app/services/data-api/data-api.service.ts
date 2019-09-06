import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class DataApiService {
  prefix:string = "/api/user"
  constructor(private http: HttpClient) {

  }
  getMapCenter()
    {
      return this.http.post<any>(this.prefix+"/mapCenter",null);
  }
  getMapData(date)
    {
        return this.http.post<any>(this.prefix+"/mapData",{lastDate:date});
  }
}
