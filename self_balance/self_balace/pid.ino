void pid(){
  
  error_map();

  pd = Kp*error + Kd*(error - prev_error);
 
//  lms = bs + pd;
//  rms = bs - pd;

  if(pd>255){pd=250;}
  if(pd<-255){pd=-250;}

//  Serial.print(" | ");
//  Serial.print(lms);
//  Serial.print(" | ");
//  Serial.println(rms);  

//  if(error<0){
//    analogWrite(lm1,lms);
//    analogWrite(lm2,0);
//    analogWrite(rm1,rms);
//    analogWrite(rm2,0);
//  }
//  else if (error>0){
//    analogWrite(lm1,0);
//    analogWrite(lm2,lms);
//    analogWrite(rm1,0);
//    analogWrite(rm2,rms);
//
//  }

  if(error<0){
    analogWrite(lm1,pd);
    analogWrite(lm2,0);
    analogWrite(rm1,pd+30);
    analogWrite(rm2,0);
  }
  else if (error>0){
    analogWrite(lm1,0);
    analogWrite(lm2,pd);
    analogWrite(rm1,0);
    analogWrite(rm2,pd+30);
  }
 
  prev_error = error;

}
