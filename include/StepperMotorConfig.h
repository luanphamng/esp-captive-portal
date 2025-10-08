#ifndef _DEFAULT_CONFIG_H
#define _DEFAULT_CONFIG_H

#define R_SENSE 0.11f
#define DEFAULT_PERCENT_TO_SPEED_RATIO      3500 // 1 percent = ??? steps / sec. Example: 1 percent = 20 steps/sec.
#define DEFAULT_MICROSTEP                   8
#define DEFAULT_RMS_CURRENT                 1650
#define DEFAULT_ACCELERATION_IN_SEC         3

#define DEFAULT_AUTOTIME                    60
#define DEFAULT_DURATION                    60
#define DEFAULT_SPEED                       50
#define DEFAULT_SENS                        1
#define DEFAULT_AUTOSPEED                   50

#define DEFAULT_THERMAL_ENABLE              true

#define DEFAULT_BLOWER_VOLT                 0   // Volt (V)
#define DEFAULT_STALL_TIME                  3   // sec

#define DEFAULT_MPU_IDLE_TIME               3   // s
#define DEFAULT_OVERHEAT_TIME               15  // min

#define STEPPER_TASK_HANDLER_TICK_LENGTH_MS 50

#define DEFAULT_STEPPER_SPEED_LOW           50      // %
#define DEFAULT_STEPPER_SPEED_MEDIUM        65      // %
#define DEFAULT_STEPPER_SPEED_HIGH          80      // %

// Stall guard via accel mpu6050
#define DEFAULT_STALL_GUARD_VIA_ACCEL_THRESHOLD     0.03f
#define STALL_GUARD_MAX_NUM_ALLOW           3

// 11.2 Tuning StallGuard4
#define STALL_GUARD_THRESHOLD_SG4               15      // [0..255]
#define DEFAULT_STALLGUARD4_THRESHOLD           15      // [0..255]
#define DEFAULT_TIME_TO_RESET_STALL_COUNTER     15000   // ms

#define DEFAULT_PROTECTION_TOO_HOT_PAUSE_TIME       120 // secs
#define DEFAULT_PROTECTION_STALLED_PAUSE_TIME       3   // secs
/*
Equation to convert from V[RPM] to VACTUAL

See data sheet TMC2209, Figure 14.1 Software generated motion profile
v[Hz] = VACTUAL * 0.715Hz
v[rps] = v[Hz] / 1 / 200
So that V[rps] = VACTUAL * 0.715 / 1 / 200
Hance, VACTUAL = V[rps] / 0.715 * 200
VACTUAL = V[rpm] / 60 / 0.715 * 200
VACTUAL = percentSpeed / 100 * percent2rpm / 60 / 0.715 * 200
VACTUAL = percentSpeed * percent2rpm / 60 * 200 / 100 / 0.715
VACTUAL = percentSpeed * percent2rpm * 0.05
*/
#define VRPM_TO_VACTUAL                     0.05 // See data sheet TMC2209, Figure 14.1 Software generated motion profile

#define DEFAUT_WIFI_ENABLE                  1
/* Stall guard notes:
TSTEP: time between 2 microstep, refer to PARAMETERS RELATED TO STEALTHCHOP, p44, TMC2209 pdf v103
TSTEP = fCLK / fSTEP

TPWMTHRS: Specifies the upper velocity for operation in StealthChop, StealthChop is disabled if TSTEP falls TPWMTHRS 

TCOOLTHRS: ower threshold velocity for switching on smart energy CoolStep and StallGuard to DIAG output. (unsigned)
Set this parameter to disable CoolStep at low speeds, where it 
cannot work reliably. The stall output signal become enabled 
when exceeding this velocity. It becomes disabled again once 
the velocity falls below this threshold.
TCOOLTHRS ≥ TSTEP > TPWMTHRS
- CoolStep is enabled, if configured (only with StealthChop)
- Stall output signal on pin DIAG is enabled


*/

/************************** LIMITATIONS ********************************/
#define LOWER_LIMIT_PERCENT_TO_SPEED_RATIO      3000 // 1 percent = ??? steps / sec. Example: 1 percent = 20 steps/sec.
#define LOWER_LIMIT_MICROSTEP                   8
#define LOWER_LIMIT_RMS_CURRENT                 1000
#define LOWER_LIMIT_ACCELERATION_IN_SEC         3

#define LOWER_LIMIT_AUTOTIME                    1
#define LOWER_LIMIT_DURATION                    15
#define LOWER_LIMIT_SPEED                       0
#define LOWER_LIMIT_SENS                        0
#define LOWER_LIMIT_AUTOSPEED                   0

#define LOWER_LIMIT_BLOWER_VOLT                 0   // Volt (V)
#define LOWER_LIMIT_STALL_TIME                  3   // sec
#define LOWER_LIMIT_G_THRESHOLD                 0  // %
#define LOWER_LIMIT_MPU_IDLE_TIME               0   // s
#define LOWER_LIMIT_OVERHEAT_TIME               10  // min
#define LOWER_STALLGUARD4_THRESHOLD             0       // [0..250]


#define UPPER_LIMIT_PERCENT_TO_SPEED_RATIO      5000 // 1 percent = ??? steps / sec. Example: 1 percent = 20 steps/sec.
#define UPPER_LIMIT_MICROSTEP                   64
#define UPPER_LIMIT_RMS_CURRENT                 2000
#define UPPER_LIMIT_ACCELERATION_IN_SEC         10

#define UPPER_LIMIT_AUTOTIME                    120
#define UPPER_LIMIT_DURATION                    480
#define UPPER_LIMIT_SPEED                       100
#define UPPER_LIMIT_SENS                        100
#define UPPER_LIMIT_AUTOSPEED                   100

#define UPPER_LIMIT_BUPPER_VOLT                 5   // Volt (V)
#define UPPER_LIMIT_STALL_TIME                  60   // sec
#define UPPER_LIMIT_G_THRESHOLD                 30  // %
#define UPPER_LIMIT_MPU_IDLE_TIME               50   // s
#define UPPER_LIMIT_OVERHEAT_TIME               60  // min
#define UPPER_STALLGUARD4_THRESHOLD             250     // [0..250]

#endif