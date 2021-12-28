/*
 * File: CAM_DENM_Example.c
 *
 * Code generated for Simulink model 'CAM_DENM_Example'.
 *
 * Model version                  : 1.9
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Dec 28 07:49:49 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-A
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "CAM_DENM_Example.h"

/* Block signals and states (default storage) */
DW rtDW;

/* Model step function */
void CAM_DENM_Example_step(void)
{
  real_T rtb_PulseGenerator;
  int32_T MsgLength;
  int32_T b_varargout_2;
  int32_T err;
  uint8_T MsgBytes[255];

  /* DiscretePulseGenerator: '<Root>/Pulse Generator' */
  rtb_PulseGenerator = ((rtDW.clockTickCounter < 1) && (rtDW.clockTickCounter >=
    0));
  if (rtDW.clockTickCounter >= 1) {
    rtDW.clockTickCounter = 0;
  } else {
    rtDW.clockTickCounter++;
  }

  /* End of DiscretePulseGenerator: '<Root>/Pulse Generator' */

  /* MATLABSystem: '<Root>/AddCAMBasicVehicleContainerLowFrequencyPathPoint' incorporates:
   *  MATLABSystem: '<Root>/CAMMessage'
   */
  if (rtDW.obj_j.SampleActive && (rtb_PulseGenerator != 0.0)) {
    rtDW.obj_j.SampleActive = false;
    err = addCAMBasicVehicleContainerLowFrequencyPathPoint(1.0, 0.0, 0.0, 0.0,
      0.0);
    if (err < 0) {
      memset(&MsgBytes[0], 0, 255U * sizeof(uint8_T));
      MsgLength = 0;
      getLastErrMsg(&MsgBytes[0], 255.0, &MsgLength);
    }
  } else if ((!rtDW.obj_j.SampleActive) && (rtb_PulseGenerator == 0.0)) {
    rtDW.obj_j.SampleActive = true;
  }

  /* End of MATLABSystem: '<Root>/AddCAMBasicVehicleContainerLowFrequencyPathPoint' */

  /* MATLABSystem: '<Root>/CAMTransmitter' incorporates:
   *  Constant: '<Root>/Constant1'
   *  MATLABSystem: '<Root>/CAMMessage'
   */
  rtb_PulseGenerator = 1.0;
  setCAMTransmissionFrequency(10.0);
  setCAMIDsForTransmission(&rtb_PulseGenerator, 1.0);

  /* MATLABSystem: '<Root>/GetDENMSituationContainer' incorporates:
   *  MATLABSystem: '<Root>/DENMMessage'
   */
  MsgLength = 0;
  b_varargout_2 = 0;
  err = 0;
  getDENMSituationContainer(2.0, 1.0, &MsgLength, &b_varargout_2, &err);

  /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem' incorporates:
   *  ActionPort: '<S2>/Action Port'
   */
  /* If: '<S1>/If' incorporates:
   *  Constant: '<Root>/CurvatureCalcMode_unavailable'
   *  Constant: '<Root>/CurvatureConf_unavailable'
   *  Constant: '<Root>/CurvatureValue_straight'
   *  Constant: '<Root>/DriveDirection_forward'
   *  Constant: '<Root>/HeadingConf_within1Degree'
   *  Constant: '<Root>/Length_30cm'
   *  Constant: '<Root>/LongAccelConf_0.1m//ss'
   *  Constant: '<Root>/NoTrailerPresent'
   *  Constant: '<Root>/SpeedConf_10cm//s'
   *  Constant: '<Root>/West'
   *  Constant: '<Root>/Width_10cm'
   *  Constant: '<Root>/YawRateConf_unavailable'
   *  Constant: '<Root>/YawRateValue_unavailable'
   *  Inport: '<S2>/In1'
   *  MATLABSystem: '<Root>/CAMMessage'
   *  MATLABSystem: '<Root>/SetCAMBasicVehicleContainerHighFrequency'
   */
  err = setCAMBasicVehicleContainerHighFrequency(1.0, 2700.0, 10.0, 40.0, 10.0,
    0.0, 3.0, 0.0, 1.0, 0.0, 1.0, 0.0, 7.0, 2.0, 32767.0, 8.0);

  /* End of Outputs for SubSystem: '<S1>/If Action Subsystem' */

  /* MATLABSystem: '<Root>/SetCAMBasicVehicleContainerHighFrequency' */
  if (err < 0) {
    memset(&MsgBytes[0], 0, 255U * sizeof(uint8_T));
    MsgLength = 0;
    getLastErrMsg(&MsgBytes[0], 255.0, &MsgLength);
  }

  /* MATLABSystem: '<Root>/SetCAMBasicVehicleContainerLowFrequency' incorporates:
   *  Constant: '<Root>/VehicleRole_default'
   *  Constant: '<Root>/daytimeRunningLightsOn'
   *  MATLABSystem: '<Root>/CAMMessage'
   */
  rtb_PulseGenerator = 4.0;
  err = setCAMBasicVehicleContainerLowFrequency(1.0, 0.0, &rtb_PulseGenerator,
    1.0);
  if (err < 0) {
    memset(&MsgBytes[0], 0, 255U * sizeof(uint8_T));
    MsgLength = 0;
    getLastErrMsg(&MsgBytes[0], 255.0, &MsgLength);
  }

  /* End of MATLABSystem: '<Root>/SetCAMBasicVehicleContainerLowFrequency' */

  /* MATLABSystem: '<Root>/SetCAMHeader' incorporates:
   *  Constant: '<Root>/Constant'
   *  MATLABSystem: '<Root>/CAMMessage'
   */
  err = setCAMHeader(1.0, 1, 1);
  if (err < 0) {
    memset(&MsgBytes[0], 0, 255U * sizeof(uint8_T));
    MsgLength = 0;
    getLastErrMsg(&MsgBytes[0], 255.0, &MsgLength);
  }

  /* End of MATLABSystem: '<Root>/SetCAMHeader' */

  /* MATLABSystem: '<Root>/GetDENMHeader' incorporates:
   *  MATLABSystem: '<Root>/DENMMessage'
   */
  MsgLength = 0;
  b_varargout_2 = 0;
  err = getDENMHeader(2.0, 1.0, &MsgLength, &b_varargout_2);
  if (err < 0) {
    memset(&MsgBytes[0], 0, 255U * sizeof(uint8_T));
    MsgLength = 0;
    getLastErrMsg(&MsgBytes[0], 255.0, &MsgLength);
  }

  /* End of MATLABSystem: '<Root>/GetDENMHeader' */
}

