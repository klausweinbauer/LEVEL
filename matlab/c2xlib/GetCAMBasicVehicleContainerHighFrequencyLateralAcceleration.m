classdef GetCAMBasicVehicleContainerHighFrequencyLateralAcceleration < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [LateralAccelerationValue, LateralAccelerationConfidence] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                LateralAccelerationValue = int32(0);
                LateralAccelerationConfidence = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMBasicVehicleContainerHighFrequencyLateralAcceleration', StationID, ...
                    coder.wref(LateralAccelerationValue), coder.wref(LateralAccelerationConfidence));
            end            
        end
        
        function releaseImpl(~)            
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMBasicVehicleContainerHighFrequencyLateralAcceleration';
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