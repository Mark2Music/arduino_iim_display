


#include <ros.h>
#include <std_msgs/Int16.h>
#include <geometry_msgs/Twist.h>
#include <Adafruit_NeoPixel.h>
#include <SCoop.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN_1 2
#define PIN_2 3
#define PIN_3 4
#define PIN_4 5
#define PIN_5 6

//IIM LOGO
Adafruit_NeoPixel strip_1 = Adafruit_NeoPixel(90, PIN_1, NEO_GRB + NEO_KHZ800);
//sound wave
Adafruit_NeoPixel strip_2 = Adafruit_NeoPixel(55, PIN_2, NEO_GRB + NEO_KHZ800);
//random shape,the robot don't speak, just a half of lightness, when the robot speak, full lightness
Adafruit_NeoPixel strip_3 = Adafruit_NeoPixel(16, PIN_3, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strip_4 = Adafruit_NeoPixel(50, PIN_4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_5 = Adafruit_NeoPixel(50, PIN_5, NEO_GRB + NEO_KHZ800);

int g_robot_speaking_status = 0;

//int map_a[10][5] = {
//		{0,35,36,71,72},
//		{1,34,37,70,73},
//		{2,33,38,69,74},
//		{3,32,39,68,75},
//		{4,31,40,67,76},
//		{5,30,41,66,77},
//		{6,29,42,65,78},
//		{7,28,43,64,79},
//		{8,27,44,63,80},
//		{9,26,45,62,81}
//};
//int map_b[8][5] = {
//		{10,25,46,61,82},
//		{11,24,47,60,83},
//		{12,23,48,59,84},
//		{13,22,49,58,85},
//		{14,21,50,57,86},
//		{15,20,51,56,87},
//		{16,19,52,55,88},
//		{17,18,53,54,89}
//};

int idx_thread1_g = 0;

// map for area a
int map_a_g[10][5] = {
		{72,71,36,35,0},
		{73,70,37,34,1},
		{74,69,38,33,2},
		{75,68,39,32,3},
		{76,67,40,31,4},
		{77,66,41,30,5},
		{78,65,42,29,6},
		{79,64,43,28,7},
		{80,63,44,27,8},
		{81,62,45,26,9}
};

// map for area b
int map_b_g[8][5] = {
		{82,61,46,25,10},
		{83,60,47,24,11},
		{84,59,48,23,12},
		{85,58,49,22,13},
		{86,57,50,21,14},
		{87,56,51,20,15},
		{88,55,52,19,16},
		{89,54,53,18,17}
};

// light map in the local region
int lignt_map_g[5][5] = {
		{1,1,1,1,1},
		{1,1,1,1,1},
		{1,1,1,1,1},
		{1,1,1,0,1},
		{1,1,1,0,1}
};

// default color map
int color_map_g[5][5] = {
		{2,2,1,1,1},
		{2,2,1,1,1},
		{2,2,1,1,1},
		{2,2,1,0,1},
		{2,2,1,0,1}
};

int color_kinds[3] = {
		0, 0xb22222, 0x0000ee //black, red, blue 
};

// global variable for the local index map
int local_map_g[5][5];

int LoG_rows_num_g = 5;

// reset the color map to the default
void reset_color_map()
{
	for (int row=0;row<LoG_rows_num_g;row++)
	{
		for (int col=0;col<5;col++)
		{
			color_map_g[row][col]=lignt_map_g[row][col];
		}
	}
}

// set the color map based on the column and color index
void set_light_color_map_by_column (int col, int color_idx)
{
	for (int row=0;row<LoG_rows_num_g;row++)
		color_map_g[row][col] = lignt_map_g[row][col]*color_idx;
}

// get the local map from area a
void get_local_map_area_a(int row_idx)
{
	for (int row = 0;row<LoG_rows_num_g;row++)
	{
		for(int col=0;col<5;col++)
		{
			local_map_g[row][col]=map_a_g[row+row_idx][col];
		}
	}
}

// get the local light index from area b
void get_local_map_area_b(int row_idx)
{
	for (int row = 0;row<LoG_rows_num_g;row++)
	{
		for(int col=0;col<5;col++)
		{
			local_map_g[row][col]=map_b_g[row+row_idx][col];
		}
	}
}

// light
void light(int color,int location_idx)
{
	strip_1.setPixelColor(location_idx, strip_1.Color((color & 0xff0000)>>16, (color & 0x00ff00)>>8, color & 0x0000ff));
}

// light the area
void light_area()
{
	for (int row = 0;row<LoG_rows_num_g;row++)
	{
		for (int col = 0;col<5;col++)
		{
			int color_idx = color_map_g[row][col];
			int color     = color_kinds[color_idx];
			int location_idx = local_map_g [row][col];

			light(color,location_idx);

		}
	}
}


void turn_off_light_area()
{
	for (int row = 0;row<LoG_rows_num_g;row++)
	{
		for (int col = 0;col<5;col++)
		{
			int color_idx = 0;
			int color     = color_kinds[color_idx];
			int location_idx = local_map_g [row][col];

			light(color,location_idx);

		}
	}
}

//完整定义
defineTask(TaskTest1);
void TaskTest1::setup()
{
  strip_1.begin();
  strip_1.show(); // Initialize all pixels to 'off'
};
void TaskTest1::loop()
{
  theaterChaseRainbow_1(1500);
  
};

//完整定义
defineTask(TaskTest2);
void TaskTest2::setup()
{
  strip_2.begin();
  //always light some leds, blue
    
  strip_2.show(); // Initialize some leds
}
void TaskTest2::loop()
{
  theaterChaseRainbow_2(50);
}

//完整定义
defineTask(TaskTest3);
void TaskTest3::setup()
{
  strip_3.begin();
  strip_3.show(); // Initialize all pixels to 'off'
}
void TaskTest3::loop()
{
  theaterChaseRainbow_3(80);
}

//完整定义
defineTask(TaskTest4);
void TaskTest4::setup()
{
  strip_4.begin();
  strip_4.show(); // Initialize all pixels to 'off'
}
void TaskTest4::loop()
{
  theaterChaseRainbow_4(50);
}

//完整定义
defineTask(TaskTest5);
void TaskTest5::setup()
{
  strip_5.begin();
  strip_5.show(); // Initialize all pixels to 'off'
}
void TaskTest5::loop()
{
  theaterChaseRainbow_5(50);
}


void messageCb( const std_msgs::Int16& info)
{
  if(info.data == 1) {
    g_robot_speaking_status = 1;
  }
  else if(info.data == 0) {
    g_robot_speaking_status = 0;
  }
  
}

ros::NodeHandle nh;
ros::Subscriber<std_msgs::Int16> sub("robot_speaking/status", &messageCb);

uint16_t param[4][5] = {
                        0, 35, 36, 71, 72,
                        9, 26, 45, 62, 81,
                        
                        10, 25, 46, 61, 82,
                        17, 18, 53, 54, 89
                      };

void setup() {
  nh.initNode();
  nh.subscribe(sub);
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  Serial.begin(57600);
  //task2 initialize leds
  //7-9
  for (uint16_t i=7; i < 10; i=i+1) {
    strip_2.setPixelColor(i, strip_2.Color(0, 0, 255));    //turn every third pixel on
  }
  //14-16
  for (uint16_t i=14; i < 17; i=i+1) {
    strip_2.setPixelColor(i, strip_2.Color(0, 0, 255));    //turn every third pixel on
  }
  //38-40
  for (uint16_t i=33; i < 36; i=i+1) {
    strip_2.setPixelColor(i, strip_2.Color(0, 0, 255));    //turn every third pixel on
  }
  //45-47
  for (uint16_t i=50; i < 53; i=i+1) {
    strip_2.setPixelColor(i, strip_2.Color(0, 0, 255));    //turn every third pixel on
  }
  mySCoop.start();
}

void loop()
{
  nh.spinOnce();
  delay(100);
  yield();
}


//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow_1(uint32_t wait) {
   
  // for the case in area a
	if (idx_thread1_g<=10-LoG_rows_num_g)
	{
		int row_idx = idx_thread1_g; // started row number in the location
//		int color_idx = 2;// set the color
//		int column_color_reset =1;// column that need to reset the color
//		reset_color_map();
//		set_light_color_map_by_column (column_color_reset, color_idx);
		get_local_map_area_a(row_idx);
		light_area();
	}
	else if (idx_thread1_g == 10-LoG_rows_num_g+1) // for the case that arrive the middle
	{
		idx_thread1_g = 10;

	}
	if (idx_thread1_g>=10 && idx_thread1_g<=18-LoG_rows_num_g) // for the case that arrive the bottom
	{
		int row_idx = idx_thread1_g-10;

//		int color_idx = 2;// set the color
//		int column_color_reset =1;// column that need to reset the color
//		reset_color_map();
//		set_light_color_map_by_column (column_color_reset, color_idx);
		get_local_map_area_b(row_idx);
		light_area();

	}
    strip_1.show();
	sleep(wait);
	turn_off_light_area();
	idx_thread1_g++;
	if (idx_thread1_g == 18-LoG_rows_num_g+1)
		idx_thread1_g =0;
  
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow_2(uint32_t wait) {
 

	 //if(g_robot_speaking_status ==  1){
  //  for (uint16_t i=0; i < 7; i=i+1) {
  //          strip_2.setPixelColor(i, strip_2.Color(0, 0, 255));    //turn every third pixel on
   //       }
   // strip_2.show();      
  //}
  
  for (int j=0; j < 1; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 1; q++) {
      if (g_robot_speaking_status == 0) {
        //0-6
        for (uint16_t i=0; i < 7; i=i+1) {
          strip_2.setPixelColor(i+q, 0);    //turn every third pixel on
        }
        //17-23
        for (uint16_t i=17; i < 24; i=i+1) {
          strip_2.setPixelColor(i+q, 0);    //turn every third pixel on
        }
        for (uint16_t i=24; i < 31; i=i+1) {
          strip_2.setPixelColor(i+q, 0);    //turn every third pixel on
        }
        //24-30
        for (uint16_t i=36; i < 43; i=i+1) {
          strip_2.setPixelColor(i+q, 0);    //turn every third pixel on
        }
        //31-37
        for (uint16_t i=43; i < 50; i=i+1) {
          strip_2.setPixelColor(i+q, 0);    //turn every third pixel on
        }
        strip_2.show();
           
      }
      else if (g_robot_speaking_status ==  1){
        for (uint16_t m=0; m < 255; m++) {
          //0-6
          for (uint16_t i=0; i < 7; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          //17-23
          for (uint16_t i=17; i < 24; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          //24-30
          for (uint16_t i=24; i < 31; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          //31-37
          for (uint16_t i=36; i < 43; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          //48-54
          for (uint16_t i=43; i < 50; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          strip_2.show();
          //sleep(1);  
        }
        //some time display
        sleep(10);
        for (uint16_t m=255; m > 0; m--) {
          //0-6
          for (uint16_t i=0; i < 7; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          //17-23
          for (uint16_t i=17; i < 24; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          //24-30
          for (uint16_t i=24; i < 31; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          //31-37
          for (uint16_t i=36; i < 43; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          //48-54
          for (uint16_t i=43; i < 50; i=i+1) {
            strip_2.setPixelColor(i+q, strip_2.Color(0, 0, m));    //turn every third pixel on
          }
          strip_2.show();
          //sleep(1);  
        }
          
      }  
      
      //display the leds
      strip_2.show();
      //delay(wait);
//      sleep(wait);
//      //close all the leds
//      //for (uint16_t i=0; i < strip_2.numPixels(); i=i+1) {
//      //  strip_2.setPixelColor(i+q, 0);        //turn every third pixel off
//      //} 
//      for (uint16_t i=0; i < 7; i=i+1) {
//        strip_2.setPixelColor(i+q, strip_2.Color(0, 0, 0));    //turn every third pixel on
//      }
//      //17-23
//      for (uint16_t i=17; i < 24; i=i+1) {
//        strip_2.setPixelColor(i+q, strip_2.Color(0, 0, 0));    //turn every third pixel on
//      }
//      //24-30
//      for (uint16_t i=24; i < 31; i=i+1) {
//        strip_2.setPixelColor(i+q, strip_2.Color(0, 0, 0));    //turn every third pixel on
//      }
//      //31-37
//      for (uint16_t i=36; i < 43; i=i+1) {
//        strip_2.setPixelColor(i+q, strip_2.Color(0, 0, 0));    //turn every third pixel on
//      }
//      //48-54
//      for (uint16_t i=43; i < 50; i=i+1) {
//        strip_2.setPixelColor(i+q, strip_2.Color(0, 0, 0));    //turn every third pixel on
//      }      
    }
  }

  
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow_3(uint32_t wait) {
  for (int j=0; j < 1; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 2; q++) {
      for (uint16_t i=0; i < strip_3.numPixels(); i=i+2) {
        if (g_robot_speaking_status == 0) {
          strip_3.setPixelColor(i+q, strip_3.Color(0, 0, 127));    //unincrease the lightness
        }
        else if (g_robot_speaking_status ==  1){
          strip_3.setPixelColor(i+q, strip_3.Color(0, 0, 255));    //unincrease the lightness
        }      
      }
      strip_3.show();
      //delay(wait);
      sleep(wait);
      //close the leds
      for (uint16_t i=0; i < strip_3.numPixels(); i=i+2) {
        strip_3.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow_4(uint8_t wait) {
//  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip_4.numPixels(); i=i+2) {
//        strip_4.setPixelColor(i+q, Wheel_4( (i+j) % 255));    //turn every third pixel on
//      }
//      strip_4.show();
//
//      //delay(wait);
//      sleep(wait);
//
//      for (uint16_t i=0; i < strip_4.numPixels(); i=i+2) {
//        strip_4.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow_5(uint8_t wait) {
//  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip_5.numPixels(); i=i+2) {
//        strip_5.setPixelColor(i+q, Wheel_5( (i+j) % 255));    //turn every third pixel on
//      }
//      strip_5.show();
//
//      //delay(wait);
//      sleep(wait);
//
//      for (uint16_t i=0; i < strip_5.numPixels(); i=i+2) {
//        strip_5.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
}

uint32_t Wheel_1(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip_1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip_1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip_1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel_2(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip_2.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip_2.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip_2.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t Wheel_3(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip_3.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip_3.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip_3.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t Wheel_4(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip_4.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip_4.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip_4.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t Wheel_5(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip_5.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip_5.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip_5.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}