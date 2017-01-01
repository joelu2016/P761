from DASIConsts import AudioMode, BitsPerFrame, MstrClkFreq, BitClkFreq
from SKUID import SKUID_Family
from SKAAMode import SKAAMode, VolumeMode
from CommonTypes import byte
from CommonTypes import Selectable, byte, VolumeSteps

kAllConsts = {
  #LED Constants
  'kRedDim'         : byte(2),
  'kRedBright'      : byte(30),
  'kAmberDim'       : byte(3),
  'kAmberBright'    : byte(50),
  'kGreenDim'       : byte(5),
  'kGreenBright'    : byte(95),
  'kLEDIsActiveHigh': True,

  #Timeout Constants
  'kStandbyTimeoutInSecs'    : 10 * 60,
  'kSleepTimeoutInSecs'      : 60 * 60,
  'kPowerConserveStandbyTimeoutInSecs' : 3 * 60,
  'kPowerConserveSleepTimeoutInSecs'   : 9 * 60,
  'kVolumeStepSize'         : VolumeSteps(20).StepSize(),
  'kVolumeOffsetAdjust'     : VolumeSteps(20).OffsetAdjust(),

  #Pin/Button Constants
  'kIsEnabledHostVolumePin'      : True,
  'kIsActiveHighPowerConservePin': True,
  'kIsActiveHighHostVolumePin'   : True,
  'kIsActiveHighMuteButton'      : True,
  'kIsActiveHighAuxInPin'        : False,
  'kIsActiveHighPowerDownPrepPin': False,
  'kIsEnabledVTRButton'          : True,
  'kIsActiveHighVTRButton'       : False,
  'kVTRButtonIsbVTRNotnVTR'      : True,
  'kIgnorePowerDownPrepOnBoot'   : True,
  #'kPowerDownPrepPowerOffDelayInMs'   : 500,
  'kPowerDownPrepCausesReset'    : False,
  'kDisableMonoMix'              : True,



  #Audio Mode Support Constants
  'kSupportedAudioModes': AudioMode.BitVector([
    AudioMode.Left, AudioMode.Right, AudioMode.Mono,
  ]),
  'kAudioModeDefault': AudioMode.Mono,

  #Other Audio Constants
  #'kVolumeStepSize'         : byte(16),

  #Miscellaneous Constants
  'kClientDriverName[]': '"P761"',
  'kDefaultClusterName[]': '""',
  'kSkaaMode'        : SKAAMode.Basic,
  'kSkuIdFamilyIndex': SKUID_Family.Ryobi,
  'kDefaultSkuIdByte': byte(2),
}
