classdef GetCAMBasicVehicleContainerHighFrequencyAccelerationControl < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        BufferSize = 1;  
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [AccelerationControl, ActualBufferSize] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                AccelerationControl = uint8(zeros(obj.BufferSize, 1));
                ActualBufferSize = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMBasicVehicleContainerHighFrequencyAccelerationControl', StationID, ...
                    coder.ref(AccelerationControl), length(AccelerationControl), coder.ref(ActualBufferSize));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMBasicVehicleContainerHighFrequencyAccelerationControl';
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