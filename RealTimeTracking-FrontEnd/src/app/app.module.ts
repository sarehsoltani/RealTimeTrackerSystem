import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { HttpClientModule } from '@angular/common/http';
import { AppComponent } from './app.component';
import { AgmCoreModule, GoogleMapsAPIWrapper } from '@agm/core';
import { UiSwitchModule } from 'ngx-toggle-switch';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { ToastrModule } from 'ngx-toastr';

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule,
    AgmCoreModule.forRoot({
      apiKey: 'AIzaSyBTrMqMkNVTpfUkZ1r_YqNRV1S1SWfTjXY'+ '&libraries=visualization'
    }),
    UiSwitchModule,
    BrowserAnimationsModule,
    ToastrModule.forRoot({
      maxOpened:1,
      autoDismiss:true,
      timeOut:5000,
      positionClass: 'toast-top-center',
    }
    ),
  ],
  providers: [GoogleMapsAPIWrapper],
  bootstrap: [AppComponent]
})
export class AppModule { }
