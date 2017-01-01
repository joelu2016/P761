#include "ProductDriverClientInterface.h"
#include "GinsengADC.h"
//#include "ADCConstants.h"

namespace driver {
  auto& AMP_RST = board::gpc6;
  auto& AMP_PWDN = board::gpc4;
  auto& AMP_FAULT = board::gpc7;

  //auto& P18V_EN_G = board::soft_power;
}



PRODUCT_DRIVER_START
private:
  static const uint16_t STA369_Address = 0x38;
  static const uint16_t STA369_Register_MasterVolume = 7;
  static const uint16_t STA369_Register_MonoVolume = 10;
 
  static const uint16_t kMuteVolume = 0xFF;
  static const uint16_t kMaxVolume = 0;

  static const uint16_t kBiquadNum = 8;
  static const uint16_t kCoefficientsPerBiquad = 5;
  static const uint16_t kRegsPerCoeffiecient = 3;
  static const uint16_t kBytesPerBiquad = (1 + (kCoefficientsPerBiquad * kRegsPerCoeffiecient));
  static const uint16_t kBiquadRegStart = 0x16;
  static const uint16_t kRWCtrlReg = 0x26;


  uint16_t saved_vol_ = kMuteVolume;
  uint16_t EQ[3] = {1, 2, 3};
  uint16_t timerForSec=0x00;
  uint16_t timesOnDet=0x00;
  

/*
  static const uint16_t EQ_Coeff_Table[8][16] = 
  {
      {0x00,0x80,0x1C,0x30,0x7F,0xE3,0xD0,0x7F,0xE3,0xA7,0x80,0x38,0x0C,0x3F,0xF1,0xE8}, 
      {0x05,0x94,0x1E,0x57,0x56,0x20,0x15,0x6B,0xE1,0xA9,0xA5,0x13,0x2B,0x42,0x66,0x60}, 
      {0x0A,0x80,0x25,0xD3,0x7F,0xD3,0xA9,0x7F,0xDA,0x2D,0x80,0x4B,0x4B,0x3F,0xF0,0x86}, 
      {0x0F,0xA3,0x3E,0x79,0x59,0x40,0x6D,0x7B,0xB5,0xF8,0x88,0x5A,0x72,0x30,0x3E,0x1E}, 
      {0x14,0x92,0x15,0x65,0x71,0x13,0xF4,0x6D,0xEA,0x9B,0x89,0x53,0x3D,0x42,0xCC,0x67}, 
      {0x19,0xB6,0x77,0x26,0x64,0xEE,0xFF,0x49,0x88,0xDA,0x8F,0xD2,0xD4,0x45,0x9F,0x16}, 
      {0x1E,0x80,0xF9,0x02,0x7E,0x47,0x40,0x7F,0x06,0xFE,0x81,0xEB,0x45,0x3F,0xE6,0xBD}, 
      {0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00}  
  };
*/
void WaitMs(std::size_t num_ms)
{
  SystemClock sys_clk;
  auto delay_timer = MakeSoftTimer(sys_clk);
  delay_timer.Start(1);
  for (std::size_t i = 0; i < num_ms; ++i) {
    for (std::size_t j = 0; j < SystemClock::SlicesPerMs; ++j) {
      delay_timer.Tick(kSysTicksPerMsSlice);
      delay_timer.Wait();
    }
  }
}


 uint16_t GetBatVoltage()
 {
   GinsengADC adcX1;
   return adcX1.Get();
 }
 

/*
//Read ADC
static uint16_t readADC(uint16_t adcNum)
{
  uint16_t configVal = adcConfig0 | adcNum | ADC_START_CONV_BIT;
  ADC_set_config(configVal);
  uint16_t dataVal = ADC_read();
  return dataVal & kADCReadDataMask;
  
}

*/

