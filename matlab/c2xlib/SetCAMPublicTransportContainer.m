classdef SetCAMPublicTransportContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, EmbarkationStatus, PtActivationType, PtActivationData) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                TmpPtActivationData = uint8(PtActivationData);
                coder.cinclude(LibConfig.getCAMHeader());
                err = coder.ceval('setCAMPublicTransportContainer', StationID, int32(EmbarkationStatus), ...
                    int32(PtActivationType), coder.ref(TmpPtActivationData), length(TmpPtActivationData));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'SetCAMPublicTransportContainer';
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