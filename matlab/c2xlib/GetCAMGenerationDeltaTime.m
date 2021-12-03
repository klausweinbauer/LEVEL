classdef GetCAMGenerationDeltaTime < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [GenerationDeltaTime] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                GenerationDeltaTime = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMGenerationDeltaTime', StationID, coder.wref(GenerationDeltaTime));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMGenerationDeltaTime';
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