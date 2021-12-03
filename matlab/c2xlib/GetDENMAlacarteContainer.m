classdef GetDENMAlacarteContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [LanePosition, ExternalTemperature, PositioningSolution] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                LanePosition = int32(0);
                ExternalTemperature = int32(0);
                PositioningSolution = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMAlacarteContainer', StationID, SequenceNumber, ...
                    coder.ref(LanePosition), coder.ref(ExternalTemperature), coder.ref(PositioningSolution));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMAlacarteContainer';
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