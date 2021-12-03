classdef CAMReceiver < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        Port = 1997;  
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('startCAMReceiver', obj.Port);
            end 
        end
        
        function stepImpl(obj)                        
        end
        
        function releaseImpl(~)  
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('stopCAMReceiver');
            end           
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'CAMReceiver';
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