classdef GetCAMBasicVehicleContainerHighFrequencySteeringWheelAngle < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [SteeringWheelAngleValue, SteeringWheelAngleConfidence] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                SteeringWheelAngleValue = int32(0);
                SteeringWheelAngleConfidence = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMBasicVehicleContainerHighFrequencySteeringWheelAngle', StationID, ...
                    coder.wref(SteeringWheelAngleValue), coder.wref(SteeringWheelAngleConfidence));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMBasicVehicleContainerHighFrequencySteeringWheelAngle';
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