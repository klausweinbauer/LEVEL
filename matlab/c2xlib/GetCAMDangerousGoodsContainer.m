classdef GetCAMDangerousGoodsContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        LightBarSirenInUseSize = 1;
        SpecialTransportTypeSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [DangerousGoodsBasic] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                DangerousGoodsBasic = int32(0);
                coder.cinclude('c2xcam.h');
                coder.ceval('getCAMDangerousGoodsContainer', StationID, coder.ref(DangerousGoodsBasic));
            end
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMDangerousGoodsContainer';
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