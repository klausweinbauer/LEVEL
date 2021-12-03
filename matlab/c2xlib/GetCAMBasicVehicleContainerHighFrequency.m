classdef GetCAMBasicVehicleContainerHighFrequency < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [HeadingValue, HeadingConfidence, SpeedValue, SpeedConfidence, DriveDirection, VehicleLengthValue, ...
            VehicleLengthConfidenceIndication, VehicleWidth, LongitudinalAccelerationValue, ...
            LongitudinalAccelerationConfidence, CurvatureValue, CurvatureConfidence, CurvatureCalculationMode, ...
            YawRateValue, YawRateConfidence] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                HeadingValue = int32(0);
                HeadingConfidence = int32(0);
                SpeedValue = int32(0);
                SpeedConfidence = int32(0);
                DriveDirection = int32(0);
                VehicleLengthValue = int32(0);
                VehicleLengthConfidenceIndication = int32(0);
                VehicleWidth = int32(0);
                LongitudinalAccelerationValue = int32(0);
                LongitudinalAccelerationConfidence = int32(0);
                CurvatureValue = int32(0);
                CurvatureConfidence = int32(0);
                CurvatureCalculationMode = int32(0);
                YawRateValue = int32(0);
                YawRateConfidence = int32(0);
                coder.cinclude('c2xcam.h');
                coder.ceval('getCAMBasicVehicleContainerHighFrequency', StationID, coder.ref(HeadingValue), ...
                    coder.ref(HeadingConfidence), coder.ref(SpeedValue), coder.ref(SpeedConfidence), coder.ref(DriveDirection), ...
                    coder.ref(VehicleLengthValue), coder.ref(VehicleLengthConfidenceIndication), coder.ref(VehicleWidth), ...
                    coder.ref(LongitudinalAccelerationValue), coder.ref(LongitudinalAccelerationConfidence), coder.ref(CurvatureValue), ...
                    coder.ref(CurvatureConfidence), coder.ref(CurvatureCalculationMode), coder.ref(YawRateValue), coder.ref(YawRateConfidence));
            end            
        end
        
        function releaseImpl(~)            
        end

        function printErrorCode(~, err)
            if (err == -22)
                error('ERR_CAM_ALREADY_EXISTS')
            elseif (err == -21)
                error('ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE')
            elseif (err == -20)
                error('ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE')
            elseif (err == -40)
                error('ERR_DENM_ALREADY_EXISTS')
            elseif (err == -1)
                error('ERR_MSG_NOT_FOUND')
            elseif (err == -9)
                error('ERR_ARG_NULL')
            elseif (err == -8)
                error('ERR_TRANSMITTER_START')
            elseif (err == -7)
                error('ERR_RECEIVER_START')
            elseif (err == -6)
                error('ERR_DECODE')
            elseif (err == -5)
                error('ERR_ENCODE')
            elseif (err == -4)
                error('ERR_BUFFER_OVERFLOW')
            elseif (err == -3)
                error('ERR_NULL')
            elseif (err == -2)
                error('ERR_ALLOC_FAILED')
            end
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMBasicVehicleContainerHighFrequency';
        end
        
        function supported = isSupportedContext(buildContext)
            myTarget = {'rtw'};
            if  buildContext.isCodeGenTarget(myTarget)
                supported = true;
            else
                error('API only supported for mex, lib, exe, dll');
            end
        end
        
        function updateBuildInfo(buildInfo, buildContext)
            LibConfig.updateBuildInfoCAM(buildInfo, buildContext);
        end
    end
end