public:
  /*!
   *  Driver Bootup Routine.
   *  This is called whenever the system reboots
   *  (Cold Start or restart from PDP and Aux In)
   */
  void DriverBootup(void)
  {

    driver::AMP_FAULT.SetAsInput();
    driver::AMP_RST.SetAsOutput(true);
    driver::AMP_PWDN.SetAsOutput(false);
    xout << "P761-010217-B";

    WaitMs(200);

    //Initial the STA369
    static const uint16_t kRegisterNum = 82;
    uint16_t init_STA369[kRegisterNum][2]={
      {0x00, 0x63},
      {0x01, 0x80},
      {0x02, 0x97},
      {0x03, 0x50},
      {0x04, 0x02},
      {0x05, 0xDF},
      {0x06, 0x10},
      {0x07, 0x00},
      {0x08, 0x60},
      {0x09, 0x60},
      {0x0A, 0x5A},
      {0x0B, 0x80},
      {0x0C, 0x00},
      {0x0E, 0X00},
      {0x0F, 0x40},
      {0x10, 0x90},
      {0x11, 0x77},
      {0x12, 0x61},
      {0x13, 0x69},
      {0x14, 0x6A},
      {0x15, 0x69},
      {0x16, 0x00},
      {0x17, 0x00},
      {0x18, 0x00},
      {0x19, 0x00},
      {0x1A, 0x00},
      {0x1B, 0x00},
      {0x1C, 0x00},
      {0x1D, 0x00},
      {0x1E, 0x00},
      {0x1F, 0x00},
      {0x20, 0x00},
      {0x21, 0x00},
      {0x22, 0x00},
      {0x23, 0x00},
      {0x24, 0x00},
      {0x25, 0x00},
      {0x26, 0x00},
      {0x27, 0x1A},
      {0x28, 0xC0},
      {0x29, 0xF3},
      {0x2A, 0x33},
      {0x2B, 0x00},
      {0x2C, 0x0C},
      {0x2D, 0x7F}, 
      {0x31, 0x00},
      {0x32, 0xA9}, 
      {0x33, 0xB0},
      {0x34, 0x30}, 
      {0x35, 0x30},
      {0x36, 0x07},
      {0x37, 0x00},
      {0x38, 0x00},
      {0x39, 0x01},
      {0x3A, 0xEE},
      {0x3B, 0xFF},
      {0x3C, 0x7E},
      {0x3D, 0xC0},
      {0x3E, 0x26},
      {0x3F, 0x00},
      {0x46, 0xFF},
      {0x48, 0x00},
      {0x49, 0x00},
      {0x4A, 0x00},
      {0x4B, 0x04},
      {0x4C, 0x00},
      {0x4D, 0x32},
      {0x4E, 0x00},
      {0x4F, 0x5E},
      {0x60, 0x00},
      {0x61, 0x00},
      {0x62, 0x00},
      {0x63, 0x00},
      {0x64, 0x00},
      {0x65, 0x00},
      {0x66, 0x00},
      {0x67, 0x00},
      {0x68, 0x00},
      {0x69, 0x00},
      {0x6A, 0x00},
      {0x6B, 0x00},
      {0x6C, 0x44}
    };

    for (uint16_t i = 0; i < kRegisterNum; i++)//for (uint16_t i = 0; i < 1; i++)//
    {
      mi2c_.WriteRegister(STA369_Address, init_STA369[i][0], init_STA369[i][1]);
    }
      



    //EQ setting
    static const constexpr std::array<std::array<const uint16_t, kBytesPerBiquad>, kBiquadNum> eq_table =
    {{
      {{0x00, 0x80, 0x19, 0x8B, 0x7F, 0xE6, 0x75, 0x7F, 0xE6, 0x3F, 0x80, 0x32, 0xA8, 0x3F, 0xF3, 0x3A}},
      {{0x05, 0x80, 0xB4, 0x8B, 0x7E, 0x61, 0x39, 0x7F, 0x4B, 0x75, 0x81, 0x63, 0x7F, 0x40, 0x1D, 0xA4}},
      {{0x0A, 0x80, 0x6F, 0xA6, 0x7F, 0x13, 0x0A, 0x7F, 0x90, 0x5A, 0x80, 0xCB, 0x18, 0x40, 0x10, 0xEF}},
      {{0x0F, 0x85, 0x61, 0x3D, 0x78, 0x73, 0xD7, 0x7A, 0x9E, 0xC3, 0x89, 0x0C, 0x43, 0x3F, 0x3F, 0xF3}},
      {{0x14, 0x92, 0x26, 0xA2, 0x69, 0x48, 0xA4, 0x6D, 0xD9, 0x5E, 0x92, 0x33, 0x4E, 0x42, 0x42, 0x07}},
      {{0x19, 0x9B, 0xC0, 0x39, 0x70, 0x35, 0xEF, 0x64, 0x3F, 0xC7, 0x92, 0xED, 0x9A, 0x3E, 0x6E, 0x3B}},
      {{0x1E, 0x80, 0x29, 0x48, 0x7F, 0xCF, 0xC3, 0x7F, 0xD6, 0xB8, 0x80, 0x51, 0xE9, 0x3F, 0xEF, 0x2A}},
      {{0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00}}
    }};

    for (uint16_t i = 0; i < kBiquadNum; i++)
    {
      mi2c_.WriteRegister(STA369_Address, kBiquadRegStart, eq_table[i].data(), eq_table[i].size());

      //tell amp to update i2c values into ram
      mi2c_.WriteRegister(STA369_Address, kRWCtrlReg, 0x02);
    }
    
    static uint16_t array_set[3]={0x08,0x78,0xF1};
    //static const addingEQ[2]={0x08, 0x78, 0xF1};
    mi2c_.WriteRegister(STA369_Address, 0x66, array_set, 3);


  }

  /*!
   *  Mute Programming Routine
   *  This is called whenever the Driver needs to program a mute.
   *  Note that for unmuting, Program Volume is called.
   */
  void ProgramMute(void)
  {
    mi2c_.WriteRegister(STA369_Address, STA369_Register_MasterVolume, kMuteVolume);
  }

  /*!
   *  Volume Programming Routine
   *  This is called whenever the Driver needs to program volume.
   *  Note that for unmuting, Program Volume is called.
   */
  void ProgramVolume(uint16_t leftVol, uint16_t rightVol, uint16_t)
  {
    // This map is only valid for Channel volume, NOT master volume!!
    static const uint16_t STA369_DAC_Map[] = {
        237, 236, 230, 222, 216, 210, 204, 200, 194, 188, 188, 184, 180,
        176, 172, 168, 164, 160, 156, 154, 150, 150, 148, 144, 142, 140,
        136, 134, 132, 130, 128, 126, 126, 124, 122, 122, 120, 118, 117,
        116, 114, 113, 112, 112, 111, 110, 109, 108, 107, 106, 105, 104,
        103, 102, 100, 98,  97,  96,  95,  94,  93,  92,  91,  90,
    };

    uint16_t new_vol = STA369_DAC_Map[(leftVol + rightVol) >> 3];
    mi2c_.WriteRegister(STA369_Address, STA369_Register_MonoVolume, new_vol);
    saved_vol_ = new_vol;

    //USED(mono); // silence warnings about mono -- we don't use it for CS4341
    //uint16_t leftDiv2 = left >> 1;
    //uint16_t rightDiv2 = right >> 1;
    //mi2c_.WriteRegister(CS4341_Address, CS4341_Reg_LeftVolume,
    //                    CS4341_DAC_Map[leftDiv2]);
    //mi2c_.WriteRegister(CS4341_Address, CS4341_Reg_RightVolume,
    //                    CS4341_DAC_Map[rightDiv2]);
  }

  /*!
   *  External RTL routine.
   *  This is called every 5ms, do whatever you need to do actively here.
   */
  void ExternalRTL(void)
  {
    //ADC subroutine
    timerForSec++;
    if(1000==timerForSec)//5 seconds per times
    {
      timerForSec=0x00;
      xout << GetBatVoltage();
      xout << "\n";
      if(624>GetBatVoltage())  //648--15.6V; 633--15.3V; 
        {
          timesOnDet++;
          if(3==timesOnDet)
          {
            regs_.Write<RegIndex::ForcePowerDown>( 0 );
            timesOnDet=0x00;
          }
        }
        else
        {
          timesOnDet=0x00;
        }

    }
  }

  /*!
   *  Exit Bonded
   *  This is called whenever the Driver exits the Bonded State
   */
  void ExitBonded(void)
  {
  }

  /*!
   *  Exit AuxIn
   *  This is called whenever the Driver exits the AuxIn state
   */
  void ExitAuxIn(void) 
  {
  }

  /*!
   *  Exit Standby
   *  This is called whenever the Driver exits the Standby state
   */
  void ExitStandby(void)
  {

  }

  /*!
   *  Enter Bonded
   *  This is called whenever the Driver enters the Bonded state
   */
  void EnterBonded(void)
  {
    

    driver::AMP_RST.SetAsOutput(true);
    driver::AMP_PWDN.SetAsOutput(true);



  }

  /*!
   *  Enter AuxIn
   *  This is called whenever the Driver enters the AuxIn state
   */
  void EnterAuxIn(void)
  {
  }

  /*!
   *  Enter Standby
   *  This is called whenever the Driver enters the Standby state
   */
  void EnterStandby(void)
  {

      driver::AMP_PWDN.SetAsOutput(false);

      //driver::AMP_PWDN.SetAsOutput(false);

  }

  /*!
   * Factory Reset
   * This is called whenever there is a 6-click on the SLB.
   * Reset anything you need to here.
   */
  void FactoryReset(void)
  {
  }

  /*!
   *  User Data Changed
   *  This is called whenever bUSR is updated (when changed or on bond).
   *  Do whatever you like here.
   */
  void UserDataChanged(void)
  {
  }
PRODUCT_DRIVER_END
