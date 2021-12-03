classdef CAMTransmitter < matlab.System & coder.ExternalDependency
    % IDsToTransmit is a vector of StationIDs which should be sent from this instance
    
    properties (Nontunable)
        Port = 1997;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('startCAMTransmitter', obj.Port);
            end 
        end
        
        function [] = stepImpl(obj, IDsToTransmit, f_send) 
            Size = length(IDsToTransmit);
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('setCAMTransmissionFrequency', f_send);
                coder.ceval('setCAMIDsForTransmission', coder.ref(IDsToTransmit), Size);
            end            
        end
        
        function releaseImpl(~)   
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('stopCAMTransmitter');
            end          
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'CAMTransmitter';
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