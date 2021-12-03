classdef CAMMessage < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        StationID = 0;
        HighFrequencyContainer = 'NONE';
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    properties (Hidden, Constant)
        HighFrequencyContainerSet = matlab.system.StringSet({'NONE', 'BasicVehicleContainerHighFrequency', 'RSUContainerHighFrequency'})
    end

    methods (Access = protected)
        function setupImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                err = coder.ceval('createCAM', obj.StationID);
                if strcmp(obj.HighFrequencyContainer, 'BasicVehicleContainerHighFrequency')
                    coder.ceval('defineCAMHighFrequencyContainer', obj.StationID, 1);
                elseif strcmp(obj.HighFrequencyContainer, 'RSUContainerHighFrequency')
                    coder.ceval('defineCAMHighFrequencyContainer', obj.StationID, 2);
                else
                    coder.ceval('defineCAMHighFrequencyContainer', obj.StationID, 0);
                end
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