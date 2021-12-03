classdef DENMTransmitter < matlab.System & coder.ExternalDependency
    % IDsToTransmit is a vector of StationIDs which should be sent from this instance
    
    properties (Nontunable)
        Port = 1998;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude('c2xdenm.h');
                coder.ceval('startDENMTransmitter', obj.Port);
            end 
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, f_send) 
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude('c2xdenm.h');
                coder.ceval('setDENMTransmissionFrequency', f_send);
                coder.ceval('setDENMTransmissionSource', StationID, SequenceNumber);
            end            
        end
        
        function releaseImpl(~)   
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude('c2xdenm.h');
                coder.ceval('stopDENMTransmitter');
            end          
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'DENMTransmitter';
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
            LibConfig.updateBuildInfoDENM(buildInfo, buildContext);
        end
    end
end