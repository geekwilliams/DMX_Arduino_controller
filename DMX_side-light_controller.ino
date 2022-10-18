/* Version 0.2
  Powered by Arduino Uno:
  DMX controll software for LUXX auditorium side lighting

  Caleb Williams
  Technical Specialist
  (307)462-6046 

  Copyright © Movie Palace Inc. 2022
*/

#include <SoftPWM.h>
#include <SoftPWM_timer.h>
#include <Conceptinetics.h>
////////////////////////////
// MP Stup
int max_level = 230;             // running at 90% for LED longevity
int min_level = 0;              
int channel_level = max_level;   // level on startup
float up_transition_time = 5000;
float down_transition_time = 5000;
////////////////////////////
int down_sig_pin = 12;
int up_sig_pin = 11;
bool is_active = false;


DMX_Master dmx_master( 1, 2 ); 
void setup() {

  dmx_master.enable();
  // start at 100%
  dmx_master.setChannelValue(1, channel_level);

  // LED_BUILTIN will simulate levels sent on DMX line
  SoftPWMBegin();
  SoftPWMSet(13, 255);

  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);

}

void loop() {

  bool up_sig = digitalRead(up_sig_pin);
  bool down_sig = digitalRead(down_sig_pin);

  if (!up_sig && !is_active){
    lights_up();
  }
  else if (!down_sig && !is_active){
    lights_down();
  }


}

void lights_up(){
  is_active = true;
  float start_time = millis();
  float elapsed_time = 0; 
  float end_time = start_time + up_transition_time;
  if (channel_level < max_level){
    while(elapsed_time < up_transition_time){
      elapsed_time = millis() - start_time;
      float perc = elapsed_time / up_transition_time;
      channel_level = round(max_level * perc);
      SoftPWMSet(13, channel_level);
      dmx_master.setChannelValue(1, channel_level);
    }
  }
  is_active = false;
}

void lights_down(){
  is_active = true; 
  float start_time = millis();
  float elapsed_time = 0; 
  float end_time = start_time + down_transition_time;
  if (channel_level > min_level){
    while(elapsed_time < down_transition_time){
      elapsed_time = millis() - start_time;
      float perc = elapsed_time / down_transition_time;
      channel_level = round(max_level * (1 - perc));
      SoftPWMSet(13, channel_level);
      dmx_master.setChannelValue(1, channel_level);
    }
  }
  is_active = false;
}