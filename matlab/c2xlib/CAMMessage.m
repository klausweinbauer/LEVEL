classdef CAMMessage < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        StationID = 0;
        HeighFrequencyContainerType = 0;  
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                err = coder.ceval('createCAM', obj.StationID, obj.HeighFrequencyContainerType);
                LibConfig.printErrorCode(err);
            end 
        end
        
        function [StationID] = stepImpl(obj)
            StationID = obj.StationID;                       
        end
        
        function releaseImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                err = coder.ceval('deleteCAM', obj.StationID);
                LibConfig.printErrorCode(err);
            end    
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'CAMMessage';
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