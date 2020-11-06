#define PERIOD 8388608
#define RATE 128
#define SSI_RATE 2048
#define bitbang_quad(a,b) PORTB = B00000000 + a + (b << 1)
#define bitbang_data(a) PORTB = B00000000 + a + (!a << 1)
#define QUD_SAW 1
#define QUD_SIN 2
#define SSI_SAW 3
#define SSI_SIN 4

int runMode;
int ssiData;
bool* binData;
int ssiCtr = 0;


void ssi_send() {
  bitbang_data(*binData);
  binData++;
  ssiCtr++;
  if (ssiCtr == 8*sizeof(int)) {
    ssiCtr = 0;
    binData = reinterpret_cast<bool*>(&ssiData);
  }
}

void quad_saw() {
    long int ind = 0;
      int state = 0;
      int dir = 0;
      while(1) {
        ind = ind % PERIOD;
        if (ind == 0)
          dir = !dir;
        if (ind % RATE == 0) {
          switch(dir)
          {
            case 0: {
              switch(state) {
                case 0: {
                  bitbang_quad(0,0);
                  state = 1;
                  break;
                }
                case 1: {
                  bitbang_quad(1,0);
                  state = 2;
                  break;
                }
                case 2: {
                  bitbang_quad(1,1);
                  state = 3;
                  break;
                }
                case 3: {
                  bitbang_quad(0,1);
                  state = 0;
                  break;
                }
              }
              break;
            }
            case 1: {
              switch(state) {
                case 0: {
                  bitbang_quad(0,0);
                  state = 1;
                  break;
                }
                case 1: {
                  bitbang_quad(0,1);
                  state = 2;
                  break;
                }
                case 2: {
                  bitbang_quad(1,1);
                  state = 3;
                  break;
                }
                case 3: {
                  bitbang_quad(1,0);
                  state = 0;
                  break;
                }
              }
              break;
            }
          }
        } 
        ind++;             
      }
}

void ssi_saw() {
  while(1) {
    ssiData = (millis() % SSI_RATE) - (SSI_RATE / 2); 
  }
}

void setup() { 
  
  runMode = SSI_SAW;
  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(8, OUTPUT);  
  pinMode(9, OUTPUT);  

  if (runMode == SSI_SAW || runMode == SSI_SIN) {
    attachInterrupt(digitalPinToInterrupt(2), ssi_send, RISING);
    //  attachInterrupt(digitalPinToInterrupt(3), increase_duty, FALLING);    
    binData = reinterpret_cast<bool*>(&ssiData);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(runMode) {
    case QUD_SAW: {
      quad_saw();
      break;
    }
    case QUD_SIN: {
      break;
    }
    case SSI_SAW: {
      ssi_saw();
      break;
    }
    case SSI_SIN: {
      break;
    }
   
  }
  
}
