classdef SetDENMStationaryVehicleContainerCarryingDangerousGoodsEActCode < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, EmergencyActionCode)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                TmpEmergencyActionCode = uint8(EmergencyActionCode);
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('setDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode', ... 
                    StationID, SequenceNumber, coder.ref(TmpEmergencyActionCode), length(TmpEmergencyActionCode));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetDENMStationaryVehicleContainerCarryingDangerousGoodsEmergencyActionCode';
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