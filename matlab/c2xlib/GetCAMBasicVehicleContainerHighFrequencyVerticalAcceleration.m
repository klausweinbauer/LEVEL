classdef GetCAMBasicVehicleContainerHighFrequencyVerticalAcceleration < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [VerticalAccelerationValue, VerticalAccelerationConfidence] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                VerticalAccelerationValue = int32(0);
                VerticalAccelerationConfidence = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMBasicVehicleContainerHighFrequencyVerticalAcceleration', StationID, ...
                    coder.wref(VerticalAccelerationValue), coder.wref(VerticalAccelerationConfidence));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMBasicVehicleContainerHighFrequencyVerticalAcceleration';
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