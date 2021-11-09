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
        
        function [HeadingValue, HeadingConfidence, SpeedValue, SpeedConfidence, DriveDirection, VehicleLengthValue, VehicleLengthConfidenceIndication, VehicleWidth, LongitudinalAccelerationValue, LongitudinalAccelerationConfidence, CurvatureValue, CurvatureConfidence, CurvatureCalculationMode, YawRateValue, YawRateConfidence] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                err = coder.ceval('getCAMBasicVehicleContainerHighFrequency', StationID, coder.wref(HeadingValue), coder.wref(HeadingConfidence), coder.wref(SpeedValue), coder.wref(SpeedConfidence), coder.wref(DriveDirection), coder.wref(VehicleLengthValue), coder.wref(VehicleLengthConfidenceIndication), coder.wref(VehicleWidth), coder.wref(LongitudinalAccelerationValue), coder.wref(LongitudinalAccelerationConfidence), coder.wref(CurvatureValue), coder.wref(CurvatureConfidence), coder.wref(CurvatureCalculationMode), coder.wref(YawRateValue), coder.wref(YawRateConfidence));
                obj.printErrorCode(err);
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
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xcam', linkLibExt);
            % Other linking parameters
            libPath = './';
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            buildInfo.addLinkObjects(libName,libPath,libPriority,libPreCompiled,libLinkOnly);
        end
    end
    methods (Access = protected)
        function [HeadingValue, HeadingConfidence, SpeedValue, SpeedConfidence, DriveDirection, VehicleLengthValue, VehicleLengthConfidenceIndication, VehicleWidth, LongitudinalAccelerationValue, LongitudinalAccelerationConfidence, CurvatureValue, CurvatureConfidence, CurvatureCalculationMode, YawRateValue, YawRateConfidence] = getOutputSizeImpl(obj)
            HeadingValue = [1, 1];
HeadingConfidence = [1, 1];
SpeedValue = [1, 1];
SpeedConfidence = [1, 1];
DriveDirection = [1, 1];
VehicleLengthValue = [1, 1];
VehicleLengthConfidenceIndication = [1, 1];
VehicleWidth = [1, 1];
LongitudinalAccelerationValue = [1, 1];
LongitudinalAccelerationConfidence = [1, 1];
CurvatureValue = [1, 1];
CurvatureConfidence = [1, 1];
CurvatureCalculationMode = [1, 1];
YawRateValue = [1, 1];
YawRateConfidence = [1, 1];

        end 
        
        function [HeadingValue, HeadingConfidence, SpeedValue, SpeedConfidence, DriveDirection, VehicleLengthValue, VehicleLengthConfidenceIndication, VehicleWidth, LongitudinalAccelerationValue, LongitudinalAccelerationConfidence, CurvatureValue, CurvatureConfidence, CurvatureCalculationMode, YawRateValue, YawRateConfidence] = isOutputFixedSizeImpl(obj)
            true;
true;
true;
true;
true;
true;
true;
true;
true;
true;
true;
true;
true;
true;
true;

        end
        
        function [HeadingValue, HeadingConfidence, SpeedValue, SpeedConfidence, DriveDirection, VehicleLengthValue, VehicleLengthConfidenceIndication, VehicleWidth, LongitudinalAccelerationValue, LongitudinalAccelerationConfidence, CurvatureValue, CurvatureConfidence, CurvatureCalculationMode, YawRateValue, YawRateConfidence] = getOutputDataTypeImpl(obj)
            'int32';
'int32';
'int32';
'int32';
'int32';
'int32';
'int32';
'int32';
'int32';
'int32';
'int32';
'int32';
'int32';
'int32';
'int32';

        end
        
        function [HeadingValue, HeadingConfidence, SpeedValue, SpeedConfidence, DriveDirection, VehicleLengthValue, VehicleLengthConfidenceIndication, VehicleWidth, LongitudinalAccelerationValue, LongitudinalAccelerationConfidence, CurvatureValue, CurvatureConfidence, CurvatureCalculationMode, YawRateValue, YawRateConfidence] = isOutputComplexImpl(obj)
            false;
false;
false;
false;
false;
false;
false;
false;
false;
false;
false;
false;
false;
false;
false;

        end
    end
end