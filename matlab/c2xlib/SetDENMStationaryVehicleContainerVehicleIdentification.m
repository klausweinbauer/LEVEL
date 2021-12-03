classdef SetDENMStationaryVehicleContainerVehicleIdentification < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, WMInumber, VDS)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                TmpWMINumber = uint8(WMInumber);
                TmpVDS = uint8(VDS);
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('setDENMStationaryVehicleContainerVehicleIdentification', StationID, SequenceNumber, ... 
                    coder.ref(TmpWMINumber), length(TmpWMINumber), coder.ref(TmpVDS), length(TmpVDS));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetDENMStationaryVehicleContainerVehicleIdentification';
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