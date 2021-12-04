classdef SetCAMSpecialTransportContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, SpecialTransportType, LightBarSirenInUse) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                TmpSpecialTransportType = uint8(SpecialTransportType);
                TmpLightBarSirenInUse = uint8(LightBarSirenInUse);
                coder.cinclude(LibConfig.getCAMHeader());
                err = coder.ceval('setCAMSpecialTransportContainer', StationID, coder.ref(TmpSpecialTransportType), ...
                    length(TmpSpecialTransportType), coder.ref(TmpLightBarSirenInUse), length(TmpLightBarSirenInUse));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetCAMSpecialTransportContainer';
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