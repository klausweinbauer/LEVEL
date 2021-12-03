classdef SetDENMRoadWorksContainerExtendedLightBarSiren < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, LightBarSirenInUse)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                TmpLightBarSirenInUser = uint8(LightBarSirenInUse);
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('setDENMRoadWorksContainerExtendedLightBarSiren', StationID, SequenceNumber, ...
                    coder.ref(TmpLightBarSirenInUser), length(TmpLightBarSirenInUser));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetDENMRoadWorksContainerExtendedLightBarSiren';
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