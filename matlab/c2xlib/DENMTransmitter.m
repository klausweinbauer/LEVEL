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
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('startDENMTransmitter', obj.Port);
            end 
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, f_send) 
            tmpStationID = int32(StationID);
            tmpSequenceNumber = int32(SequenceNumber);
            if coder.target('Rtw') || coder.target('Sfun')
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('setDENMTransmissionFrequency', f_send);
                coder.ceval('setDENMTransmissionSource', tmpStationID, tmpSequenceNumber);                
            end            
        end
        
        function releaseImpl(~)   
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getDENMHeader());
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