/* Model initialize function */
void CAM_DENM_Example_initialize(void)
{
  {
    int32_T Ns;
    uint8_T MsgBytes[255];

    /* Start for MATLABSystem: '<Root>/CAMMessage' */
    rtDW.obj.matlabCodegenIsDeleted = false;
    rtDW.obj.isInitialized = 1;
    Ns = createCAM(1.0);
    defineCAMHighFrequencyContainer(1.0, 1.0);
    if (Ns < 0) {
      memset(&MsgBytes[0], 0, 255U * sizeof(uint8_T));
      Ns = 0;
      getLastErrMsg(&MsgBytes[0], 255.0, &Ns);
    }

    rtDW.obj.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<Root>/CAMMessage' */

    /* Start for MATLABSystem: '<Root>/AddCAMBasicVehicleContainerLowFrequencyPathPoint' */
    rtDW.obj_j.SampleActive = true;
    rtDW.obj_j.matlabCodegenIsDeleted = false;
    rtDW.obj_j.isSetupComplete = true;

    /* Start for MATLABSystem: '<Root>/CAMTransmitter' */
    rtDW.obj_k.matlabCodegenIsDeleted = false;
    rtDW.obj_k.isInitialized = 1;
    startCAMTransmitter(1997.0);
    rtDW.obj_k.isSetupComplete = true;

    /* Start for MATLABSystem: '<Root>/DENMMessage' */
    rtDW.obj_h.matlabCodegenIsDeleted = false;
    rtDW.obj_h.isInitialized = 1;
    Ns = createDENM(2.0, 1.0);
    if (Ns < 0) {
      memset(&MsgBytes[0], 0, 255U * sizeof(uint8_T));
      Ns = 0;
      getLastErrMsg(&MsgBytes[0], 255.0, &Ns);
    }

    rtDW.obj_h.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<Root>/DENMMessage' */

    /* Start for MATLABSystem: '<Root>/GetDENMSituationContainer' */
    rtDW.obj_bs.matlabCodegenIsDeleted = false;
    rtDW.obj_bs.isSetupComplete = true;

    /* Start for MATLABSystem: '<Root>/SetCAMBasicVehicleContainerHighFrequency' */
    rtDW.obj_hy.matlabCodegenIsDeleted = false;
    rtDW.obj_hy.isSetupComplete = true;

    /* Start for MATLABSystem: '<Root>/SetCAMBasicVehicleContainerLowFrequency' */
    rtDW.obj_i.matlabCodegenIsDeleted = false;
    rtDW.obj_i.isSetupComplete = true;

    /* Start for MATLABSystem: '<Root>/SetCAMHeader' */
    rtDW.obj_g.matlabCodegenIsDeleted = false;
    rtDW.obj_g.isSetupComplete = true;

    /* Start for MATLABSystem: '<Root>/GetDENMHeader' */
    rtDW.obj_a.matlabCodegenIsDeleted = false;
    rtDW.obj_a.isSetupComplete = true;

    /* Start for MATLABSystem: '<Root>/DENMReceiver' */
    rtDW.obj_b.matlabCodegenIsDeleted = false;
    rtDW.obj_b.isInitialized = 1;
    startDENMReceiver(1998.0);
    rtDW.obj_b.isSetupComplete = true;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
