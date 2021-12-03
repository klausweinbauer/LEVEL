classdef GetCAMRescueContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        LightBarSirenInUseSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [LightBarSirenInUse] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                LightBarSirenInUse = uint8(zeros(obj.LightBarSirenInUseSize, 1));
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMRescueContainer', StationID, coder.ref(LightBarSirenInUse), length(LightBarSirenInUse));
            end
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMRescueContainer';
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
    methods (Access = protected)
        function [LightBarSirenInUse] = getOutputSizeImpl(obj)
            LightBarSirenInUse = [obj.LightBarSirenInUseSize 1];
        end 
        
        function [LightBarSirenInUse] = isOutputFixedSizeImpl(obj)
            LightBarSirenInUse = true;
        end
        
        function [LightBarSirenInUse] = getOutputDataTypeImpl(obj)
            LightBarSirenInUse = 'uint8'; 
        end
        
        function [LightBarSirenInUse] = isOutputComplexImpl(obj)
            LightBarSirenInUse = false;
        end
    end
end