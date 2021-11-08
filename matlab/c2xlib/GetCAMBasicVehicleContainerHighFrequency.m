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
                coder.ceval('getCAMBasicVehicleContainerHighFrequency', StationID, coder.wref(HeadingValue), coder.wref(HeadingConfidence), coder.wref(SpeedValue), coder.wref(SpeedConfidence), coder.wref(DriveDirection), coder.wref(VehicleLengthValue), coder.wref(VehicleLengthConfidenceIndication), coder.wref(VehicleWidth), coder.wref(LongitudinalAccelerationValue), coder.wref(LongitudinalAccelerationConfidence), coder.wref(CurvatureValue), coder.wref(CurvatureConfidence), coder.wref(CurvatureCalculationMode), coder.wref(YawRateValue), coder.wref(YawRateConfidence));
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
end