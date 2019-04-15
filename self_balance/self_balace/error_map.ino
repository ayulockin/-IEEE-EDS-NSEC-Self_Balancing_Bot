void error_map(){
  calculate_angle();

//  if(angle_x<0){
//    error = -20;
//  }
//  else {
//    error = 20;
//  }
  
  if(angle_x<0.0){
    if(angle_x<0 and angle_x>-5){
      error = -0.5;
    }
    else if(angle_x<=-5 and angle_x>-10){
      error = -1.0;
    }
    else if(angle_x<=-10 and angle_x>-15){
      error = -1.5;
    }
    else if(angle_x<=-15 and angle_x>-20){
      error = -2.0;
    }
    else if(angle_x<=-20 and angle_x>-25){
      error = -2.5;
    }
    else if(angle_x<=-25 and angle_x>-30){
      error = -3.0;
    }
    else if(angle_x<=-30 and angle_x>-35){
      error = -3.5;
    }
    else if(angle_x<=-35 and angle_x>-40){
      error = -4.0;
    }
    else if(angle_x<=-40 and angle_x>-45){
      error = -4.5;
    }
    else if(angle_x<=-45){
      error = -5.0;
    }
  }
  else if(angle_x>0.0){ 
    if(angle_x>0 and angle_x<5){
      error = 0.5;
    }
    else if(angle_x>=5 and angle_x<10){
      error = 1.0;
    }
    else if(angle_x>=10 and angle_x<15){
      error = 1.5;
    }
    else if(angle_x>=15 and angle_x<20){
      error = 2.0;
    }
    else if(angle_x>=20 and angle_x<25){
      error = 2.5;
    }
    else if(angle_x>=25 and angle_x<30){
      error = 3.0;
    }
    else if(angle_x>=30 and angle_x<35){
      error = 3.5;
    }
    else if(angle_x>=35 and angle_x<40){
      error = 4.0;
    }
    else if(angle_x>=40 and angle_x<45){
      error = 4.5;
    }
    else if(angle_x>=45){
      error = 5.0;
    }
  }
  else if(angle_x==0.0){
    error = 0.0;
  }

  Serial.println(error);
  
}
