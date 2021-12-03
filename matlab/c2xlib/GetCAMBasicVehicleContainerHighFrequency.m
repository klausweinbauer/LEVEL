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
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMBasicVehicleContainerHighFrequency', StationID, coder.ref(HeadingValue), ...
                    coder.ref(HeadingConfidence), coder.ref(SpeedValue), coder.ref(SpeedConfidence), coder.ref(DriveDirection), ...
                    coder.ref(VehicleLengthValue), coder.ref(VehicleLengthConfidenceIndication), coder.ref(VehicleWidth), ...
                    coder.ref(LongitudinalAccelerationValue), coder.ref(LongitudinalAccelerationConfidence), coder.ref(CurvatureValue), ...
                    coder.ref(CurvatureConfidence), coder.ref(CurvatureCalculationMode), coder.ref(YawRateValue), coder.ref(YawRateConfidence));
            end            
        end
        
        function releaseImpl(~)            
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