classdef GetDENMStationaryVehicleContainerNumberOfOccupants < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [NumberOfOccupants] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                NumberOfOccupants = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMStationaryVehicleContainerNumberOfOccupants', StationID, SequenceNumber, ...
                    coder.ref(NumberOfOccupants));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMStationaryVehicleContainerNumberOfOccupants';
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