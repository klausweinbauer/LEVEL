classdef SetCAMEmergencyContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
    
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, LightBarSirenInUse, CauseCode, SubCauseCode, EmergencyPriority) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                TmpLightBarSirenInUse = uint8(LightBarSirenInUse);
                TmpEmergencyPriority = uint8(EmergencyPriority);
                coder.cinclude(LibConfig.getCAMHeader());
                err = coder.ceval('setCAMEmergencyContainer', StationID, coder.ref(TmpLightBarSirenInUse), length(TmpLightBarSirenInUse), ...
                    CauseCode, SubCauseCode, coder.ref(TmpEmergencyPriority), length(TmpEmergencyPriority));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetCAMEmergencyContainer';
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